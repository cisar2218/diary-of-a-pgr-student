#version 330 core

smooth in vec4 Color;
smooth in vec2 TexCoord;
smooth in float Visibility;

uniform sampler2D tex;
uniform bool texEnabled;

out vec4 FragColor;

void main()
{
    vec4 finalColor;
    if (texEnabled)
        finalColor = Color * texture(tex, TexCoord);
    else
        finalColor = Color;
    FragColor = mix(vec4(0.631, 0.051, 0.243, 1.0), finalColor, Visibility);
}
