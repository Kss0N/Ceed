

out vec3 Normal;
out vec2 TexCoord;
out vec3 CrntPos;

in DATA 
{
    vec3 Normal;
    vec2 texCoord;
    mat4 projection;
} data_in[];

void main(){
    Normal = data_in[0].Normal;
    TexCoord = data_in[0].texCoord;
    CrntPos = vec3(gl_Position);
    EmitVertex();

    Normal = data_in[1].Normal;
    TexCoord = data_in[1].texCoord;
    CrntPos = vec3(gl_Position);
    EmitVertex();

    Normal = data_in[2].Normal;
    TexCoord = data_in[2].texCoord;
    CrntPos = vec3(gl_Position);
    EmitVertex();


    EndPrimitive();
}