//#define DEBUG_NORMAL_MAP
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec3 ViewPos;
#if defined(USE_NORMAL_MAP)
in vec3 FragPosTangent;
in vec3 ViewPosTangent;
in vec3 LightPosTangent;
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
    // Diffuse
    #if defined(USE_DIFFUSE_TEXTURE)
    vec4 DiffuseTex = texture(material.diffuse1, TexCoords);
    if (DiffuseTex.a < 0.1) discard;
    #elif defined(USE_DIFFUSE_COLOR)
    vec4 DiffuseTex = material.color;
    #endif

    // Normals
    #if defined(USE_NORMAL_MAP)
    vec3 NormalTex = texture(material.normal1, TexCoords).rgb;
    vec3 normal = normalize(NormalTex * 2.0f - 1.0f);
    vec3 light_direction = normalize(LightPosTangent - FragPosTangent);
    vec3 view_direction = normalize(ViewPosTangent - FragPosTangent);
    float distance = length(LightPosTangent - FragPosTangent);
    #else
    vec3 normal = normalize(Normal);
    vec3 light_direction = normalize(light.position - FragPos);
    vec3 view_direction = normalize(ViewPos - FragPos);
    float distance = length(light.position - FragPos);
    #endif

    vec3 halfway_direction = normalize(light_direction + view_direction);
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
    vec3 SpecularTex = DiffuseTex.rgb * texture(material.specular1, TexCoords).bbb;
    #else
    vec3 SpecularTex = vec3(0.0f);
    #endif

    float spec_multiplier = pow(max(dot(normal, halfway_direction), 0.0), 16);
    vec3 specular = light.specular * spec_multiplier * SpecularTex;

    // Attenuation
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    // Result
    vec3 result =  ambient + diffuse + specular;

    #if defined(USE_NORMAL_MAP) && defined(DEBUG_NORMAL_MAP)
    result = mix (light_direction.rgb, DiffuseTex.rgb, 0.0f);
    #endif

    FragColor = vec4(result, 1.0);
}