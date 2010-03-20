/*  ZeroGS KOSMOS
 *  Copyright (C) 2005-2006 zerofrog@gmail.com
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __ZEROGS_TARGETS_H__
#define __ZEROGS_TARGETS_H__

#define TARGET_VIRTUAL_KEY 0x80000000
#include "PS2Edefs.h"

inline Vector DefaultOneColor( FRAGMENTSHADER ptr ) {
	Vector v = Vector ( 1, 1, 1, 1 );
	cgGLSetParameter4fv( ptr.sOneColor, v);
	return v ;
}

namespace ZeroGS {

	inline u32 GetFrameKey (int fbp, int fbw, VB& curvb);

	// manages render targets
	class CRenderTargetMngr
	{
	public:
		typedef map<u32, CRenderTarget*> MAPTARGETS;
		
		enum TargetOptions {
			TO_DepthBuffer = 1,
			TO_StrictHeight = 2, // height returned has to be the same as requested
			TO_Virtual = 4
		};
		
		~CRenderTargetMngr() { Destroy(); }
		
		void Destroy();
		static MAPTARGETS::iterator GetOldestTarg(MAPTARGETS& m);
		
		CRenderTarget* GetTarg(const frameInfo& frame, u32 Options, int maxposheight);
		inline CRenderTarget* GetTarg(int fbp, int fbw, VB& curvb) {
			MAPTARGETS::iterator it = mapTargets.find (GetFrameKey(fbp, fbw, curvb));

/*			if (fbp == 0x3600 && fbw == 0x100 && it == mapTargets.end()) 
			{
				printf("%x\n", GetFrameKey(fbp, fbw, curvb)) ;
				printf("%x %x\n", fbp, fbw);
				for(MAPTARGETS::iterator it1 = mapTargets.begin(); it1 != mapTargets.end(); ++it1)
					printf ("\t %x %x %x %x\n", it1->second->fbw, it1->second->fbh, it1->second->psm, it1->second->fbp);
			}*/
			return it != mapTargets.end() ? it->second : NULL;
		}
		
		// gets all targets with a range
		void GetTargs(int start, int end, list<ZeroGS::CRenderTarget*>& listTargets) const;
		
		// resolves all targets within a range
		__forceinline void Resolve(int start, int end);
		__forceinline void ResolveAll() {
			for(MAPTARGETS::iterator it = mapTargets.begin(); it != mapTargets.end(); ++it )
				it->second->Resolve();
		}
		
		void DestroyAllTargs(int start, int end, int fbw);
		void DestroyIntersecting(CRenderTarget* prndr);
	
		// promotes a target from virtual to real
		inline CRenderTarget* Promote(u32 key) {
			assert( !(key & TARGET_VIRTUAL_KEY) );
		
			// promote to regular targ
			CRenderTargetMngr::MAPTARGETS::iterator it = mapTargets.find(key|TARGET_VIRTUAL_KEY);
			assert( it != mapTargets.end() );
			
			CRenderTarget* ptarg = it->second;
			mapTargets.erase(it);
			
			DestroyIntersecting(ptarg);
			
			it = mapTargets.find(key);
			if( it != mapTargets.end() ) {
				DestroyTarg(it->second);
				it->second = ptarg;
			}
			else
				mapTargets[key] = ptarg;
			
		        if( g_GameSettings & GAME_RESOLVEPROMOTED )
                		ptarg->status = CRenderTarget::TS_Resolved;
				else
			ptarg->status = CRenderTarget::TS_NeedUpdate;
				return ptarg;
		}
		
		static void DestroyTarg(CRenderTarget* ptarg);
		
		MAPTARGETS mapTargets, mapDummyTargs;
	};

	class CMemoryTargetMngr
	{
	public:
		CMemoryTargetMngr() : curstamp(0) {}
		CMemoryTarget* GetMemoryTarget(const tex0Info& tex0, int forcevalidate); // pcbp is pointer to start of clut
		CMemoryTarget* MemoryTarget_SearchExistTarget (int start, int end, int nClutOffset, int clutsize, const tex0Info& tex0, int forcevalidate);
		CMemoryTarget* MemoryTarget_ClearedTargetsSearch(int fmt, int widthmult, int channels, int height);

		void Destroy(); // destroy all targs

		void ClearRange(int starty, int endy); // set all targets to cleared
		void DestroyCleared(); // flush all cleared targes
		void DestroyOldest();

		list<CMemoryTarget> listTargets, listClearedTargets;
		u32 curstamp;

	private:
		list<CMemoryTarget>::iterator DestroyTargetIter(list<CMemoryTarget>::iterator& it);
	};

	class CBitwiseTextureMngr
	{
	public:
		~CBitwiseTextureMngr() { Destroy(); }

		void Destroy();

		// since GetTex can delete textures to free up mem, it is dangerous if using that texture, so specify at least one other tex to save
		__forceinline u32 GetTex(u32 bitvalue, u32 ptexDoNotDelete) {
			map<u32, u32>::iterator it = mapTextures.find(bitvalue);
			if( it != mapTextures.end() )
				return it->second;
			return GetTexInt(bitvalue, ptexDoNotDelete);
		}

	private:
		u32 GetTexInt(u32 bitvalue, u32 ptexDoNotDelete);

		map<u32, u32> mapTextures;
	};

	// manages
	class CRangeManager
	{
	public:
		CRangeManager() {
			ranges.reserve(16);
		}

		// [start, end)
		struct RANGE {
			RANGE() {}
			inline RANGE(int start, int end) : start(start), end(end) {}
			int start, end;
		};

		// works in semi logN
		void Insert(int start, int end);
		inline void Clear() { 
			ranges.resize(0); 
		}
		
		vector<RANGE> ranges; // organized in ascending order, non-intersecting
	};

	extern CRenderTargetMngr s_RTs, s_DepthRTs;
	extern CBitwiseTextureMngr s_BitwiseTextures;
	extern CMemoryTargetMngr g_MemTargs;

	extern u8 s_AAx, s_AAy, s_AAz, s_AAw;

	// Real rendered width, depends on AA and AAneg.
	inline int RW(int tbw) {
		if (s_AAx >= s_AAz)
			return (tbw << ( s_AAx - s_AAz ));
		else
			return (tbw >> ( s_AAz - s_AAx ));
	}
	
	// Real rendered height, depends on AA and AAneg.
	inline int RH(int tbh) {
		if (s_AAy >= s_AAw)
			return (tbh << ( s_AAy - s_AAw ));
		else
			return (tbh >> ( s_AAw - s_AAy ));
	}

/*	inline void CreateTargetsList(int start, int end, list<ZeroGS::CRenderTarget*>& listTargs) {
		s_DepthRTs.GetTargs(start, end, listTargs);
		s_RTs.GetTargs(start, end, listTargs);
	}*/

	// This pattern of functions is called 3 times, so I add creating Targets list into one.
	inline list<ZeroGS::CRenderTarget*> CreateTargetsList(int start, int end) {
		list<ZeroGS::CRenderTarget*> listTargs;
		s_DepthRTs.GetTargs(start, end, listTargs);
		s_RTs.GetTargs(start, end, listTargs);
		return listTargs;
	}

	extern Vector g_vdepth;
	extern int icurctx;

	extern VERTEXSHADER pvsBitBlt;
	extern FRAGMENTSHADER ppsBitBlt[2], ppsBitBltDepth, ppsOne;
	extern FRAGMENTSHADER ppsBaseTexture, ppsConvert16to32, ppsConvert32to16;
	extern GLuint vboRect;

// Unworking
#define PSMPOSITION 28

// Code width and height of frame into key, that used in targetmanager
// This is 3 variants of one function, Key dependant on fbp and fbw.
inline u32 GetFrameKey (const frameInfo& frame) {
	return (((frame.fbw) << 16) | (frame.fbp));
}
inline u32 GetFrameKey ( CRenderTarget* frame ) {
	return (((frame->fbw) << 16) | (frame->fbp));
}

inline u32 GetFrameKey (int fbp, int fbw,  VB& curvb) {
	return (((fbw) << 16) | (fbp));
}

inline u16 ShiftHeight (int fbh, int fbp, int fbhCalc) {
	return fbh;
}	

//FIXME: this code for P4 ad KH1. It should not be such strange!
//Dummy targets was deleted from mapTargets, but not erased.
inline u32 GetFrameKeyDummy (const frameInfo& frame) {
//	if (frame.fbp > 0x2000 && ZZOgl_fbh_Calc(frame) < 0x400 && ZZOgl_fbh_Calc(frame) != frame.fbh)
//		printf ("Z %x %x %x %x\n", frame.fbh, frame.fbhCalc, frame.fbp, ZZOgl_fbh_Calc(frame));
	// height over 1024 would shrink to 1024, so dummy targets with calculated size more than 0x400 should be 
	// distinct by real height. But in FFX there is 3e0 height target, so I put 0x300 as limit
	if (/*frame.fbp > 0x2000 && */ZZOgl_fbh_Calc(frame) < 0x300)
		return (((frame.fbw) << 16) | ZZOgl_fbh_Calc(frame));
	else
		return (((frame.fbw) << 16) | frame.fbh);
}

inline u32 GetFrameKeyDummy ( CRenderTarget* frame ) {
	if (/*frame->fbp > 0x2000 && */ZZOgl_fbh_Calc(frame->fbp, frame->fbw, frame->psm) < 0x300)
		return (((frame->fbw) << 16) | ZZOgl_fbh_Calc(frame->fbp, frame->fbw, frame->psm));
	else
		return (((frame->fbw) << 16) | frame->fbh);
}

} // End of namespace

#endif