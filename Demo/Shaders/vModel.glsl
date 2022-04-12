#version 450 core

// vertex attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

// forward to fragment shader
out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;// fragment position in world space
out vec3 ViewPos;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 inverse_model;
uniform vec3 view_pos;

mat3 NormalMatrix;

// mat4 model, vec3 tangent, vec3 bitangent, normal
mat3 create_TBN_matrix(){
    vec3 T = normalize(NormalMatrix * aTangent);
    vec3 B = normalize(NormalMatrix * aBitangent);
    vec3 N = normalize(NormalMatrix * aNormal);
    mat3 TBN = mat3(T, B, N);
    return TBN;
}

void main()
{
    NormalMatrix = mat3(transpose(inverse_model));
    Normal =  NormalMatrix * aNormal;
    TexCoords = aTexCoords;
    FragPos = vec3(model * vec4(aPos, 1.0));
    ViewPos = view_pos;
    TBN = create_TBN_matrix();
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}