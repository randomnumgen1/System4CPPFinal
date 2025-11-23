#ifndef _SYSTEM_GRAPHICS_H
#define _SYSTEM_GRAPHICS_H

#if (defined(__x86_64__) || defined(_M_X64))// && defined(_WIN32)
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
	enum class GLenum1 : unsigned int{
	 GL_TEXTURE_2D = 0x0DE1
	};


	typedef enum GL_BitField : uint32_t {
		COLOR_BUFFER_BIT = 0x00004000,
		DEPTH_BUFFER_BIT = 0x00000100,
		STENCIL_BUFFER_BIT = 0x00000400
	} GL_BitField;
	typedef enum GL_FrameBufferTarget : uint32_t {
		GL_DRAW_FRAMEBUFFER = 0x8CA9,
		GL_READ_FRAMEBUFFER = 0x8CA8,
		GL_FRAMEBUFFER = 0x8D40
	} GL_FrameBufferTarget;
	typedef enum GL_DrawMode : uint32_t {
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
	} GL_DrawMode;
	typedef enum GL_Usage : uint32_t {
		STREAM_DRAW = 0x88E0,
		STATIC_DRAW = 0x88E4,
		DYNAMIC_DRAW = 0x88E8
	} GL_Usage;
	 
		inline GL_BitField operator|(GL_BitField a, GL_BitField b) {
			return static_cast<GL_BitField>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
		}
	class GL{ 
	public: 

	
	


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
#endif
		}




		inline static void gl_glBindTexture(GLenum1 target, uint32_t texture){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glBindTexture( (unsigned int)target,   texture);
#endif
		}
		inline static void gl_glTexSubImage2D(){
			//SYSTEM_INTERNAL_glTexSubImage2D();
		}











		inline static void gl_glUniformMatrix4fv(int location, int count, bool transpose, const float* value) {
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glUniformMatrix4fv(location, count, transpose, value);
#endif
		}
		inline static int gl_glGetUniformLocation(uint32_t program, const char* name) {
#if defined(SYSTEM_GRAPHICS_OPENGL)
			return SYSTEM_INTERNAL_glGetUniformLocation(program, name);
#endif
		}
		inline static void gl_glBindFramebuffer(GL_FrameBufferTarget target, uint32_t framebuffer){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glBindFramebuffer(static_cast<GLenum>(target), static_cast<GLuint>(framebuffer));
#endif
		}
		inline static void gl_glViewport(float x, float y, float width, float height) {
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glViewport(x,y,width,height);
#endif
		}
		inline static const uint8_t* gl_glGetString(GLenum name) {
#if defined(SYSTEM_GRAPHICS_OPENGL)
			return reinterpret_cast<const uint8_t*>(SYSTEM_INTERNAL_glGetString(name));
#endif
		}
		inline static const uint8_t* gl_glGetStringi(GLenum name, GLuint index) {
#if defined(SYSTEM_GRAPHICS_OPENGL)
			return reinterpret_cast<const uint8_t*>(SYSTEM_INTERNAL_glGetStringi(name, index));
#endif
		}
		inline static void gl_glClear(GL_BitField BitField) {
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glClear(BitField);
#endif
		}
		inline static void gl_glClearColor(float r, float g, float b, float a){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glClearColor(r, g, b, a);
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
#endif
		}
		inline static void gl_glBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glBufferData(target,size,data,usage);
#endif
		}
		inline static uint32_t gl_glCreateShader(int n){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glCreateShader(n);
#endif
		}
		inline static void gl_glShaderSource(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glShaderSource(shader,count,string,length);
#endif
		}
		inline static void gl_glCompileShader(GLuint shader){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glCompileShader(shader);
#endif
		}
		inline static void gl_glGetShaderiv(GLuint shader, GLenum pname, GLint* params){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glGetShaderiv(shader,pname,params);
#endif
		}
		inline static void gl_glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glGetShaderInfoLog(shader, bufSize,length,infoLog);
#endif
		}
		inline static uint32_t gl_glCreateProgram(){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glCreateProgram();
#endif
		}
		inline static void gl_glAttachShader(GLuint program, GLuint shader){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glAttachShader(program, shader);
#endif
		}
		inline static void gl_glLinkProgram(GLuint program){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glLinkProgram(program);
#endif
		}
		inline static void gl_glGetProgramiv(GLuint program, GLenum pname, GLint* params){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glGetProgramiv(program,pname,params);
#endif
		}
		inline static void gl_glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glGetProgramInfoLog(program, bufSize,  length, infoLog);
#endif
		}
		inline static void gl_glUseProgram(GLuint program){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glUseProgram(program);
#endif
		}
		inline static void gl_glDeleteShader(GLuint shader){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glDeleteShader(shader);
#endif
		}
		inline static void gl_glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glVertexAttribPointer( index,  size,  type,  normalized,  stride, pointer);
#endif
		}
		inline static void gl_glEnableVertexAttribArray(GLuint index){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glEnableVertexAttribArray(index);
#endif
		}
		inline static void gl_glBindBuffer(GLenum target, GLuint buffer){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glBindBuffer( target,  buffer);
#endif
		} 
		inline static void gl_glGenVertexArrays(GLsizei n, GLuint* arrays){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glGenVertexArrays(  n,  arrays);
#endif
		}

		inline static void gl_glDrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glDrawElements(  mode,   count,   type,   indices);
#endif
		} 
		inline static void gl_glBindVertexArray(GLuint array){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glBindVertexArray(  array);
#endif
		}
		inline static void gl_glGetIntegerv(GLenum pname, GLint* data){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glGetIntegerv(  pname,   data);
#endif
		}

		inline static void gl_glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glUniform4f(  location,   v0,   v1,   v2,   v3);
#endif
		}
		inline static void gl_glUniform1i(GLint location, GLint v0){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glUniform1i(  location,   v0);
#endif
		}
		inline static void gl_glUniform1f(GLint location, GLfloat v0){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glUniform1f(  location,   v0);
#endif
		}
		inline static void gl_glTexParameteri(GLenum target, GLenum pname, GLint param){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glTexParameteri(  target,   pname,   param);
#endif
		}
		inline static void gl_glTexParameterfv(GLenum target, GLenum pname, const GLfloat* params){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glTexParameterfv(  target,   pname,   params);
#endif
		}
		inline static void gl_glGenerateMipmap(GLenum target){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glGenerateMipmap(  target);
#endif
		}

		inline static void gl_glActiveTexture(GLenum texture){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glActiveTexture(  texture);
#endif
		}
		inline static void gl_glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels){
#if defined(SYSTEM_GRAPHICS_OPENGL)
			SYSTEM_INTERNAL_glTexImage2D(  target,   level,   internalformat,   width,   height,   border,   format,   type,   pixels);
#endif
		}
		
	}; 

#endif

 

} 