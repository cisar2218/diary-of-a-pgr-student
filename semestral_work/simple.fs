

#version 330 core

in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 ambient = vec3(0.1, 0.1, 0.1);
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.5));
    vec3 diffuse = vec3(1.0, 1.0, 1.0) * max(dot(norm, lightDir), 0.0);

    
    vec3 finalColor = (ambient + diffuse);

    FragColor = vec4(finalColor, 1.0);
}
