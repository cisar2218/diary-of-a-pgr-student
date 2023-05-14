#version 330 core

in vec3 aPos;
in vec3 aNormal;
in vec2 aTexCoord;

out vec2 TexCoord;
out float Visibility;
out vec3 Position_eye;
out vec3 Normal_eye;

uniform mat4 PVM;
uniform mat4 Vmatrix;
uniform mat4 Mmatrix;
uniform mat4 Nmatrix;

// Fog settings
bool fogEnabled = true;
const float density = 0.015;
const float gradient = 1.5;


float getVisibility(float distance) {
    float ret = exp(-pow((distance * density), gradient));
    return clamp(ret, 0.0, 1.0);
}

void main()
{
    Position_eye = (Vmatrix * Mmatrix * vec4(aPos, 1.0)).xyz;
    Normal_eye = normalize( (Vmatrix * Nmatrix * vec4(aNormal, 0.0) ).xyz);
  
    if(fogEnabled)  {
        Visibility = getVisibility(length(Position_eye));
    } else {
        Visibility = 1.0;
    }

    gl_Position = PVM * vec4(aPos, 1);
    TexCoord = aTexCoord;
}
