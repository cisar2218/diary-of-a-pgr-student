#version 330 core

smooth in vec4 Color;
smooth in vec2 TexCoord;

uniform sampler2D tex;
uniform bool texEnabled;

out vec4 FragColor;

void main()
{
    if (texEnabled)
        FragColor = Color * texture(tex, TexCoord);
    else
        FragColor = Color;
}
