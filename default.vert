#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTex;

uniform mat4 transformation;
uniform mat4 camMatrix;



out DATA 
{
    vec3 Normal;
    vec2 texCoord;
    mat4 projection;
} data_out;


void main(){
    gl_Position = transformation * vec4(aPos, 1);
    data_out.Normal = aNormal;
    data_out.texCoord =   mat2(0, -1, 
                                1, 0) * aTex; 
    data_out.projection = camMatrix;
}