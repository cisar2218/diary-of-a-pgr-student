#version 330 core

in vec3 aPos;
in vec3 aNormal;
in vec2 aTexCoord;

uniform mat4 PVM;

out vec2 TexCoord;
out vec4 Color;

uniform mat4 Vmatrix;       // View                       --> world to eye coordinates
uniform mat4 Mmatrix;       // Model                      --> model to world coordinates
uniform mat4 Nmatrix;  // inverse transposed Mmatrix

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

vec4 getDirectionalLight(vec3 vertexPosition, vec3 vertexNormal) {
    
    Material lightMaterial;
    lightMaterial.ambient  = vec3(0.0);
    lightMaterial.diffuse  = vec3(1.0, 1.0, 0.5f);
    lightMaterial.specular = vec3(1.0);

    vec3 lightPosition = (Vmatrix * vec4(1.0, 0.0, 0.0, 0.0)).xyz;

    vec3 ret = vec3(0.0);

    vec3 light_norm = normalize(lightPosition);
    vec3 light_reflection = reflect(-light_norm, vertexNormal);
    vec3 viewer = normalize(-vertexPosition);

    ret += material.ambient * lightMaterial.ambient;
    ret += material.diffuse * lightMaterial.diffuse * max(0.0, dot(vertexNormal, light_norm));
    ret += material.specular * lightMaterial.specular * pow(max(0.0, dot(light_reflection, viewer)), material.shininess);

    return vec4(ret, 1.0);
}

void main()
{
    vec3 position_eye = (Vmatrix * Mmatrix * vec4(aPos, 1.0)).xyz;
    vec3 normal_eye = normalize( (Vmatrix * Nmatrix * vec4(aNormal, 0.0) ).xyz);

  
    vec3 globalAmbientLight = vec3(0.4f); // TODO global ambient light
    vec4 outputColor = vec4(material.ambient * globalAmbientLight, 0.0);

    outputColor += getDirectionalLight(position_eye, normal_eye);
    //outputColor += spotLight(spaceShipReflector, material, vertexPosition, vertexNormal);

   
    gl_Position = PVM * vec4(aPos, 1);
    Color = outputColor;
    TexCoord = aTexCoord;
}
