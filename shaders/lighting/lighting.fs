#version 330 core
#define NR_POINT_LIGHTS 4

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
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

struct DirectionalLight{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;
// uniform Light light;
uniform float time;
uniform DirectionalLight directionalLight;
uniform SpotLight spotLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

vec4 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);

vec4 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec4 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

float near = 0.1;
float far = 100.0;

float linearizeDepth(float depth){
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));
}

float logisticDepth(float depth, float stepness, float offset){
    float zValue = linearizeDepth(depth);
    return 1.0 / (1.0 + exp(-stepness * (zValue - offset / 2.0)));
}

void main()
{
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec4 result = calculateDirectionalLight(directionalLight, norm, viewDir);
    for(int i = 0; i < NR_POINT_LIGHTS; i++){
        result += calculatePointLight(pointLights[i], norm, fragPos, viewDir);
    }
    result += calculateSpotLight(spotLight, norm, fragPos, viewDir);

    float depth = logisticDepth(gl_FragCoord.z, 0.1f,  5.0f);
    FragColor = result * (1.0f - depth) + vec4(depth * vec3(0.85f, 0.85f, 0.90f), 1.0);
}

vec4 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Point Light
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec4 ambient  = vec4(light.ambient, 1.0f)  * texture(material.diffuse, texCoords);
    vec4 diffuse  = vec4(light.diffuse, 1.0f)  * diff * texture(material.diffuse, texCoords);
    vec4 specular = vec4(light.specular, 1.0f) * spec * texture(material.specular, texCoords);

    ambient *= attenuation;
    specular *= attenuation;
    diffuse *= attenuation;

    return (ambient + specular + diffuse);
}

vec4 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec4 ambient = vec4(light.ambient, 1.0f) * texture(material.diffuse, texCoords);
    vec4 diffuse = vec4(light.diffuse, 1.0f) * diff * texture(material.diffuse, texCoords);
    vec4 specular = vec4(light.specular, 1.0f) * spec * texture(material.specular, texCoords);

    return (ambient + diffuse + specular);
}


// calculates the color when using a spot light.
vec4 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec4 ambient = vec4(light.ambient, 1.0f) * texture(material.diffuse, texCoords);
    vec4 diffuse = vec4(light.diffuse, 1.0f) * diff * texture(material.diffuse, texCoords);
    vec4 specular = vec4(light.specular, 1.0f) * spec * texture(material.specular, texCoords);
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}