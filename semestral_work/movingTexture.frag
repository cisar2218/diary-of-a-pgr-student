#version 330 core

smooth in vec4 Color;
smooth in vec2 TexCoord;

uniform sampler2D tex;
uniform bool texEnabled;

uniform vec2 scrollSpeed;
uniform float elapsedTime;

out vec4 FragColor;

void main()
{
    if (!texEnabled) {
        FragColor = Color;
        return;
    }
    
    vec2 scrollOffset = mod(scrollSpeed * elapsedTime, vec2(1.0));

    FragColor = Color * texture(tex, TexCoord + scrollOffset);
}
