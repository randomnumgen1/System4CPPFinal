//This header is a wrapper for the OpenGL functions in the file InternalGLloader.
#ifndef _SYSTEM_GRAPHICS_H
#define _SYSTEM_GRAPHICS_H

#include <System/Internal/InternalGLloader.h> 
#include <string>
#include <vector>
#include <System/Matrix4x4.hpp>


#if (defined(__x86_64__) || defined(_M_X64))// && defined(_WIN32)
#define _SYSTEM_OPENGL_WIN
#include <cstdint>
#elif defined(__arm__) || defined(_M_ARM)
#define _SYSTEM_OPENGLES_ARM
#endif





#ifndef __ENUM_FLAGS_H__
#define __ENUM_FLAGS_H__

/*
Use this line before header, if you don't want flags(T x) function to be implemented for your enum.
#define USE_ENUM_FLAGS_FUNCTION 0
*/

#ifndef USE_ENUM_FLAGS_FUNCTION
#define USE_ENUM_FLAGS_FUNCTION 1
#endif


#define ENUM_FLAGS_EX_NO_FLAGS_FUNC(T,INT_T) \
enum class T;	\
inline T	operator	&	(T x, T y)		{	return static_cast<T>	(static_cast<INT_T>(x) & static_cast<INT_T>(y));	}; \
inline T	operator	|	(T x, T y)		{	return static_cast<T>	(static_cast<INT_T>(x) | static_cast<INT_T>(y));	}; \
inline T	operator	^	(T x, T y)		{	return static_cast<T>	(static_cast<INT_T>(x) ^ static_cast<INT_T>(y));	}; \
inline T	operator	~	(T x)			{	return static_cast<T>	(~static_cast<INT_T>(x));							}; \
inline T&	operator	&=	(T& x, T y)		{	x = x & y;	return x;	}; \
inline T&	operator	|=	(T& x, T y)		{	x = x | y;	return x;	}; \
inline T&	operator	^=	(T& x, T y)		{	x = x ^ y;	return x;	};

#if(USE_ENUM_FLAGS_FUNCTION)

#define ENUM_FLAGS_EX(T,INT_T) ENUM_FLAGS_EX_NO_FLAGS_FUNC(T,INT_T) \
	inline bool			flags(T x)			{	return static_cast<INT_T>(x) != 0;};

#else

#define ENUM_FLAGS_EX(T,INT_T) ENUM_FLAGS_EX_NO_FLAGS_FUNC(T,INT_T) 

#endif

#define ENUM_FLAGS(T) ENUM_FLAGS_EX(T,uint32_t)
#endif
 











namespace System::Graphics{ 
	// Enum for depth comparison functions
	enum class DepthFunc : int32_t {
		Never = 0x0200, // GL_NEVER
		Less = 0x0201, // GL_LESS
		Equal = 0x0202, // GL_EQUAL
		Lequal = 0x0203, // GL_LEQUAL
		Greater = 0x0204, // GL_GREATER
		Notequal = 0x0205, // GL_NOTEQUAL
		Gequal = 0x0206, // GL_GEQUAL
		Always = 0x0207  // GL_ALWAYS
	};
	// Enum for common OpenGL capabilities using hex values
	enum class GraphicsCapability : uint32_t {
		CullFace = 0x0B44, // GL_CULL_FACE
		DepthTest = 0x0B71, // GL_DEPTH_TEST
		Blend = 0x0BE2, // GL_BLEND
		ScissorTest = 0x0C11, // GL_SCISSOR_TEST
		StencilTest = 0x0B90, // GL_STENCIL_TEST
		Multisample = 0x809D  // GL_MULTISAMPLE
	};

	enum class WindingOrder : int32_t {
		CW = 0x0900,
		CCW = 0x0901
	};
	enum class PixelFormat : int32_t {
		RED = 0x1903, // GL_RED
		RG = 0x8227, // GL_RG
		RGB = 0x1907, // GL_RGB
		RGBA = 0x1908, // GL_RGBA
		DEPTH_COMPONENT = 0x1902, // GL_DEPTH_COMPONENT
		STENCIL_INDEX = 0x1901  // GL_STENCIL_INDEX
	};

	enum class PixelType : int32_t {
		UNSIGNED_BYTE = 0x1401, // GL_UNSIGNED_BYTE
		UNSIGNED_SHORT = 0x1403, // GL_UNSIGNED_SHORT
		UNSIGNED_INT = 0x1405, // GL_UNSIGNED_INT
		FLOAT = 0x1406  // GL_FLOAT
	};

	enum class GLStateParam : int32_t {
		MAX_TEXTURE_SIZE = 0x0D33, // GL_MAX_TEXTURE_SIZE
		VIEWPORT = 0x0BA2, // GL_VIEWPORT
		MAJOR_VERSION = 0x821B, // GL_MAJOR_VERSION
		MINOR_VERSION = 0x821C, // GL_MINOR_VERSION
		MAX_VERTEX_ATTRIBS = 0x8869, // GL_MAX_VERTEX_ATTRIBS
		// add more as needed
	};
	enum class BufferTarget : int32_t {
		ARRAY_BUFFER = 0x8892, // GL_ARRAY_BUFFER
		ELEMENT_ARRAY_BUFFER = 0x8893, // GL_ELEMENT_ARRAY_BUFFER
		UNIFORM_BUFFER = 0x8A11, // GL_UNIFORM_BUFFER
		TEXTURE_BUFFER = 0x8C2A, // GL_TEXTURE_BUFFER
		// add others as needed
	};
	enum class IndexType : int32_t {
		UNSIGNED_BYTE = 0x1401, // GL_UNSIGNED_BYTE
		UNSIGNED_SHORT = 0x1403, // GL_UNSIGNED_SHORT
		UNSIGNED_INT = 0x1405,  // GL_UNSIGNED_INT
		FLOAT = 0x1406
	};
	enum class DrawMode : int32_t {
		POINTS = 0x0000, // GL_POINTS
		LINE_STRIP = 0x0003, // GL_LINE_STRIP
		LINE_LOOP = 0x0002, // GL_LINE_LOOP
		LINES = 0x0001, // GL_LINES
		LINE_STRIP_ADJACENCY = 0x000B, // GL_LINE_STRIP_ADJACENCY
		LINES_ADJACENCY = 0x000A, // GL_LINES_ADJACENCY
		TRIANGLE_STRIP = 0x0005, // GL_TRIANGLE_STRIP
		TRIANGLE_FAN = 0x0006, // GL_TRIANGLE_FAN
		TRIANGLES = 0x0004, // GL_TRIANGLES
		TRIANGLE_STRIP_ADJACENCY = 0x000D, // GL_TRIANGLE_STRIP_ADJACENCY
		TRIANGLES_ADJACENCY = 0x000C, // GL_TRIANGLES_ADJACENCY
		PATCHES = 0x000E  // GL_PATCHES
	};
	enum class ProgramParam : int32_t {
		LINK_STATUS = 0x8B82, // GL_LINK_STATUS
		VALIDATE_STATUS = 0x8B83, // GL_VALIDATE_STATUS
		INFO_LOG_LENGTH = 0x8B84, // GL_INFO_LOG_LENGTH
		ATTACHED_SHADERS = 0x8B85, // GL_ATTACHED_SHADERS
		ACTIVE_ATTRIBUTES = 0x8B89, // GL_ACTIVE_ATTRIBUTES
		ACTIVE_ATTRIBUTE_MAX_LENGTH = 0x8B8A, // GL_ACTIVE_ATTRIBUTE_MAX_LENGTH
		ACTIVE_UNIFORMS = 0x8B86, // GL_ACTIVE_UNIFORMS
		ACTIVE_UNIFORM_MAX_LENGTH = 0x8B87  // GL_ACTIVE_UNIFORM_MAX_LENGTH
	};
	enum class ShaderParam : int32_t {
		COMPILE_STATUS = 0x8B81,
		INFO_LOG_LENGTH = 0x8B84,
	}  ;

	enum shaderTypes : int32_t {
		GL_COMPUTE_SHADER = 0x91B9,
		GL_VERTEX_SHADER = 0x8B31,
		GL_TESS_CONTROL_SHADER = 0x8E88,
		GL_TESS_EVALUATION_SHADER = 0x8E87,
		GL_GEOMETRY_SHADER = 0x8DD9,
		GL_FRAGMENT_SHADER = 0x8B30
	};
	enum class GLenum1 : unsigned int{
	 GL_TEXTURE_2D = 0x0DE1
	};


	enum class GL_BitField : uint32_t {
		COLOR_BUFFER_BIT = 0x00004000,
		DEPTH_BUFFER_BIT = 0x00000100,
		STENCIL_BUFFER_BIT = 0x00000400
	}  ;
	enum class GL_FrameBufferTarget : uint32_t {
		GL_DRAW_FRAMEBUFFER = 0x8CA9,
		GL_READ_FRAMEBUFFER = 0x8CA8,
		GL_FRAMEBUFFER = 0x8D40
	}  ;
	enum class GL_DrawMode : uint32_t {
		POINTS = 0x0000,
		LINE_STRIP = 0x0003,
		LINE_LOOP = 0x0002,
		LINES = 0x0001,
		LINE_STRIP_ADJACENCY = 0x000B,
		LINES_ADJACENCY = 0x000A,
		TRIANGLE_STRIP = 0x0005,
		TRIANGLE_FAN = 0x0006,
		TRIANGLES = 0x0004,
		TRIANGLE_STRIP_ADJACENCY = 0x000D,
		TRIANGLES_ADJACENCY = 0x000C,
		PATCHES = 0x000E
	}  ;
	enum class GL_Usage : uint32_t {
		STREAM_DRAW = 0x88E0,
		STATIC_DRAW = 0x88E4,
		DYNAMIC_DRAW = 0x88E8
	}  ;
	 
		inline GL_BitField operator|(GL_BitField a, GL_BitField b) {
			return static_cast<GL_BitField>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
		}
	class GL{ 
	public: 

	
		/// <summary>
	/// Matrix4x4 Adjusted projection matrix for the current graphics API.
	/// contains platform specific changes to handle y-flip and reverse z. 
	/// </summary>
	/// <param name="proj"></param>
	/// <param name="renderIntoTexture"></param>
	/// <returns></returns>
		static  Matrix4x4 GetGPUProjectionMatrix( Matrix4x4 proj) {
#if defined(SYSTEM_GRAPHICS_OPENGL)
			    return proj;
			Matrix4x4 m = proj;

			// Flip Y axis
			m.m11 *= -1.0f;

			// Reverse Z: flip depth range so near=1, far=0
			// m.m22 *= -1.0f;   // flip the Z scale
			 //m.m32 *= -1.0f;   // flip the Z translation

			return m;


#elif defined(SYSTEM_GRAPHICS_VULKAN)
			return proj;
#else
			return proj;
#endif
		}
 
		static bool IsExtensionSupported(const std::string& v) {
#if defined(SYSTEM_GRAPHICS_OPENGL)
			int GL_NUM_EXTENSIONS = 0x821D;
			int GL_EXTENSIONS = 0x1F03;
			int numExtensions = 0;
			gl_glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
			for (int i = 0; i < numExtensions; ++i) {
				const char* extension = reinterpret_cast<const char*>(gl_glGetStringi(GL_EXTENSIONS, i));
				if (extension && std::string(extension) == v) {
					return true;
				}
			}
			return false;
#else

			return false;
#endif
		}
		static std::vector<std::string> ListExtensions(){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			std::vector<std::string> extensions;
			int numExtensions = 0;
			int GL_NUM_EXTENSIONS = 0x821D;
			int GL_EXTENSIONS = 0x1F03;
			gl_glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
			for (int i = 0; i < numExtensions; ++i) {
				const char* extension = reinterpret_cast<const char*>(gl_glGetStringi(GL_EXTENSIONS, i));
				if (extension) {
					extensions.emplace_back(extension);
				}
			}
			return extensions;
#else
			return std::vector<std::string>();
#endif
		}
		/// <summary>
		/// Creates a shader object
		/// </summary>
		/// <param name="n">Specifies the type of shader to be created. Must be one of GL_COMPUTE_SHADER, GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER, or GL_FRAGMENT_SHADER</param>
		/// <returns></returns>
		inline static uint32_t gl_glCreateShader(shaderTypes n){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			return SYSTEM_INTERNAL_glCreateShader(n);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		/// <summary>
		/// render primitives from array data
		/// </summary>
		/// <param name="mode">Specifies what kind of primitives to render. Symbolic constants GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY and GL_PATCHES are accepted.</param>
		/// <param name="count">Specifies the number of elements to be rendered.</param>
		/// <param name="type">Specifies the type of the values in indices. Must be one of GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, or GL_UNSIGNED_INT.</param>
		/// <param name="indices">Specifies a pointer to the location where the indices are stored.</param>
		inline static void gl_glDrawElements(DrawMode mode, int count, IndexType type, const void* indices){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glDrawElements(static_cast<GLenum>(mode), static_cast<GLsizei>(count), static_cast<GLenum>(type),   indices);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		/// <summary>
		/// updates a subset of a buffer objects data store
		/// </summary>
		/// <param name="target"></param>
		/// <param name="offset"></param>
		/// <param name="size"></param>
		/// <param name="data"></param>
		inline static void gl_glBufferSubData(BufferTarget target,GLintptr offset,GLsizeiptr size,const void* data) {
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glBufferSubData(static_cast<GLenum>(target), offset, size, data);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		inline static void gl_glBindBuffer(BufferTarget target, GLuint buffer){//GLenum
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glBindBuffer(static_cast<GLenum>(target),  buffer);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		} 
		inline static void gl_glBufferData(BufferTarget target, GLsizeiptr size, const void* data, GL_Usage usage){//GL_Usage
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glBufferData(static_cast<GLenum>(target),size,data, static_cast<GLenum>(usage));
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
 		inline static void gl_glVertexAttribPointer(GLuint index, GLint size, IndexType type, GLboolean normalized, GLsizei stride, const void* pointer){//IndexType
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glVertexAttribPointer( index,  size, static_cast<GLenum>(type),  normalized,  stride, pointer);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}








		inline static void gl_glFrontFace(WindingOrder w){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glFrontFace(static_cast<GLenum>(w));
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}


		inline static void gl_glDisable(GraphicsCapability w){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glDisable(static_cast<GLenum>(w));
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		inline static void gl_glEnable(GraphicsCapability w) {
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glEnable(static_cast<GLenum>(w));
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}






		inline static void gl_glUniform3fv(GLint location, GLsizei count, const GLfloat* value) {
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glUniform3fv(location, count, value);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		//inline static void  gl_glDepthFunc(GL_ALWAYS)


		inline static void gl_glDepthFunc(DepthFunc func) {
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glDepthFunc(static_cast<GLenum>(func));
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}















		inline static void gl_glReadPixels(GLint x,
									GLint y,
									GLsizei width,
									GLsizei height,
									PixelFormat format,
									PixelType type,
									void* data) {
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glReadPixels(x,
						   y,
						   width,
						   height,
						   static_cast<GLenum>(format),
						   static_cast<GLenum>(type),
						   data);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		inline static void gl_glGetIntegerv(GLStateParam pname, GLint* data) {
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glGetIntegerv(static_cast<GLenum>(pname), data);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}

		inline static void gl_glBindTexture(GLenum1 target, uint32_t texture){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glBindTexture( (unsigned int)target,   texture);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		inline static void gl_glTexSubImage2D(){
			//SYSTEM_INTERNAL_glTexSubImage2D();
		}

		inline static void gl_glUniformMatrix4fv(int location, int count, bool transpose, const float* value) {
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glUniformMatrix4fv(location, count, transpose, value);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		inline static int gl_glGetUniformLocation(uint32_t program, const char* name) {
#if defined(SYSTEM_GRAPHICS_OPENGL)
			return SYSTEM_INTERNAL_glGetUniformLocation(program, name);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		inline static void gl_glBindFramebuffer(GL_FrameBufferTarget target, uint32_t framebuffer){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glBindFramebuffer(static_cast<GLenum>(target), static_cast<GLuint>(framebuffer));
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		inline static void gl_glViewport(float x, float y, float width, float height) {
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glViewport(x,y,width,height);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		inline static const uint8_t* gl_glGetString(GLenum name) {
#if defined(SYSTEM_GRAPHICS_OPENGL)
			return reinterpret_cast<const uint8_t*>(SYSTEM_INTERNAL_glGetString(name));
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		inline static const uint8_t* gl_glGetStringi(GLenum name, GLuint index) {
#if defined(SYSTEM_GRAPHICS_OPENGL)
			return reinterpret_cast<const uint8_t*>(SYSTEM_INTERNAL_glGetStringi(name, index));
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		inline static void gl_glClear(GL_BitField BitField) {
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glClear(static_cast<int>(BitField));
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		inline static void gl_glClearColor(float r, float g, float b, float a){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glClearColor(r, g, b, a);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		inline static void gl_glDrawArrays(GL_DrawMode mode, int first, int count) {
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glDrawArrays(static_cast<GLenum>(mode), static_cast<GLint>(first), static_cast<GLsizei>(count));//(GLenum mode, GLint first, GLsizei count);
#endif
		}
		inline static void gl_glGenBuffers(int n,uint32_t* buffers) {
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glGenBuffers(n,buffers);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}


		inline static void gl_glShaderSource(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glShaderSource(shader,count,string,length);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		inline static void gl_glCompileShader(GLuint shader){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glCompileShader(shader);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		inline static void gl_glGetShaderiv(GLuint shader, ShaderParam pname, GLint* params) {
			SYSTEM_INTERNAL_glGetShaderiv(shader, static_cast<GLenum>(pname), params);
		}
		inline static void gl_glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glGetShaderInfoLog(shader, bufSize,length,infoLog);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		inline static uint32_t gl_glCreateProgram(){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			return SYSTEM_INTERNAL_glCreateProgram();
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		inline static void gl_glAttachShader(GLuint program, GLuint shader){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glAttachShader(program, shader);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		inline static void gl_glLinkProgram(GLuint program){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glLinkProgram(program);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		/// <summary>
		/// glGetProgramiv
		/// </summary>
		/// <param name="program"></param>
		/// <param name="pname"></param>
		/// <param name="params"></param>
		inline static void gl_glGetProgramiv(GLuint program, ProgramParam pname, GLint* params){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glGetProgramiv(program, static_cast<GLenum>(pname),params);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		inline static void gl_glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glGetProgramInfoLog(program, bufSize,  length, infoLog);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		inline static void gl_glUseProgram(GLuint program){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glUseProgram(program);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		inline static void gl_glDeleteShader(GLuint shader){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glDeleteShader(shader);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}

		inline static void gl_glEnableVertexAttribArray(GLuint index){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glEnableVertexAttribArray(index);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}

		inline static void gl_glGenVertexArrays(GLsizei n, GLuint* arrays){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glGenVertexArrays(  n,  arrays);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}


		inline static void gl_glBindVertexArray(GLuint array){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glBindVertexArray(  array);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		inline static void gl_glGetIntegerv(GLenum pname, GLint* data){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glGetIntegerv(  pname,   data);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}

		inline static void gl_glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glUniform4f(  location,   v0,   v1,   v2,   v3);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		inline static void gl_glUniform1i(GLint location, GLint v0){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glUniform1i(  location,   v0);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		inline static void gl_glUniform1f(GLint location, GLfloat v0){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glUniform1f(  location,   v0);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		inline static void gl_glTexParameteri(GLenum target, GLenum pname, GLint param){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glTexParameteri(  target,   pname,   param);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		inline static void gl_glTexParameterfv(GLenum target, GLenum pname, const GLfloat* params){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glTexParameterfv(  target,   pname,   params);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		inline static void gl_glGenerateMipmap(GLenum target){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glGenerateMipmap(  target);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}

		inline static void gl_glActiveTexture(GLenum texture){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glActiveTexture(  texture);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		inline static void gl_glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glTexImage2D(  target,   level,   internalformat,   width,   height,   border,   format,   type,   pixels);
#else
			throw std::runtime_error("GraphicsHelpers gl function not implemented");
#endif
		}
		
	}; 




} 

#endif