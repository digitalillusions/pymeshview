#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 lightPos;
// uniform vec3 lightColor;
// uniform vec3 objectColor;


void main(){
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 objectColor = vec3(0.8, 0.8, 0.8);

    float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * lightColor;

    float diffuseStrength = 0.8;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(viewPos - FragPos);
    // float diffuseIntensity = max(dot(norm, lightDir), 0.0);
    float diffuseIntensity = abs(dot(norm, lightDir));
    vec3 diffuse = diffuseIntensity * diffuseStrength * lightColor;

    float specularStrength = 0.2;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float specularIntensity = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularIntensity * specularStrength * lightColor;

    vec3 result = objectColor * (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}
