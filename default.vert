#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTex;

uniform mat4 transformation;
uniform mat4 camMatrix;

out vec3 Normal;
out vec3 FragPos;

void main(){
    gl_Position = camMatrix * transformation * vec4(aPos, 1);
    Normal = aNormal;
    FragPos = vec3(gl_Position);

}