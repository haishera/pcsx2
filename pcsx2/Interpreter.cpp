/*  PCSX2 - PS2 Emulator for PCs
 *  Copyright (C) 2002-2010  PCSX2 Dev Team
 *
 *  PCSX2 is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU Lesser General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  PCSX2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with PCSX2.
 *  If not, see <http://www.gnu.org/licenses/>.
 */


#include "PrecompiledHeader.h"
#include "Common.h"

#include "R5900OpcodeTables.h"
#include "R5900Exceptions.h"
#include "System/SysThreads.h"

#include "Elfheader.h"

#include "../DebugTools/Breakpoints.h"

#include <float.h>

using namespace R5900;		// for OPCODE and OpcodeImpl

extern int vu0branch, vu1branch;

static int branch2 = 0;
static u32 cpuBlockCycles = 0;		// 3 bit fixed point version of cycle count
static std::string disOut;

static void intEventTest();

// These macros are used to assemble the repassembler functions

static void debugI()
{
	if( !IsDevBuild ) return;
	if( cpuRegs.GPR.n.r0.UD[0] || cpuRegs.GPR.n.r0.UD[1] ) Console.Error("R0 is not zero!!!!");
}

//long int runs=0;

void intBreakpoint(bool memcheck)
{
	u32 pc = cpuRegs.pc;
 	if (CBreakPoints::CheckSkipFirst(pc) != 0)
		return;

	if (!memcheck)
	{
		auto cond = CBreakPoints::GetBreakPointCondition(pc);
		if (cond && !cond->Evaluate())
			return;
	}

	CBreakPoints::SetBreakpointTriggered(true);
	GetCoreThread().PauseSelf();
	throw Exception::ExitCpuExecute();
}

void intMemcheck(u32 op, u32 bits, bool store)
{
	// compute accessed address
	u32 start = cpuRegs.GPR.r[(op >> 21) & 0x1F].UD[0];
	if ((s16)op != 0)
		start += (s16)op;
	if (bits == 128)
		start &= ~0x0F;

	start = standardizeBreakpointAddress(start);
	u32 end = start + bits/8;
	
	auto checks = CBreakPoints::GetMemChecks();
	for (size_t i = 0; i < checks.size(); i++)
	{
		auto& check = checks[i];

		if (check.result == 0)
			continue;
		if ((check.cond & MEMCHECK_WRITE) == 0 && store == true)
			continue;
		if ((check.cond & MEMCHECK_READ) == 0 && store == false)
			continue;

		if (start < check.end && check.start < end)
			intBreakpoint(true);
	}
}

void intCheckMemcheck()
{
	u32 pc = cpuRegs.pc;
	int needed = isMemcheckNeeded(pc);
	if (needed == 0)
		return;

	u32 op = memRead32(needed == 2 ? pc+4 : pc);
	const OPCODE& opcode = GetInstruction(op);

	bool store = (opcode.flags & IS_STORE) != 0;
	switch (opcode.flags & MEMTYPE_MASK)
	{
	case MEMTYPE_BYTE:
		intMemcheck(op,8,store);
		break;
	case MEMTYPE_HALF:
		intMemcheck(op,16,store);
		break;
	case MEMTYPE_WORD:
		intMemcheck(op,32,store);
		break;
	case MEMTYPE_DWORD:
		intMemcheck(op,64,store);
		break;
	case MEMTYPE_QWORD:
		intMemcheck(op,128,store);
		break;
	}
}

static void execI()
{
	// check if any breakpoints or memchecks are triggered by this instruction
	if (isBreakpointNeeded(cpuRegs.pc))
		intBreakpoint(false);

	intCheckMemcheck();

	// interprete instruction
	cpuRegs.code = memRead32( cpuRegs.pc );
	if( IsDebugBuild )
		debugI();

	const OPCODE& opcode = GetCurrentInstruction();
	//use this to find out what opcodes your game uses. very slow! (rama)
	//runs++;
	//if (runs > 1599999999){ //leave some time to startup the testgame
	//	if (opcode.Name[0] == 'L') { //find all opcodes beginning with "L"
	//		Console.WriteLn ("Load %s", opcode.Name);
	//	}
	//}

	// Another method of instruction dumping:
	/*if( cpuRegs.cycle > 0x4f24d714 )
	{
		//CPU_LOG( "%s", disR5900Current.getCString());
		disOut.clear();
		opcode.disasm( disOut );
		disOut += '\n';
		CPU_LOG( disOut.c_str() );
	}*/


	cpuBlockCycles += opcode.cycles;
	cpuRegs.pc += 4;

	opcode.interpret();
}

static __fi void _doBranch_shared(u32 tar)
{
	branch2 = cpuRegs.branch = 1;
	execI();

	// branch being 0 means an exception was thrown, since only the exception
	// handler should ever clear it.

	if( cpuRegs.branch != 0 )
	{
		cpuRegs.pc = tar;
		cpuRegs.branch = 0;
	}
}

static void __fastcall doBranch( u32 target )
{
	_doBranch_shared( target );
	cpuRegs.cycle += cpuBlockCycles >> 3;
	cpuBlockCycles &= (1<<3)-1;
	intEventTest();
}

void __fastcall intDoBranch(u32 target)
{
	//Console.WriteLn("Interpreter Branch ");
	_doBranch_shared( target );

	if( Cpu == &intCpu )
	{
		cpuRegs.cycle += cpuBlockCycles >> 3;
		cpuBlockCycles &= (1<<3)-1;
		intEventTest();
	}
}

void intSetBranch()
{
	branch2 = /*cpuRegs.branch =*/ 1;
}

////////////////////////////////////////////////////////////////////
// R5900 Branching Instructions!
// These are the interpreter versions of the branch instructions.  Unlike other
// types of interpreter instructions which can be called safely from the recompilers,
// these instructions are not "recSafe" because they may not invoke the
// necessary branch test logic that the recs need to maintain sync with the
// cpuRegs.pc and delaySlot instruction and such.

namespace R5900 {
namespace Interpreter {
namespace OpcodeImpl {

/*********************************************************
* Jump to target                                         *
* Format:  OP target                                     *
*********************************************************/
// fixme: looking at the other branching code, shouldn't those _SetLinks in BGEZAL and such only be set
// if the condition is true? --arcum42

void J()
{
	doBranch(_JumpTarget_);
}

void JAL()
{
	_SetLink(31);
	doBranch(_JumpTarget_);
}

/*********************************************************
* Register branch logic                                  *
* Format:  OP rs, rt, offset                             *
*********************************************************/

void BEQ()  // Branch if Rs == Rt
{
	if (cpuRegs.GPR.r[_Rs_].SD[0] == cpuRegs.GPR.r[_Rt_].SD[0])
		doBranch(_BranchTarget_);
	else
		intEventTest();
}

void BNE()  // Branch if Rs != Rt
{
	if (cpuRegs.GPR.r[_Rs_].SD[0] != cpuRegs.GPR.r[_Rt_].SD[0])
		doBranch(_BranchTarget_);
	else
		intEventTest();
}

/*********************************************************
* Register branch logic                                  *
* Format:  OP rs, offset                                 *
*********************************************************/

void BGEZ()    // Branch if Rs >= 0
{
	if(cpuRegs.GPR.r[_Rs_].SD[0] >= 0)
	{
		doBranch(_BranchTarget_);
	}
}

void BGEZAL() // Branch if Rs >= 0 and link
{

	if (cpuRegs.GPR.r[_Rs_].SD[0] >= 0)
	{
		_SetLink(31);
		doBranch(_BranchTarget_);
	}
}

void BGTZ()    // Branch if Rs >  0
{
	if (cpuRegs.GPR.r[_Rs_].SD[0] > 0)
	{
		doBranch(_BranchTarget_);
	}
}

void BLEZ()   // Branch if Rs <= 0
{
	if (cpuRegs.GPR.r[_Rs_].SD[0] <= 0)
	{
		doBranch(_BranchTarget_);
	}
}

void BLTZ()    // Branch if Rs <  0
{
	if (cpuRegs.GPR.r[_Rs_].SD[0] < 0)
	{
		doBranch(_BranchTarget_);
	}
}

void BLTZAL()  // Branch if Rs <  0 and link
{
	if (cpuRegs.GPR.r[_Rs_].SD[0] < 0)
	{
		_SetLink(31);
		doBranch(_BranchTarget_);
	}
}

/*********************************************************
* Register branch logic  Likely                          *
* Format:  OP rs, offset                                 *
*********************************************************/


void BEQL()    // Branch if Rs == Rt
{
	if(cpuRegs.GPR.r[_Rs_].SD[0] == cpuRegs.GPR.r[_Rt_].SD[0])
	{
		doBranch(_BranchTarget_);
	}
	else
	{
		cpuRegs.pc +=4;
		intEventTest();
	}
}

void BNEL()     // Branch if Rs != Rt
{
	if(cpuRegs.GPR.r[_Rs_].SD[0] != cpuRegs.GPR.r[_Rt_].SD[0])
	{
		doBranch(_BranchTarget_);
	}
	else
	{
		cpuRegs.pc +=4;
		intEventTest();
	}
}

void BLEZL()    // Branch if Rs <= 0
{
	if(cpuRegs.GPR.r[_Rs_].SD[0] <= 0)
	{
		doBranch(_BranchTarget_);
	}
	else
	{
		cpuRegs.pc +=4;
		intEventTest();
	}
}

void BGTZL()     // Branch if Rs >  0
{
	if(cpuRegs.GPR.r[_Rs_].SD[0] > 0)
	{
		doBranch(_BranchTarget_);
	}
	else
	{
		cpuRegs.pc +=4;
		intEventTest();
	}
}

void BLTZL()     // Branch if Rs <  0
{
	if(cpuRegs.GPR.r[_Rs_].SD[0] < 0)
	{
		doBranch(_BranchTarget_);
	}
	else
	{
		cpuRegs.pc +=4;
		intEventTest();
	}
}

void BGEZL()     // Branch if Rs >= 0
{
	if(cpuRegs.GPR.r[_Rs_].SD[0] >= 0)
	{
		doBranch(_BranchTarget_);
	}
	else
	{
		cpuRegs.pc +=4;
		intEventTest();
	}
}

void BLTZALL()   // Branch if Rs <  0 and link
{

	if(cpuRegs.GPR.r[_Rs_].SD[0] < 0)
	{
		_SetLink(31);
		doBranch(_BranchTarget_);
	}
	else
	{
		cpuRegs.pc +=4;
		intEventTest();
	}
}

void BGEZALL()   // Branch if Rs >= 0 and link
{

	if(cpuRegs.GPR.r[_Rs_].SD[0] >= 0)
	{
		_SetLink(31);
		doBranch(_BranchTarget_);
	}
	else
	{
		cpuRegs.pc +=4;
		intEventTest();
	}
}

/*********************************************************
* Register jump                                          *
* Format:  OP rs, rd                                     *
*********************************************************/
void JR()
{
	// 0x33ad48 is the return address of the function that populate the TLB cache
	if (cpuRegs.GPR.r[_Rs_].UL[0] == 0x33ad48 && EmuConfig.Gamefixes.GoemonTlbHack) {
		GoemonPreloadTlb();
	}
	doBranch(cpuRegs.GPR.r[_Rs_].UL[0]);
}

void JALR()
{
	u32 temp = cpuRegs.GPR.r[_Rs_].UL[0];

	if (_Rd_)  _SetLink(_Rd_);

	doBranch(temp);
}

} } }		// end namespace R5900::Interpreter::OpcodeImpl


// --------------------------------------------------------------------------------------
//  R5900cpu/intCpu interface (implementations)
// --------------------------------------------------------------------------------------

static void intReserve()
{
	// fixme : detect cpu for use the optimize asm code
}

static void intAlloc()
{
	// Nothing to do!
}

static void intReset()
{
	cpuRegs.branch = 0;
	branch2 = 0;
}

static void intEventTest()
{
	// Perform counters, ints, and IOP updates:
	_cpuEventTest_Shared();
}

static void intExecute()
{
	try {
		if (g_SkipBiosHack) {
			do
				execI();
			while (cpuRegs.pc != EELOAD_START);
			eeloadReplaceOSDSYS();
		}
		if (ElfEntry != 0xFFFFFFFF) {
			do
				execI();
			while (cpuRegs.pc != ElfEntry);
			eeGameStarting();
		} else {
			while (true)
				execI();
		}
	} catch( Exception::ExitCpuExecute& ) { }
}

static void intCheckExecutionState()
{
	if( GetCoreThread().HasPendingStateChangeRequest() )
		throw Exception::ExitCpuExecute();
}

static void intStep()
{
	execI();
}

static void intClear(u32 Addr, u32 Size)
{
}

static void intShutdown() {
}

static void intThrowException( const BaseR5900Exception& ex )
{
	// No tricks needed; C++ stack unwnding should suffice for MSW and GCC alike.
	ex.Rethrow();
}

static void intThrowException( const BaseException& ex )
{
	// No tricks needed; C++ stack unwnding should suffice for MSW and GCC alike.
	ex.Rethrow();
}

static void intSetCacheReserve( uint reserveInMegs )
{
}

static uint intGetCacheReserve()
{
	return 0;
}

R5900cpu intCpu =
{
	intReserve,
	intShutdown,

	intReset,
	intStep,
	intExecute,

	intCheckExecutionState,
	intThrowException,
	intThrowException,
	intClear,

	intGetCacheReserve,
	intSetCacheReserve,
};
