

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos;

in DATA 
{
    vec3 Normal;
    vec2 TexCoord;
} data_in[];

void main(){
    Normal = data_in[0].Normal;
    TexCoord = data_in[0].TexCoord;
    FragPos = vec3(gl_in[0].gl_Position);
    EmitVertex();

    Normal = data_in[1].Normal;
    TexCoord = data_in[1].TexCoord;
    FragPos = vec3(gl_in[1].gl_Position);
    EmitVertex();

    Normal = data_in[2].Normal;
    TexCoord = data_in[2].TexCoord;
    FragPos = vec3(gl_in[2].gl_Position);
    EmitVertex();


    EndPrimitive();
}