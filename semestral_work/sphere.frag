// TODO review all light duplicate function in all frag shaders (replace with new version to be sure)

#version 330 core

smooth in vec2 TexCoord;
smooth in float Visibility;
in vec3 Position_eye;
in vec3 Normal_eye;

out vec4 FragColor;

uniform bool texEnabled;
uniform sampler2D tex;

uniform mat4 Vmatrix;
uniform mat4 Mmatrix;
uniform mat4 Nmatrix;

struct Light {
  vec3  ambient;
  vec3  diffuse;
  vec3  specular;
  vec3  position;
  vec3  direction;
};

uniform Light spotLight;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

// Light settings
bool dirLightEnabled = true;
bool pointLightEnabled = false;
bool spotLightEnabled = true;


vec4 getDirectionalLight(vec3 vertexPosition, vec3 vertexNormal) {
    
    vec3 ret = vec3(0.0);
    vec3 lightPosition = (Vmatrix * vec4(-1.0, 5.0, 5.0, 0.0)).xyz;

    vec3 light_norm = normalize(lightPosition);
    vec3 light_reflection = reflect(-light_norm, vertexNormal);
    vec3 viewer = normalize(-vertexPosition);

    ret += material.ambient * spotLight.ambient;
    ret += material.diffuse * spotLight.diffuse * max(0.0, dot(vertexNormal, light_norm));
    ret += material.specular * spotLight.specular * pow(max(0.0, dot(light_reflection, viewer)), material.shininess);

    return vec4(ret, 1.0);
}

vec4 getPointLight(vec3 vertexPosition, vec3 vertexNormal) {
    
    vec3 ret = vec3(0.0);
    vec3 lightPosition = (Vmatrix * vec4(spotLight.position, 1.0)).xyz;

    vec3 light_norm = normalize(lightPosition - vertexPosition);
    vec3 light_reflection = reflect(-light_norm, vertexNormal);
    vec3 viewer = normalize(-vertexPosition);
	
    ret += material.ambient * spotLight.ambient;
    ret += material.diffuse * spotLight.diffuse * max(0.0, dot(vertexNormal, light_norm));
    ret += material.specular * spotLight.specular * pow(max(0.0, dot(light_reflection, viewer)), material.shininess);

    float distFromLight = distance(lightPosition, vertexPosition);
    float attenConst = 1.0;
    float attenLin = 0.5;
    float attenQuad = 0.1;  // TODO set to ~1.5
    
    float attenFactor = 1.0 / (attenConst + attenLin * distFromLight 
        + attenQuad * distFromLight*distFromLight);

    return vec4(ret, 1.0);
}

vec4 getSpotLight(vec3 vertexPosition, vec3 vertexNormal) {
    vec3 lightPosition = (Vmatrix * vec4(spotLight.position, 1.0)).xyz;
    vec3 ret = vec3(0.0);

    vec3 light_norm = normalize(lightPosition - vertexPosition);
    vec3 light_reflection = reflect(-light_norm, vertexNormal); // smer odrazu
    vec3 viewer = normalize(-vertexPosition);
    
    vec3 spotDir = normalize((Vmatrix * vec4(spotLight.direction,0.0)).xyz);
    float dotAngle = dot(spotDir, -light_norm);

    ret += material.ambient * spotLight.ambient;
	ret += material.diffuse * spotLight.diffuse * max(dot(vertexNormal, viewer), 0);
	ret += material.specular * spotLight.specular * pow( max(dot(light_reflection,viewer),0), material.shininess );
    
    float distFromLight = distance(lightPosition, vertexPosition);
    float attenConst = 1.0;
    float attenLin = 0.5;
    float attenQuad = 1.5;
    
    float spotCoef = max(0.0, dot(-light_norm, spotDir));

    float cutoff = 0.85; // TODO
    if (spotCoef < cutoff) {
        ret*= 0.0;
    } else {
        float spotExponent = 1.5; // TODO
        ret *= pow(spotCoef, spotExponent);
    }
    return vec4(ret, 1.0);
 }


void main()
{
    vec3 globalAmbientLight = vec3(0.6);
    vec4 finalColor = vec4(material.ambient * globalAmbientLight, 1.0);

    if (dirLightEnabled) {
        finalColor += getDirectionalLight(Position_eye, Normal_eye);
    }
    if (pointLightEnabled) {
        finalColor += getPointLight(Position_eye, Normal_eye);
    }
    if (spotLightEnabled) {
        finalColor += getSpotLight(Position_eye, Normal_eye);
    }

    if (texEnabled)
        finalColor = finalColor * texture(tex, TexCoord);
    else
        finalColor = finalColor;

    //FragColor = finalColor;
    FragColor = mix(vec4(0.631, 0.051, 0.243, 1.0), finalColor, Visibility);
}
