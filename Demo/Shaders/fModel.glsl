#version 450 core
#define BLINNPHONG
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec3 ViewPos;
in mat3 TBN;

struct Material {
    sampler2D diffuse1;
    sampler2D specular1;
    sampler2D height1;
    sampler2D normal1;
    sampler2D ambient1;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

void main()
{
    // Textures
    vec4 DiffuseTex = texture(material.diffuse1, TexCoords);
    vec4 SpecularTex = texture(material.specular1, TexCoords);
    vec4 NormalTex = texture(material.normal1, TexCoords);
    vec4 HeightTex = texture(material.height1, TexCoords);
    vec4 AmbientTex = texture(material.height1, TexCoords);

    // Lighting
    vec3 vNormal = normalize(Normal);
    vec3 normal = normalize(TBN * normalize(NormalTex.rgb * 2.0 - 1.0));
    vec3 light_direction = normalize(light.position - FragPos);
    vec3 view_direction = normalize(ViewPos - FragPos);
    vec3 reflection_direction = reflect(-light_direction, normal);
    vec3 halfway_direction = normalize(light_direction + view_direction);

    // Ambient
    vec3 ambient = light.ambient * DiffuseTex.rgb;

    // Diffuse
    float diffuse_multiplier = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = diffuse_multiplier * light.diffuse * DiffuseTex.rgb;

    // Specular
    #ifdef PHONG
        float spec_multiplier = pow(max(dot(view_direction, reflection_direction), 0.0), 32);
    #endif
    #ifdef BLINNPHONG
        float spec_multiplier = pow(max(dot(normal, halfway_direction), 0.0), 16);
    #endif
    vec3 specular = light.specular * spec_multiplier * SpecularTex.bbb;

    // Attenuation
    float max_distance = 1.5;
    float distance = length(light.position - FragPos);
    //float attenuation = 1.0 / (distance);

    //diffuse *= attenuation;
    //specular *= attenuation;

    // Result
    vec3 result = ambient + diffuse + specular;

    // Gamma correction
    //result *= pow(result, vec3(1.0/2.2));

    FragColor = vec4(result, 1.0);
}