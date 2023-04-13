#version 330 core

in vec3 Normal;
in vec2 TexCoord;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(Normal);

    // Ambient lighting
    vec3 ambient = material.ambient * 0.1;

    // Diffuse lighting
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.5));
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = material.diffuse * diff;

    // Specular lighting
    //vec3 viewDir = normalize(viewPos - vec3(gl_FragCoord));
    //vec3 reflectDir = reflect(-lightDir, norm);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //vec3 specular = material.specular * spec;

    // Combine all components
    vec3 finalColor = ambient + diffuse;// + specular;

    FragColor = vec4(finalColor, 1.0);
}
