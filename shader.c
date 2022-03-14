
#include "shader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"



static GLuint createSource(GLenum type, const char* path, buffer* pBuffer);

/*
	files is a null-terminated array of filepaths, names for files 
*/
GLuint cdCreateShader(_In_opt_ const char* vertPath, 
					_In_opt_ const char* geomPath, 
					_In_opt_ const char* fragPath,
					_Inout_ buffer* pBuffer){
	GLuint Shader = glCreateProgram();

	GLuint vertShader;
	GLuint geomShader;
	GLuint fragShader;

	if (vertPath != NULL) {
		vertShader = createSource(GL_VERTEX_SHADER, vertPath, pBuffer);
		glAttachShader(Shader, vertShader);
	}
	if (geomPath != NULL) {
		geomShader = createSource(GL_GEOMETRY_SHADER_ARB, geomPath, pBuffer);
		glAttachShader(Shader, geomShader);
	}
	if (fragPath != NULL) {
		fragShader = createSource(GL_FRAGMENT_SHADER, fragPath, pBuffer);
		glAttachShader(Shader, fragShader);
	}
	glLinkProgram(Shader);

	if(vertPath) glDeleteShader(vertShader);
	if(geomPath) glDeleteShader(geomShader);
	if(fragPath) glDeleteShader(fragShader);
	
	return Shader;
}



static GLuint createSource(GLenum type, const char* path, buffer* pBuffer) {
	GLuint shaderSource = glCreateShader(type);
	FILE* file;
	errno_t err = fopen_s(&file, path, "rb"); //read in binary mode
	if (!file) {
		
		printf_s("Error %i unable to open file %s", err, path);
		cdExitUnsuccessful("unable to open file");
	}
	char ch = 0;
	char* iterator = *pBuffer;
	while (1) {
		if (cdIsAtEndOfBuffer(*pBuffer, iterator)) {
			//reallocation time
			size_t oldSize = cdGetBufferSize(*pBuffer);
			cdReallocateBuffer(pBuffer, 256);
			iterator = *pBuffer + oldSize;
		}
		ch = fgetc(file);
		if (ch == EOF) break; //end at End Of File
		*iterator++ = ch; 
	}
	glShaderSource(shaderSource, 1, pBuffer, NULL);
	glCompileShader(shaderSource);
	
#ifdef _DEBUG
	//Check Compilation
	GLint hasCompiled;
	char log[1024];
	glGetShaderiv(shaderSource, GL_COMPILE_STATUS, &hasCompiled);
	if (!hasCompiled) {
		GLsizei len;
		glGetShaderInfoLog(shaderSource, 1024, &len, log);
		cdExitUnsuccessful(log);
	}
#endif // _DEBUG

	fclose(file);
	return shaderSource;
}
