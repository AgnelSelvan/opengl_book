#version 330 core
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos; 

in vec3 normal;
in vec3 fragPos;


void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = lightColor * objectColor;
    FragColor = vec4(result, 1.0);
}