/* *	Copyright (C) 2011-2014 Gregory hainaut
 *	Copyright (C) 2007-2009 Gabest
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GNU Make; see the file COPYING.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include "stdafx.h"
#include "GLLoader.h"
#include "GSdx.h"

#ifndef ENABLE_GLES
PFNGLACTIVETEXTUREPROC                 gl_ActiveTexture                     = NULL;
PFNGLBLENDCOLORPROC                    gl_BlendColor                        = NULL;
PFNGLATTACHSHADERPROC                  gl_AttachShader                      = NULL;
PFNGLBINDBUFFERPROC                    gl_BindBuffer                        = NULL;
PFNGLBINDBUFFERBASEPROC                gl_BindBufferBase                    = NULL;
PFNGLBINDFRAMEBUFFERPROC               gl_BindFramebuffer                   = NULL;
PFNGLBINDSAMPLERPROC                   gl_BindSampler                       = NULL;
PFNGLBINDVERTEXARRAYPROC               gl_BindVertexArray                   = NULL;
PFNGLBLENDEQUATIONSEPARATEIARBPROC     gl_BlendEquationSeparateiARB         = NULL;
PFNGLBLENDFUNCSEPARATEIARBPROC         gl_BlendFuncSeparateiARB             = NULL;
PFNGLBLITFRAMEBUFFERPROC               gl_BlitFramebuffer                   = NULL;
PFNGLBUFFERDATAPROC                    gl_BufferData                        = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC        gl_CheckFramebufferStatus            = NULL;
PFNGLCLEARBUFFERFVPROC                 gl_ClearBufferfv                     = NULL;
PFNGLCLEARBUFFERIVPROC                 gl_ClearBufferiv                     = NULL;
PFNGLCLEARBUFFERUIVPROC                gl_ClearBufferuiv                    = NULL;
PFNGLCOLORMASKIPROC                    gl_ColorMaski                        = NULL;
PFNGLCOMPILESHADERPROC                 gl_CompileShader                     = NULL;
PFNGLCREATEPROGRAMPROC                 gl_CreateProgram                     = NULL;
PFNGLCREATESHADERPROC                  gl_CreateShader                      = NULL;
PFNGLCREATESHADERPROGRAMVPROC          gl_CreateShaderProgramv              = NULL;
PFNGLDELETEBUFFERSPROC                 gl_DeleteBuffers                     = NULL;
PFNGLDELETEFRAMEBUFFERSPROC            gl_DeleteFramebuffers                = NULL;
PFNGLDELETEPROGRAMPROC                 gl_DeleteProgram                     = NULL;
PFNGLDELETESAMPLERSPROC                gl_DeleteSamplers                    = NULL;
PFNGLDELETESHADERPROC                  gl_DeleteShader                      = NULL;
PFNGLDELETEVERTEXARRAYSPROC            gl_DeleteVertexArrays                = NULL;
PFNGLDETACHSHADERPROC                  gl_DetachShader                      = NULL;
PFNGLDRAWBUFFERSPROC                   gl_DrawBuffers                       = NULL;
PFNGLDRAWELEMENTSBASEVERTEXPROC        gl_DrawElementsBaseVertex            = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC       gl_EnableVertexAttribArray           = NULL;
PFNGLFRAMEBUFFERRENDERBUFFERPROC       gl_FramebufferRenderbuffer           = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC          gl_FramebufferTexture2D              = NULL;
PFNGLGENBUFFERSPROC                    gl_GenBuffers                        = NULL;
PFNGLGENFRAMEBUFFERSPROC               gl_GenFramebuffers                   = NULL;
PFNGLGENSAMPLERSPROC                   gl_GenSamplers                       = NULL;
PFNGLGENVERTEXARRAYSPROC               gl_GenVertexArrays                   = NULL;
PFNGLGETBUFFERPARAMETERIVPROC          gl_GetBufferParameteriv              = NULL;
PFNGLGETDEBUGMESSAGELOGARBPROC         gl_GetDebugMessageLogARB             = NULL;
PFNGLDEBUGMESSAGECALLBACKPROC          gl_DebugMessageCallback              = NULL;
PFNGLGETPROGRAMINFOLOGPROC             gl_GetProgramInfoLog                 = NULL;
PFNGLGETPROGRAMIVPROC                  gl_GetProgramiv                      = NULL;
PFNGLGETSHADERIVPROC                   gl_GetShaderiv                       = NULL;
PFNGLGETSTRINGIPROC                    gl_GetStringi                        = NULL;
PFNGLISFRAMEBUFFERPROC                 gl_IsFramebuffer                     = NULL;
PFNGLLINKPROGRAMPROC                   gl_LinkProgram                       = NULL;
PFNGLMAPBUFFERPROC                     gl_MapBuffer                         = NULL;
PFNGLMAPBUFFERRANGEPROC                gl_MapBufferRange                    = NULL;
PFNGLPROGRAMPARAMETERIPROC             gl_ProgramParameteri                 = NULL;
PFNGLSAMPLERPARAMETERFPROC             gl_SamplerParameterf                 = NULL;
PFNGLSAMPLERPARAMETERIPROC             gl_SamplerParameteri                 = NULL;
PFNGLSHADERSOURCEPROC                  gl_ShaderSource                      = NULL;
PFNGLUNIFORM1IPROC                     gl_Uniform1i                         = NULL;
PFNGLUNMAPBUFFERPROC                   gl_UnmapBuffer                       = NULL;
PFNGLUSEPROGRAMSTAGESPROC              gl_UseProgramStages                  = NULL;
PFNGLVERTEXATTRIBIPOINTERPROC          gl_VertexAttribIPointer              = NULL;
PFNGLVERTEXATTRIBPOINTERPROC           gl_VertexAttribPointer               = NULL;
PFNGLBUFFERSUBDATAPROC                 gl_BufferSubData                     = NULL;
PFNGLFENCESYNCPROC                     gl_FenceSync                         = NULL;
PFNGLDELETESYNCPROC                    gl_DeleteSync                        = NULL;
PFNGLCLIENTWAITSYNCPROC                gl_ClientWaitSync                    = NULL;
PFNGLFLUSHMAPPEDBUFFERRANGEPROC        gl_FlushMappedBufferRange            = NULL;
// GL4.0
PFNGLUNIFORMSUBROUTINESUIVPROC         gl_UniformSubroutinesuiv             = NULL;
// GL4.1
PFNGLBINDPROGRAMPIPELINEPROC           gl_BindProgramPipeline               = NULL;
PFNGLGENPROGRAMPIPELINESPROC           gl_GenProgramPipelines               = NULL;
PFNGLDELETEPROGRAMPIPELINESPROC        gl_DeleteProgramPipelines            = NULL;
PFNGLGETPROGRAMPIPELINEIVPROC          gl_GetProgramPipelineiv              = NULL;
PFNGLVALIDATEPROGRAMPIPELINEPROC       gl_ValidateProgramPipeline           = NULL;
PFNGLGETPROGRAMPIPELINEINFOLOGPROC     gl_GetProgramPipelineInfoLog         = NULL;
// NO GL4.1
PFNGLUSEPROGRAMPROC                    gl_UseProgram                        = NULL;
PFNGLGETSHADERINFOLOGPROC              gl_GetShaderInfoLog                  = NULL;
PFNGLPROGRAMUNIFORM1IPROC              gl_ProgramUniform1i                  = NULL;
// GL4.3
PFNGLCOPYIMAGESUBDATAPROC              gl_CopyImageSubData                  = NULL;
// GL4.2
PFNGLBINDIMAGETEXTUREPROC              gl_BindImageTexture                  = NULL;
PFNGLMEMORYBARRIERPROC                 gl_MemoryBarrier                     = NULL;
PFNGLTEXSTORAGE2DPROC                  gl_TexStorage2D                      = NULL;
// GL4.4
PFNGLCLEARTEXIMAGEPROC                 gl_ClearTexImage                     = NULL;
PFNGLBUFFERSTORAGEPROC                 gl_BufferStorage                     = NULL;
// GL_ARB_bindless_texture (GL5?)
PFNGLGETTEXTURESAMPLERHANDLEARBPROC    gl_GetTextureSamplerHandleARB        = NULL;
PFNGLMAKETEXTUREHANDLERESIDENTARBPROC  gl_MakeTextureHandleResidentARB      = NULL;
PFNGLMAKETEXTUREHANDLENONRESIDENTARBPROC gl_MakeTextureHandleNonResidentARB = NULL;
PFNGLUNIFORMHANDLEUI64VARBPROC         gl_UniformHandleui64vARB             = NULL;
PFNGLPROGRAMUNIFORMHANDLEUI64VARBPROC  gl_ProgramUniformHandleui64vARB      = NULL;

// GL4.5
PFNGLCREATETEXTURESPROC				   gl_CreateTextures                    = NULL;
PFNGLTEXTURESTORAGE2DPROC			   gl_TextureStorage2D                  = NULL;
PFNGLTEXTURESUBIMAGE2DPROC			   gl_TextureSubImage2D                 = NULL;
PFNGLCOPYTEXTURESUBIMAGE2DPROC		   gl_CopyTextureSubImage2D             = NULL;
PFNGLBINDTEXTUREUNITPROC			   gl_BindTextureUnit                   = NULL;
PFNGLGETTEXTUREIMAGEPROC               gl_GetTextureImage                   = NULL;

PFNGLCREATEFRAMEBUFFERSPROC            gl_CreateFramebuffers                = NULL;
PFNGLCLEARNAMEDFRAMEBUFFERFVPROC       gl_ClearNamedFramebufferfv           = NULL;
PFNGLCLEARNAMEDFRAMEBUFFERIVPROC       gl_ClearNamedFramebufferiv           = NULL;
PFNGLCLEARNAMEDFRAMEBUFFERUIVPROC      gl_ClearNamedFramebufferuiv          = NULL;
PFNGLNAMEDFRAMEBUFFERTEXTUREPROC       gl_NamedFramebufferTexture           = NULL;
PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC   gl_NamedFramebufferDrawBuffers       = NULL;
PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC    gl_NamedFramebufferReadBuffer        = NULL;
PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC   gl_CheckNamedFramebufferStatus       = NULL;

PFNGLCREATEBUFFERSPROC                 gl_CreateBuffers                     = NULL;
PFNGLNAMEDBUFFERSTORAGEPROC            gl_NamedBufferStorage                = NULL;
PFNGLNAMEDBUFFERDATAPROC               gl_NamedBufferData                   = NULL;
PFNGLNAMEDBUFFERSUBDATAPROC            gl_NamedBufferSubData                = NULL;
PFNGLMAPNAMEDBUFFERPROC                gl_MapNamedBuffer                    = NULL;
PFNGLMAPNAMEDBUFFERRANGEPROC           gl_MapNamedBufferRange               = NULL;
PFNGLUNMAPNAMEDBUFFERPROC              gl_UnmapNamedBuffer                  = NULL;

PFNGLCREATESAMPLERSPROC                gl_CreateSamplers                    = NULL;
PFNGLCREATEPROGRAMPIPELINESPROC        gl_CreateProgramPipelines            = NULL;

PFNGLCLIPCONTROLPROC                   gl_ClipControl                       = NULL;

#endif

namespace GLLoader {

	bool fglrx_buggy_driver    = false;
	bool mesa_amd_buggy_driver = false;
	bool nvidia_buggy_driver   = false;
	bool intel_buggy_driver    = false;
	bool in_replayer           = false;

	// Optional
	bool found_GL_ARB_separate_shader_objects = false; // Issue with Mesa and Catalyst...
	bool found_geometry_shader = true; // we require GL3.3 so geometry must be supported by default
	bool found_GL_ARB_clear_texture = false; // Don't know if GL3 GPU can support it
	// Note: except Apple, all drivers support explicit uniform location
	bool found_GL_ARB_explicit_uniform_location = false; // need by subroutine and bindless texture
	// GL4 hardware
	bool found_GL_ARB_buffer_storage = false;
	bool found_GL_ARB_copy_image = false; // Not sure actually maybe GL3 GPU can do it
	bool found_GL_ARB_gpu_shader5 = false;
	bool found_GL_ARB_shader_image_load_store = false; // GLES3.1
	bool found_GL_ARB_shader_subroutine = false;
	bool found_GL_ARB_bindless_texture = false; // GL5 GPU?

	// GL4.5 for the future (dx10/dx11 compatibility)
	bool found_GL_ARB_clip_control = false;
	bool found_GL_ARB_direct_state_access = false;

	// Mandatory for opengl ES (allow to use GL code)
	bool found_GL_EXT_shader_io_blocks = false;

	// Mandatory
	bool found_GL_ARB_texture_storage = false;
	bool found_GL_ARB_shading_language_420pack = false;

	static bool status_and_override(bool& found, const std::string& name, bool mandatory = false)
	{
		if (!found) {
			if(mandatory) {
				fprintf(stderr, "ERROR: %s is NOT SUPPORTED\n", name.c_str());
				return false;
			} else {
				fprintf(stderr, "INFO: %s is NOT SUPPORTED\n", name.c_str());
			}
		} else {
			fprintf(stderr, "INFO: %s is available\n", name.c_str());
		}

		std::string opt("override_");
		opt += name;

		if (theApp.GetConfig(opt.c_str(), -1) != -1) {
			found = !!theApp.GetConfig(opt.c_str(), -1);
			fprintf(stderr, "Override %s detection (%s)\n", name.c_str(), found ? "Enabled" : "Disabled");
		}

		return true;
	}

    bool check_gl_version(uint32 major, uint32 minor) {

		const GLubyte* s = glGetString(GL_VERSION);
		if (s == NULL) {
			fprintf(stderr, "Error: GLLoader failed to get GL version\n");
			return false;
		}

		const char* vendor = (const char*)glGetString(GL_VENDOR);
		fprintf(stderr, "Supported Opengl version: %s on GPU: %s. Vendor: %s\n", s, glGetString(GL_RENDERER), vendor);
#ifndef ENABLE_GLES
		fprintf(stderr, "Note: the maximal version supported by GSdx is 3.3 (even if you driver support more)!\n");
#endif

		// Name change but driver is still bad!
		if (strstr(vendor, "ATI") || strstr(vendor, "Advanced Micro Devices"))
			fglrx_buggy_driver = true;
		if (strstr(vendor, "NVIDIA Corporation"))
			nvidia_buggy_driver = true;
		if (strstr(vendor, "Intel"))
			intel_buggy_driver = true;
		if (strstr(vendor, "X.Org")) // Note: it might actually catch nouveau too, but bug are likely to be the same anyway
			mesa_amd_buggy_driver = true;

		GLuint dot = 0;
		while (s[dot] != '\0' && s[dot] != '.') dot++;
		if (dot == 0) return false;

		GLuint major_gl = s[dot-1]-'0';
		GLuint minor_gl = s[dot+1]-'0';

#ifndef ENABLE_GLES
		if (mesa_amd_buggy_driver || intel_buggy_driver) {
			fprintf(stderr, "Buggy driver detected. Geometry shaders will be disabled\n");
			found_geometry_shader = false;
		}
		if (theApp.GetConfig("override_geometry_shader", -1) != -1) {
			found_geometry_shader = !!theApp.GetConfig("override_geometry_shader", -1);
			fprintf(stderr, "Override geometry shaders detection\n");
		}
#else
		found_geometry_shader = false;
#endif
#ifndef ENABLE_GLES
		if ( (major_gl < major) || ( major_gl == major && minor_gl < minor ) ) {
			fprintf(stderr, "OPENGL %d.%d is not supported\n", major, minor);
			return false;
		}
#endif

        return true;
    }

	bool check_gl_supported_extension() {
		int max_ext = 0;
		glGetIntegerv(GL_NUM_EXTENSIONS, &max_ext);

		if (gl_GetStringi && max_ext) {
			for (GLint i = 0; i < max_ext; i++) {
				string ext((const char*)gl_GetStringi(GL_EXTENSIONS, i));
#ifndef ENABLE_GLES
				// GL4.0
				if (ext.compare("GL_ARB_gpu_shader5") == 0) found_GL_ARB_gpu_shader5 = true;
				// GL4.1
				if (ext.compare("GL_ARB_separate_shader_objects") == 0) {
					if (!fglrx_buggy_driver && !mesa_amd_buggy_driver && !intel_buggy_driver) found_GL_ARB_separate_shader_objects = true;
					else fprintf(stderr, "Buggy driver detected, GL_ARB_separate_shader_objects will be disabled\n");
				}
#if 0
				// Erratum: on nvidia implementation, gain is very nice : 42.5 fps => 46.5 fps
				//
				// Strangely it doesn't provide the speed boost as expected.
				// Note: only atst/colclip was replaced with subroutine for the moment. It replace 2000 program switch on
				// colin mcrae 3 by 2100 uniform, but code is slower!
				//
				// Current hypothesis: the validation of useprogram is done in the "driver thread" whereas the extra function calls
				// are done on the overloaded main threads.
				// Apitrace profiling shows faster GPU draw times

				if (ext.compare("GL_ARB_shader_subroutine") == 0) found_GL_ARB_shader_subroutine = true;
#endif
				// GL4.2
				if (ext.compare("GL_ARB_shading_language_420pack") == 0) found_GL_ARB_shading_language_420pack = true;
				if (ext.compare("GL_ARB_texture_storage") == 0) found_GL_ARB_texture_storage = true;
				// Only enable this extension on nvidia
				if (nvidia_buggy_driver && ext.compare("GL_ARB_shader_image_load_store") == 0) found_GL_ARB_shader_image_load_store = true;
				// GL4.3
				if (ext.compare("GL_ARB_copy_image") == 0) found_GL_ARB_copy_image = true;
				if (ext.compare("GL_ARB_explicit_uniform_location") == 0) found_GL_ARB_explicit_uniform_location = true;
				// GL4.4
				if (ext.compare("GL_ARB_buffer_storage") == 0) found_GL_ARB_buffer_storage = true;
				if (ext.compare("GL_ARB_clear_texture") == 0) found_GL_ARB_clear_texture = true;
				// FIXME: I have a crash when I hit pause (debug build)
				//if (ext.compare("GL_ARB_bindless_texture") == 0) found_GL_ARB_bindless_texture = true;
				// GL4.5
				if (ext.compare("GL_ARB_direct_state_access") == 0) found_GL_ARB_direct_state_access = true;
				if (ext.compare("GL_ARB_clip_control") == 0) found_GL_ARB_clip_control = true;
#else // ENABLE_GLES
				if (ext.compare("GL_EXT_shader_io_blocks") == 0) found_GL_EXT_shader_io_blocks = true;
#endif

				//fprintf(stderr, "DEBUG ext: %s\n", ext.c_str());
			}
		}

		bool status = true;
		fprintf(stderr, "\n");

#ifndef ENABLE_GLES
		// GL4.0
		status &= status_and_override(found_GL_ARB_gpu_shader5,"GL_ARB_gpu_shader5");
		// GL4.1
		status &= status_and_override(found_GL_ARB_separate_shader_objects,"GL_ARB_separate_shader_objects");
		status &= status_and_override(found_GL_ARB_shader_subroutine,"GL_ARB_shader_subroutine");
		// GL4.2
		status &= status_and_override(found_GL_ARB_shader_image_load_store,"GL_ARB_shader_image_load_store");
		status &= status_and_override(found_GL_ARB_shading_language_420pack,"GL_ARB_shading_language_420pack", true);
		status &= status_and_override(found_GL_ARB_texture_storage, "GL_ARB_texture_storage", true);
		// GL4.3
		status &= status_and_override(found_GL_ARB_explicit_uniform_location,"GL_ARB_explicit_uniform_location");
		status &= status_and_override(found_GL_ARB_copy_image, "GL_ARB_copy_image");
		// GL4.4
		status &= status_and_override(found_GL_ARB_buffer_storage,"GL_ARB_buffer_storage");
		status &= status_and_override(found_GL_ARB_bindless_texture,"GL_ARB_bindless_texture");
		status &= status_and_override(found_GL_ARB_clear_texture,"GL_ARB_clear_texture");
		// GL4.5
		status &= status_and_override(found_GL_ARB_clip_control, "GL_ARB_clip_control");
		status &= status_and_override(found_GL_ARB_direct_state_access, "GL_ARB_direct_state_access");
#else // ENABLE_GLES
		status &= status_and_override(found_GL_EXT_shader_io_blocks, "GL_EXT_shader_io_blocks", true);
#endif

		fprintf(stderr, "\n");

		return status;
	}
}
