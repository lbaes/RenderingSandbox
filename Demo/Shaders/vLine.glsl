#version 450 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 color;

out vec4 Color;

void main()
{
    Color = color;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}