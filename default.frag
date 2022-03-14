#version 460 core

out vec4 FragColor;



in vec3 Normal;
in vec3 FragPos;



/*Light Source Position*/
uniform vec3 lgPos;
uniform vec3 viewPos;



void main(){
    float ambient = 0.5f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3  norm   = normalize(Normal);
    vec3  lgDir  = normalize(lgPos - FragPos);
    vec3  refDir = reflect(-lgDir, norm);
    float diffse = max(dot(norm, lgDir), 0); /*Don't want "negative light"*/
    float speculr= pow(max(dot(viewDir, refDir), 0), 8);

    FragColor = (ambient+speculr+diffse) * vec4(0.5,0.5,0.5);

}