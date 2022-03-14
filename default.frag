#version 460 core

out vec4 FragColor;



in vec3 Normal;
in vec2 TexCoord;
in vec3 CrntPos;



/*Light Source Position*/
uniform vec3 lgPos;
uniform vec3 viewPos;



void main(){
    float ambientFactor = 0.5f;
    vec3 viewDir = normalize(viewPos - CrntPos);
    vec3  norm   = normalize(Normal);
    vec3  lgDir  = normalize(lgPos - CrntPos);
    vec3  refDir = reflect(-lightDir, norm);
    float diffse = max(dot(norm, lgDir), 0); /*Don't want "negative light"*/
    float speculr= pow(max(dot(viewDir, refDir), 0), 8);

    

}