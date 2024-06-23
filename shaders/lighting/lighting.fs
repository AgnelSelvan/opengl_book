#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct TextureMaterial {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

uniform vec3 viewPos;
uniform TextureMaterial material;
uniform Light light;
uniform float time;

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

void main()
{

    vec3 lightDir = normalize(light.position - fragPos);

    // vec3 ambient = light.ambient * material.ambient;
    vec3 ambient = light.ambient * texture(material.diffuse, texCoords).rgb;

    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.0);
    // vec3 diffuse = (light.diffuse * diff *  material.diffuse);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, texCoords).rgb;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specularMap = vec3(texture(material.specular, texCoords));
    vec3 specular = light.specular * specularMap * spec;

    //emission
    // vec3 emission = texture(material.emission, texCoords).rgb;
    vec3 emission = vec3(texture(material.emission, texCoords));

    //Spotlight
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    // float intensity = clamp((theta - light.outerCutOff)/epsilon, 0.0, 1.0);
    float intensity = smoothstep(0.0, 1.0, (theta - light.outerCutOff)/epsilon);
    diffuse *= intensity;
    specular *= intensity;

    // Point Light
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient *= attenuation;
    specular *= attenuation;
    diffuse *= attenuation;

    vec3 result = (ambient + diffuse + specular + emission);
    FragColor = vec4(result, 1.0);

}