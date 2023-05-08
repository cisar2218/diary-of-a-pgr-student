#version 330 core

in vec3 position;
in vec2 texCoord;

uniform mat4 PVM;

out vec2 TexCoord;

void main()
{
    gl_Position = PVM * vec4(position, 1);
    TexCoord = texCoord;
}
