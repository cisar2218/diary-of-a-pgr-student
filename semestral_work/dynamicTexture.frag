#version 330 core

smooth in vec4 Color;
smooth in vec2 TexCoord;

uniform sampler2D tex;
uniform bool texEnabled;

uniform int frame; // Current frame index

out vec4 FragColor;

void main()
{
    if (!texEnabled) {
        FragColor = Color;
        return;
    }

    int numColSamples = 4;
    int numRowSamples = 1;

    vec2 offset = vec2(1.0 / float(numColSamples), 1.0 / float(numRowSamples));
    vec2 adjustedTexCoord = vec2(TexCoord.x / numColSamples, TexCoord.y);
    adjustedTexCoord = adjustedTexCoord + vec2(frame % numColSamples, adjustedTexCoord.y) * offset;

    // Set the final color of the fragment
    // Sample the texture using the adjusted texture coordinates
    FragColor = Color * texture(tex, adjustedTexCoord);
}
