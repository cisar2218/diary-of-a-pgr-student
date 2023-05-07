#version 330 core

in vec3 aPos;
in vec3 aNormal;
in vec2 aTexCoord;

uniform mat4 PVM;

out vec2 TexCoord;
out vec4 Color;

uniform mat4 Vmatrix;       // View                       --> world to eye coordinates
uniform mat4 Mmatrix;       // Model                      --> model to world coordinates
uniform mat4 Nmatrix;       // inverse transposed Mmatrix

bool dirLight = true;
bool pointLight = false;
bool spotLight = false;


struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

vec4 getSpotLight(vec3 vertexPosition, vec3 vertexNormal)  {
    vec3 ret = vec3(0.0);
    vec3 lightPosition = (Vmatrix * vec4(2.0, 0.0, 0.0, 0.0)).xyz;

    Material lightMaterial;
    lightMaterial.ambient  = vec3(0.0);
    lightMaterial.diffuse  = vec3(1.0, 1.0, 0.5f);
    lightMaterial.specular = vec3(1.0);
    
    vec3 light_norm = normalize(lightPosition);
    vec3 light_reflection = reflect(-light_norm, vertexNormal);
    vec3 viewer = normalize(-vertexPosition);

    // new 
    vec3 spotDirection = vec3(-1.0,0.0,0.0);
    float spotCoef = max(0.0, dot(-light_norm, spotDirection));
    float spotCutOff = 0.95;
    float spotExponent = 0.0;

    ret += material.ambient * lightMaterial.ambient;
    ret += material.diffuse * lightMaterial.diffuse * max(0.0, dot(vertexNormal, light_norm));
    ret += material.specular * lightMaterial.specular * pow(max(0.0, dot(light_reflection, viewer)), material.shininess);


    if (spotCoef < spotCutOff) {
        ret *= 0.0;
    } else {
        ret *= pow(spotCoef, spotExponent);
    }

    return vec4(ret, 1.0);
}

vec4 getPointLight(vec3 vertexPosition, vec3 vertexNormal)  {
    vec3 ret = vec3(0.0);
    vec3 lightPosition = (Vmatrix * vec4(3.0, 0.0, 0.0, 0.0)).xyz;

    Material lightMaterial;
    lightMaterial.ambient  = vec3(0.0);
    lightMaterial.diffuse  = vec3(1.0, 1.0, 0.5f);
    lightMaterial.specular = vec3(1.0);

    float dist = length(lightPosition - vertexPosition);
    float atten_const = 1.0;
    float atten_linear = 0.2; 
    float atten_exp = 0.0; 
    
    float atten = 1 / (atten_const + atten_linear * dist + atten_exp * dist * dist);

    vec3 light_norm = normalize(lightPosition);
    vec3 light_reflection = reflect(-light_norm, vertexNormal);
    vec3 viewer = normalize(-vertexPosition);

    ret += material.ambient * lightMaterial.ambient;
    ret += material.diffuse * lightMaterial.diffuse * max(0.0, dot(vertexNormal, light_norm));
    ret += material.specular * lightMaterial.specular * pow(max(0.0, dot(light_reflection, viewer)), material.shininess);

    return vec4(ret * atten, 1.0);
}

vec4 getDirectionalLight(vec3 vertexPosition, vec3 vertexNormal) {
    
    Material lightMaterial;
    lightMaterial.ambient  = vec3(0.0);
    lightMaterial.diffuse  = vec3(1.0, 1.0, 0.5f);
    lightMaterial.specular = vec3(1.0);

    vec3 lightPosition = (Vmatrix * vec4(-1.0, 1.0, 0.0, 0.0)).xyz;

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

    if (dirLight) outputColor += getDirectionalLight(position_eye, normal_eye);
    if (pointLight) outputColor += getPointLight(position_eye, normal_eye);
    if (spotLight) outputColor += getSpotLight(position_eye, normal_eye);
   
    gl_Position = PVM * vec4(aPos, 1);
    Color = outputColor;
    TexCoord = aTexCoord;
}
