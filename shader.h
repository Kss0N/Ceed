#ifndef SHADER_H
#include "framework.h"
#include "utils.h" 


/*
	Creates an OpenGL shader program from a Vertex, Geometry and Fragment source file

	@param vertPath is the path to a Vertex Shader source file
	@param geomPath is the path to a Geometry Shader source file
	@param fragPath is the path to a Fragment Shader source file
	@return GL reference to a Shader Program Object made from said source files.
*/
GLuint cdCreateShader(_In_opt_ const char* vertPath,
	_In_opt_ const char* geomPath,
	_In_opt_ const char* fragPath,
	_Inout_ buffer* buffer);

#endif // !SHADER_H

