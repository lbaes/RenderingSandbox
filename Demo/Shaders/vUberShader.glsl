#define NR_POINT_LIGHTS 5

// vertex attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;

// forward to fragment shader
out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;
out vec3 ViewPos;

#if defined(USE_NORMAL_MAP)
out TangentPointLight
{
    vec3 FragPosTangent;
    vec3 ViewPosTangent;
    vec3 LightPosTangent[NR_POINT_LIGHTS];
};
uniform vec3 light_pos[NR_POINT_LIGHTS];
#endif

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 inverse_model;
uniform vec3 view_pos;

mat3 create_TBN_matrix(mat3 normal_matrix){
    vec3 T = normalize(normal_matrix * aTangent);
    vec3 N = normalize(normal_matrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    mat3 TBN = mat3(T, B, N);
    return transpose(TBN);
}

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    ViewPos = view_pos;
    TexCoords = aTexCoords;

    // Normals
    mat3 NormalMatrix = transpose(inverse_model);
    Normal =  NormalMatrix * aNormal;

    #if defined(USE_NORMAL_MAP)
    mat3 TBN = create_TBN_matrix(NormalMatrix);
    FragPosTangent = TBN * FragPos;
    ViewPosTangent =  TBN * view_pos;
    for (int i = 0; i < NR_POINT_LIGHTS; ++i){
        LightPosTangent[i] = TBN * light_pos[i];
    }
    #endif
}