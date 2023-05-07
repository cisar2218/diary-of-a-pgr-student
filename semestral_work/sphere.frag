#version 330 core

smooth in vec4 Color;
smooth in vec2 TexCoord;

uniform sampler2D tex;

out vec4 FragColor;

void main()
{
    FragColor = Color;

    // TODO use texture if any
    FragColor = Color * texture(tex, TexCoord);
}
