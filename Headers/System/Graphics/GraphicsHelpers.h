#ifndef _SYSTEM_GRAPHICS_H
#define _SYSTEM_GRAPHICS_H

#if (defined(__x86_64__) || defined(_M_X64)) && defined(_WIN32)
#define _SYSTEM_OPENGL
#include <windows.h>
#include <GL/gl.h>
#include <cstdint>
#endif
namespace System::Graphics{
#ifdef _SYSTEM_OPENGL
	class GL{
		enum GL_BitField :uint32_t { 
			COLOR_BUFFER_BIT = 0x00004000,
			DEPTH_BUFFER_BIT = 0x00000100,
			STENCIL_BUFFER_BIT = 0x00000400
		};
		enum GL_DrawMode : uint32_t {
			POINTS = 0x0000,
			LINE_STRIP = 0x0003,
			LINE_LOOP = 0x0002,
			LINES = 0x0001,
			LINE_STRIP_ADJACENCY = 0x000B,  // Correct OpenGL value
			LINES_ADJACENCY = 0x000A,       // Correct OpenGL value
			TRIANGLE_STRIP = 0x0005,
			TRIANGLE_FAN = 0x0006,
			TRIANGLES = 0x0004,
			TRIANGLE_STRIP_ADJACENCY = 0x000D, // Correct OpenGL value
			TRIANGLES_ADJACENCY = 0x000C,      // Correct OpenGL value
			PATCHES = 0x000E  // OpenGL tessellation patch primitive
		};
	public:
		/// <summary>
		/// clear buffers to preset values
		/// </summary>
		/// <param name="BitField">Bitwise OR of masks that indicate the buffers to be cleared. The three masks are GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, and GL_STENCIL_BUFFER_BIT.</param>
		inline static void glClear(GL_BitField BitField) {
			::glClear(BitField);
		}
		/// <summary>
		/// specify clear values for the color buffers
		/// </summary>
		inline static void glClearColor(float r, float g, float b, float a){
			::glClearColor(r, g, b, a);
		}
		/// <summary>
		/// render primitives from array data
		/// </summary>
		/// <param name="mode">Specifies what kind of primitives to render. Symbolic constants GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY and GL_PATCHES are accepted.</param>
		/// <param name="first">Specifies the starting index in the enabled arrays.</param>
		/// <param name="count">Specifies the number of indices to be rendered.</param>
		inline static void glDrawArrays(GL_DrawMode mode,GLint first,GLsizei count) {
			::glDrawArrays(mode, first, count);
		}





 

	}; 

#endif

 

}
#endif