/**
@author J. Keson
@version 1.0

@description:
Basic model loader for Wavefront .Obj files
*/

#include "mesh.h"

#include "utils.h"
#include "vertex.h"
#include <stdio.h>
#include <string.h>


/*
* Notes for Wavefront .obj
* If a line begins with '#', discard it for it is a comment
* 
* 'v' denotes the beginning of a geometric vertex. it's a vec3 or vec4, 
* if only three numbers follow, the 4 number will default to 1.0f 
* 
* if it begins with 'vt' the input is a vec3 (vertex) texture coordinate.
* if only one number is supplied the other two will default to 0.
* 
* 'vn' denotes a vec3 vector normal
* 
* 'vp' is vertex space Parameter
*/

size_t peak(FILE* file, buffer buffer, unsigned int nElements) {
	if (cdGetBufferSize(buffer) < nElements) return -1;
	size_t nRead = fread(buffer, 1, nElements, file);
	fseek(file, -(signed)nRead, SEEK_CUR);
	return nRead;


}


/*
Will go to the next line.
@param file buffer to work in
@returns 0 on normally, -1 if eof is reached.


*/



#define ADD_BUFFER_SIZE 256




#define INIT_SIZE_FOR_ATTRIBUTE_BUFFERS 1024
#define VERTEX_STRIDE sizeof(Vertex)


Mesh createMesh(const char* objFile) {
	/*open and input file*/
	FILE* File;
	fopen_s(&File, objFile, "r");
	if (File == NULL) {
		cdExitUnsuccessful("Cannot file for Mesh creation");
	}

	buffer vertBuffer = cdCreateBuffer(VERTEX_STRIDE * INIT_SIZE_FOR_ATTRIBUTE_BUFFERS);
	buffer normBuffer = cdCreateBuffer(sizeof(vec3) * INIT_SIZE_FOR_ATTRIBUTE_BUFFERS);
	buffer textBuffer = cdCreateBuffer(sizeof(vec2) * INIT_SIZE_FOR_ATTRIBUTE_BUFFERS);
	
	buffer elemBuffer = cdCreateBuffer(sizeof(unsigned int) * 3 * INIT_SIZE_FOR_ATTRIBUTE_BUFFERS);

	buffer streamBuffer = cdCreateBuffer(64);

	char* vertIterator = vertBuffer;
	char* normIterator = normBuffer;
	char* textIterator = textBuffer; //offset is 2 vec3 because each vertex has a vec3 pos + vec3 normal before the texture coordinate

	char* elemIterator = elemBuffer;
	
	int bFailFlag = FALSE;
	while (!bFailFlag) {
		/*get a new line*/
		char* streamIterator = streamBuffer; //reset iterator
		while (1) {
			cdCheck_n_reallocBuffer(&streamBuffer, &streamIterator, +64);
			char ch = fgetc(File);
			*streamIterator++ = ch;
			if (ch == EOF || ch == '\n') {
				*streamIterator = '\0';
				break;
			}
		}
		if (*streamIterator == EOF) break;

		/*Comment, ignore*/
		if (streamBuffer[0] == '#') continue;

		/*Vertices*/
		else if (streamBuffer[0] == 'v') {
			switch (streamBuffer[1])
			{
			case ' ':
			{
				cdCheck_n_reallocBuffer(&vertBuffer, &vertIterator, +64 * VERTEX_STRIDE);
				/*Geom Vertex*/
				sscanf_s(streamBuffer, "v %f %f %f",
					((Vertex*)vertIterator)->pos + 0,
					((Vertex*)vertIterator)->pos + 1,
					((Vertex*)vertIterator)->pos + 2);
			}
			case 'n':
			{
				cdCheck_n_reallocBuffer(&normBuffer, &normIterator, +64 * sizeof(vec3));
				sscanf_s(streamBuffer, "vn %f %f %f",
					(float*)normBuffer + 0,
					(float*)normBuffer + 1,
					(float*)normBuffer + 2);
			}
			case 't':
			{
				cdCheck_n_reallocBuffer(&textBuffer, &textIterator, +64 * sizeof(vec2));
				sscanf_s(streamBuffer, "vt %f %f",
					(float*)textBuffer + 0,
					(float*)textBuffer + 1);
			}
			default:
				//Invalid, return null
				bFailFlag = TRUE;
				break;
			}
		}

		else if (streamBuffer[0] == 'f') 
		{
			/*
			*A Face is a polygon (contains three or more vertices)
			*OpenGL for all intents and purposes here only accepts triangles
			*Each polygon consists of indices to vertices which compose the polygon
			* 
			*/
			streamIterator += 2;
			int bEndOfLine = FALSE;
			while (!bEndOfLine)
			{
				unsigned int geomElement;
				unsigned int normElement;
				unsigned int textElement;
				/*geometry/texture/normal*/
				if (sscanf_s(streamIterator, "%d/%d/%d",
					&geomElement, &textElement, &normElement) == 2) 
				{

				}
				else 
				if (sscanf_s(streamIterator, "%d/%d",
					&geomElement, &textElement) == 2) 
				{
					normElement = 0;
				}
				else 
				if (sscanf_s(streamIterator, "%d//%d",
					&geomElement, &normElement) == 2) 
				{
					textElement = 0;
				}
				while (*streamIterator != '\n' && *streamIterator != ' ') {
					*streamIterator++;
				}
				/*Insert indices; DEAL WITH IT*/
				if (normElement) {
					laCopy3(((Vertex*)vertBuffer)[geomElement].normal, 
						((vec3*)normBuffer)[normElement]);
				}
				if (textElement) {
					laCopy2(((Vertex*)vertBuffer)[geomElement].texture, 
						((vec2*)textBuffer)[textElement]);
				}
				cdCheck_n_reallocBuffer
				(&elemBuffer, &elemIterator, sizeof(unsigned int) * 3 * ADD_BUFFER_SIZE);
				*elemIterator++ = geomElement;

				if (*streamIterator == '\n') break;
			}
			
		}
	}
	cdFreeBuffer(&streamBuffer);
	cdFreeBuffer(&normBuffer);
	cdFreeBuffer(&textBuffer);

	Mesh mesh;
	mesh.nVertices = vertIterator - vertBuffer;
	mesh.vertices = vertBuffer;

	mesh.nIndices = elemIterator - elemBuffer;
	mesh.indices = elemBuffer;

	if (bFailFlag) {
		deleteMesh(&mesh);
		return mesh;
	}

	/*Finally the OpenGL parts Enjoy hehe*/
	GLuint bufferObjects[2];
	glGenBuffers(2, bufferObjects);
	mesh.VBO = bufferObjects[0];
	mesh.EBO = bufferObjects[1];

	glGenVertexArrays(1, &(mesh.VAO));
	glBindVertexArray(mesh.VAO);

	glBindBuffer(GL_VERTEX_ARRAY, mesh.VBO);
	glBufferData(GL_VERTEX_ARRAY, mesh.nVertices, mesh.vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.nIndices, mesh.indices, GL_STATIC_DRAW);

	glVertexAttribPointer(POS_LOCATION,		VEC3_DIMENSIONS, GL_FLOAT, GL_FALSE, VERTEX_STRIDE,	(void*)0);
	glEnableVertexAttribArray(POS_LOCATION);
	glVertexAttribPointer(NORMAL_LOCATION,	VEC3_DIMENSIONS, GL_FLOAT, GL_FALSE, VERTEX_STRIDE, (void*)(sizeof(vec3)));
	glEnableVertexAttribArray(NORMAL_LOCATION);
	glVertexAttribPointer(TEXTURE_LOCATION, VEC2_DIMENSIONS, GL_FLOAT, GL_FALSE, VERTEX_STRIDE, (void*)(2 * sizeof(vec3)));
	glEnableVertexAttribArray(TEXTURE_LOCATION);



	return mesh;
}



void deleteMesh(Mesh* mesh) {
	glDeleteBuffers(2, &mesh->VBO);
	glDeleteVertexArrays(1, &mesh->VAO);



	cdFreeBuffer(&(mesh->vertices));
	cdFreeBuffer(&(mesh->indices));
}



void drawMesh(Mesh* mesh, GLuint Shader) {



	glBindVertexArray(mesh->VAO);
	glDrawElements(GL_TRIANGLES, mesh->nIndices, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}