
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec3 ViewPos;
#if defined(USE_NORMAL_MAP)
in mat3 TBN;
#endif

struct Material {
    sampler2D diffuse1;
    sampler2D specular1;
    sampler2D normal1;
    vec4 color;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light light;

void main()
{
    // Lighting

    #if defined(USE_NORMAL_MAP)
    vec3 NormalTex = texture(material.normal1, TexCoords).rgb;
    NormalTex.xy = NormalTex.xy * 2.0 - 1.0;
    vec3 normal = normalize(TBN * NormalTex);
    #else
    vec3 normal = normalize(Normal);
    #endif
    vec3 light_direction = normalize(light.position - FragPos);
    vec3 view_direction = normalize(ViewPos - FragPos);
    vec3 halfway_direction = normalize(light_direction + view_direction);

    // Diffuse
    #if defined(USE_DIFFUSE_TEXTURE)
    vec4 DiffuseTex = texture(material.diffuse1, TexCoords);
    #elif defined(USE_DIFFUSE_COLOR)
    vec4 DiffuseTex = material.color;
    #endif

    float diffuse_multiplier = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = diffuse_multiplier * light.diffuse * DiffuseTex.rgb;

    // Ambient
    #if defined(USE_AMBIENT_LIGHT) && defined(USE_DIFFUSE_TEXTURE)
    vec3 ambient = light.ambient * DiffuseTex.rgb;
    #elif defined(USE_AMBIENT_LIGHT) && defined(USE_DIFFUSE_COLOR)
    vec3 ambient = light.ambient * material.color.rgb;
    #else
    vec3 ambient = vec3(0.0f);
    #endif

    // Specular
    #if defined(USE_SPECULAR_MAP)
    vec4 SpecularTex = texture(material.specular1, TexCoords);
    #else
    vec4 SpecularTex = vec4(1.0f);
    #endif

    float spec_multiplier = pow(max(dot(normal, halfway_direction), 0.0), 16);
    vec3 specular = light.specular * spec_multiplier * SpecularTex.bbb;

    // Attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    // Result
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}