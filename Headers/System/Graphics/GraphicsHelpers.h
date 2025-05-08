#ifndef _SYSTEM_GRAPHICS_H
#define _SYSTEM_GRAPHICS_H

#if (defined(__x86_64__) || defined(_M_X64)) && defined(_WIN32)
#define _SYSTEM_OPENGL_WIN
#include <cstdint>
#elif defined(__arm__) || defined(_M_ARM)
#define _SYSTEM_OPENGLES_ARM
#endif
#include <System/Internal/InternalGLloader.h> 
#include <string>





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
#ifdef _SYSTEM_OPENGL_WIN
	typedef enum GL_BitField : uint32_t {
		COLOR_BUFFER_BIT = 0x00004000,
		DEPTH_BUFFER_BIT = 0x00000100,
		STENCIL_BUFFER_BIT = 0x00000400
	} GL_BitField;
	typedef enum GL_FrameBufferTarget : uint32_t {
		GL_DRAW_FRAMEBUFFER,
		GL_READ_FRAMEBUFFER,
		GL_FRAMEBUFFER
	} GL_FrameBufferTarget;
	typedef enum GL_DrawMode : uint32_t {
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
		} GL_DrawMode;
	typedef enum GL_Usage : uint32_t {
		STREAM_DRAW,
		STATIC_DRAW,
		DYNAMIC_DRAW
		} GL_Usage;
	 
		inline GL_BitField operator|(GL_BitField a, GL_BitField b) {
			return static_cast<GL_BitField>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
		}
	class GL{ 
	public: 

	
	


		static bool IsExtensionSupported(const std::string& v) {
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
		}
		static std::vector<std::string> ListExtensions() {
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
		}
		inline static void gl_glUniformMatrix4fv(int location, int count, bool transpose, const float* value) {
			SYSTEM_INTERNAL_glUniformMatrix4fv(location, count, transpose, value);
		}
		inline static int gl_glGetUniformLocation(uint32_t program, const char* name) {
			return SYSTEM_INTERNAL_glGetUniformLocation(program, name);
		}
		inline static void gl_glBindFramebuffer(GL_FrameBufferTarget target, uint32_t framebuffer){
			SYSTEM_INTERNAL_glBindFramebuffer(static_cast<GLenum>(target), static_cast<GLuint>(framebuffer));
		}
		inline static void gl_glViewport(float x, float y, float width, float height) {
			SYSTEM_INTERNAL_glViewport(x,y,width,height);
		}
		inline static const uint8_t* gl_glGetString(GLenum name) {
			return reinterpret_cast<const uint8_t*>(SYSTEM_INTERNAL_glGetString(name));
		}
		inline static const uint8_t* gl_glGetStringi(GLenum name, GLuint index) {
			return reinterpret_cast<const uint8_t*>(SYSTEM_INTERNAL_glGetStringi(name, index));
		}
		inline static void gl_glClear(GL_BitField BitField) {
			SYSTEM_INTERNAL_glClear(BitField);
		}
		inline static void gl_glClearColor(float r, float g, float b, float a){
			SYSTEM_INTERNAL_glClearColor(r, g, b, a);
		}
		inline static void gl_glDrawArrays(GL_DrawMode mode, int first, int count) {
			SYSTEM_INTERNAL_glDrawArrays(static_cast<GLenum>(mode), static_cast<GLint>(first), static_cast<GLsizei>(count));//(GLenum mode, GLint first, GLsizei count);
		}
		inline static void gl_glGenBuffers(int n,uint32_t* buffers) {
			SYSTEM_INTERNAL_glGenBuffers(n,buffers);
		}
		inline static void gl_glBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage){
			SYSTEM_INTERNAL_glBufferData(target,size,data,usage);
		}
		inline static void gl_glCreateShader(int n){
			SYSTEM_INTERNAL_glCreateShader(n);
		}
		inline static void gl_glShaderSource(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length){
			SYSTEM_INTERNAL_glShaderSource(shader,count,string,length);
		}
		inline static void gl_glCompileShader(GLuint shader){
			SYSTEM_INTERNAL_glCompileShader(shader);
		}
		inline static void gl_glGetShaderiv(GLuint shader, GLenum pname, GLint* params){
			SYSTEM_INTERNAL_glGetShaderiv(shader,pname,params);
		}
		inline static void gl_glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog){
			SYSTEM_INTERNAL_glGetShaderInfoLog(shader, bufSize,length,infoLog);
		}
		inline static void gl_glCreateProgram(){
			SYSTEM_INTERNAL_glCreateProgram();
		}
		inline static void gl_glAttachShader(GLuint program, GLuint shader){
			SYSTEM_INTERNAL_glAttachShader(program, shader);
		}
		inline static void gl_glLinkProgram(GLuint program){
			SYSTEM_INTERNAL_glLinkProgram(program);
		}
		inline static void gl_glGetProgramiv(GLuint program, GLenum pname, GLint* params){
			SYSTEM_INTERNAL_glGetProgramiv(program,pname,params);
		}
		inline static void gl_glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog){
			SYSTEM_INTERNAL_glGetProgramInfoLog(program, bufSize,  length, infoLog);
		}
		inline static void gl_glUseProgram(GLuint program){
			SYSTEM_INTERNAL_glUseProgram(program);
		}
		inline static void gl_glDeleteShader(GLuint shader){
			SYSTEM_INTERNAL_glDeleteShader(shader);
		}
		inline static void gl_glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer){
			SYSTEM_INTERNAL_glVertexAttribPointer( index,  size,  type,  normalized,  stride, pointer);
		}
		inline static void gl_glEnableVertexAttribArray(GLuint index){
			SYSTEM_INTERNAL_glEnableVertexAttribArray(index);
		}
		inline static void gl_glBindBuffer(GLenum target, GLuint buffer){
			SYSTEM_INTERNAL_glBindBuffer( target,  buffer);
		} 
		inline static void gl_glGenVertexArrays(GLsizei n, GLuint* arrays){
			SYSTEM_INTERNAL_glGenVertexArrays(  n,  arrays);
		}
		inline static void gl_glDrawArrays(GLenum mode, GLint first, GLsizei count){
			SYSTEM_INTERNAL_glDrawArrays(  mode,   first,   count);
		}
		inline static void gl_glDrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices){
			SYSTEM_INTERNAL_glDrawElements(  mode,   count,   type,   indices);
		} 
		inline static void gl_glBindVertexArray(GLuint array){
			SYSTEM_INTERNAL_glBindVertexArray(  array);
		}
		inline static void gl_glGetIntegerv(GLenum pname, GLint* data){
			SYSTEM_INTERNAL_glGetIntegerv(  pname,   data);
		}

		inline static void gl_glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3){
			SYSTEM_INTERNAL_glUniform4f(  location,   v0,   v1,   v2,   v3);
		}
		inline static void gl_glUniform1i(GLint location, GLint v0){
			SYSTEM_INTERNAL_glUniform1i(  location,   v0);
		}
		inline static void gl_glUniform1f(GLint location, GLfloat v0){
			SYSTEM_INTERNAL_glUniform1f(  location,   v0);
		}
		inline static void gl_glTexParameteri(GLenum target, GLenum pname, GLint param){
			SYSTEM_INTERNAL_glTexParameteri(  target,   pname,   param);
		}
		inline static void gl_glTexParameterfv(GLenum target, GLenum pname, const GLfloat* params){
			SYSTEM_INTERNAL_glTexParameterfv(  target,   pname,   params);
		}
		inline static void gl_glGenerateMipmap(GLenum target){
			SYSTEM_INTERNAL_glGenerateMipmap(  target);
		}
		inline static void gl_glBindTexture(GLenum target, GLuint texture){
			SYSTEM_INTERNAL_glBindTexture(  target,   texture);
		}
		inline static void gl_glActiveTexture(GLenum texture){
			SYSTEM_INTERNAL_glActiveTexture(  texture);
		}
		inline static void gl_glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels){
			SYSTEM_INTERNAL_glTexImage2D(  target,   level,   internalformat,   width,   height,   border,   format,   type,   pixels);
		}

	}; 

#endif

 

}
#endif