#version 330 core

smooth in vec4 Color;
smooth in vec2 TexCoord;


out vec4 FragColor;

void main()
{
    FragColor = Color;

    // TODO use texture if any
}
