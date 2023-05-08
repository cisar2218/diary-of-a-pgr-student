#version 330 core

smooth in vec2 TexCoord;

uniform sampler2D tex;
uniform bool texEnabled;

out vec4 FragColor;

void main()
{
    if (texEnabled)
        FragColor = texture(tex, TexCoord);
    else
        FragColor = vec4(1.0, 1.0, 1.0, 0.0);
}
