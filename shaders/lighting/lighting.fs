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

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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
    // vec3 ambient = light.ambient * material.ambient;
    vec3 ambient = light.ambient * texture(material.diffuse, texCoords).rgb;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
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

    vec3 result = (ambient + diffuse + specular + emission);
    FragColor = vec4(result, 1.0);
}