#ifndef MESH_H
#define MESH_H

#include "framework.h"
#include "vertex.h"
#include "utils.h"





typedef struct {

	buffer vertices;
	unsigned long long nVertices;

	buffer indices;
	unsigned long long  nIndices;



	GLuint VAO; //Vertex Array Buffer
	GLuint VBO; //Vertex Buffer Object
	GLuint EBO; //Element Buffer Object


} Mesh;


/* Creates a Mesh from an .obj file*/
Mesh createMesh(const char* objFile);

void deleteMesh(Mesh* mesh);

void drawMesh(Mesh* mesh, GLuint Shader);

#endif // !MESH_H





