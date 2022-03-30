#version 450 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec3 ViewPos;

struct Material {
    sampler2D diffuse1;
    sampler2D specular1;
    sampler2D height1;
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
    vec4 HeightTex = texture(material.height1, TexCoords);
    vec4 AmbientTex = texture(material.height1, TexCoords);

    // Lighting
    vec3 normal = normalize(Normal);
    vec3 light_direction = normalize(light.position - FragPos);
    vec3 view_direction = normalize(ViewPos - FragPos);
    vec3 reflection_direction = reflect(-light_direction, normal);

    // Diffuse
    float diffuse_multiplier = max(dot(normal, light_direction), 0.0);
    vec3 diffuse = diffuse_multiplier * light.diffuse * DiffuseTex.rgb;

    // Specular
    float spec_multiplier = pow(max(dot(view_direction, reflection_direction), 0.0), 32);
    vec3 specular = light.specular * spec_multiplier * SpecularTex.bbb;

    // Final colors
    vec3 ambient = light.ambient * DiffuseTex.rgb;

    // Result
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}