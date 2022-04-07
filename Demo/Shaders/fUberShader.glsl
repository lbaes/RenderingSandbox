#define NR_POINT_LIGHTS 10

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec3 ViewPos;

struct Material {
    sampler2D diffuse1;
    sampler2D specular1;
    sampler2D normal1;
    vec4 color;
};

#if defined(USE_NORMAL_MAP)

in TangentPointLight
{
    vec3 DirectionalLightTangent;
    vec3 FragPosTangent;
    vec3 ViewPosTangent;
    vec3 LightPosTangent[NR_POINT_LIGHTS];
} tangentPointLight;

#endif

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform PointLight light[NR_POINT_LIGHTS];
uniform DirectionalLight directional_light;

vec3 calDirectionalLight(DirectionalLight dl, vec3 lightDirection, vec3 fragPos, vec3 viewPos, vec4 diffuse_texel, vec3 specular_texel, vec3 normal ){
    vec3 light_direction = normalize(-lightDirection);
    vec3 view_direction = normalize(viewPos - fragPos);
    vec3 halfway_direction = normalize(light_direction + view_direction);

    float diffuse_multiplier = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = diffuse_multiplier * dl.diffuse * diffuse_texel.rgb;

    vec3 ambient = dl.ambient * diffuse_texel.rgb;

    float spec_multiplier = pow(max(dot(normal, halfway_direction), 0.0), 128);
    vec3 specular = dl.specular * spec_multiplier * specular_texel;

    vec3 result = ambient + diffuse + specular;
    return result;
}

vec3 calcPointLight(PointLight pl, vec3 lightPos, vec3 normal, vec3 fragPos, vec3 viewPos, vec4 diffuse_texel, vec3 specular_texel){
    // Light
    vec3 light_direction = normalize(lightPos - fragPos);
    vec3 view_direction = normalize(viewPos - fragPos);
    vec3 halfway_direction = normalize(light_direction + view_direction);
    float distance = length(lightPos - fragPos);

    float diffuse_multiplier = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = diffuse_multiplier * pl.diffuse * diffuse_texel.rgb;

    // Ambient
    #if defined(USE_AMBIENT_LIGHT) && defined(USE_DIFFUSE_TEXTURE)
    vec3 ambient = pl.ambient * diffuse_texel.rgb;
    #elif defined(USE_AMBIENT_LIGHT) && defined(USE_DIFFUSE_COLOR)
    vec3 ambient = pl.ambient * material.color.rgb;
    #else
    vec3 ambient = vec3(0.0f);
    #endif

    float spec_multiplier = pow(max(dot(normal, halfway_direction), 0.0), 128);
    vec3 specular = pl.specular * spec_multiplier * specular_texel;

    // Attenuation
    float attenuation = 1.0 / (pl.constant + pl.linear * distance + pl.quadratic * (distance * distance));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    // Result
    vec3 result = ambient + diffuse + specular;

    #if defined(USE_NORMAL_MAP) && defined(DEBUG_NORMAL_MAP)
    result = mix (light_direction.rgb, diffuse_texel.rgb, 0.0f);
    #endif

    return result;
}

void main()
{

    // Diffuse
    #if defined(USE_DIFFUSE_TEXTURE)
    vec4 diffuse_texel = texture(material.diffuse1, TexCoords);
    if (diffuse_texel.a < 0.1) discard;
    #elif defined(USE_DIFFUSE_COLOR)
    vec4 diffuse_texel = material.color;
    #endif

    // Specular
    #if defined(USE_SPECULAR_MAP)
    vec3 specular_texel = diffuse_texel.rgb * texture(material.specular1, TexCoords).bbb;
    #else
    vec3 specular_texel = vec3(0.0f);
    #endif

    // Normals
    #if defined(USE_NORMAL_MAP)
    vec3 NormalTex = texture(material.normal1, TexCoords).rgb;
    vec3 normal = normalize(NormalTex * 2.0f - 1.0f);
    vec3 fragPos = tangentPointLight.FragPosTangent;
    vec3 viewPos = tangentPointLight.ViewPosTangent;
    vec3 lightDirection = tangentPointLight.DirectionalLightTangent;
    #else
    vec3 normal = normalize(Normal);
    vec3 fragPos = FragPos;
    vec3 viewPos = ViewPos;
    vec3 lightDirection = directional_light.direction;
    #endif

    vec3 result = vec3(0.0f);
    result += calDirectionalLight(directional_light, lightDirection, fragPos, viewPos, diffuse_texel, specular_texel, normal);
    for (int i = 0; i < NR_POINT_LIGHTS; ++i){
        #if defined(USE_NORMAL_MAP)
        result += calcPointLight(light[i], tangentPointLight.LightPosTangent[i], normal, fragPos, viewPos, diffuse_texel, specular_texel);
        #else
        result += calcPointLight(light[i], light[i].position, normal, fragPos, viewPos, diffuse_texel, specular_texel);
        #endif
    }
    FragColor = vec4(result, 1.0);
}