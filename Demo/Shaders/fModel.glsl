#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

void main()
{
    vec4 SpecularTex = texture(texture_specular1, TexCoords);
    vec4 NormalTex = texture(texture_normal1, TexCoords);
    vec4 HeightTex = texture(texture_height1, TexCoords);
    FragColor = texture(texture_diffuse1, TexCoords);
}