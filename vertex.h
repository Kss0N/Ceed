#ifndef VERTEX_H
#define VERTEX_H 1

#include "linalg.h"

#define POS_LOCATION 0
#define NORMAL_LOCATION 1
#define TEXTURE_LOCATION 2

typedef struct Vertex {
	vec3 pos;
	vec3 normal;
	vec2 texture;
} Vertex;


#endif // !VERTEX_H

