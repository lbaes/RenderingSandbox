
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
#if defined(USE_NORMAL_MAP)
out mat3 TBN;
#endif

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 inverse_model;
uniform vec3 view_pos;

// mat4 model, vec3 tangent, vec3 bitangent, normal
mat3 create_TBN_matrix(mat3 normal_matrix){
    vec3 T = normalize(normal_matrix * aTangent);
    vec3 B = normalize(normal_matrix * aBitangent);
    vec3 N = normalize(normal_matrix * aNormal);
    mat3 TBN = mat3(T, B, N);
    return TBN;
}

void main()
{
    // fragment position in world space
    FragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ViewPos = view_pos;
    TexCoords = aTexCoords;

    // Normals
    mat3 NormalMatrix = mat3(transpose(inverse_model));
    Normal =  NormalMatrix * aNormal;
    #if defined(USE_NORMAL_MAP)
    TBN = create_TBN_matrix(NormalMatrix);
    #endif
}