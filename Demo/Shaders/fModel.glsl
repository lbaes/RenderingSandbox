#version 450 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

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
    float diffuse_multiplier = max(dot(normal, light_direction), 0.0);

    // Final color
    vec3 diffuse = diffuse_multiplier * light.diffuse * vec3(texture(material.diffuse1, TexCoords));
    vec3 ambient = light.ambient * vec3(texture(material.diffuse1, TexCoords));

    FragColor = vec4(diffuse, 1.0);
}