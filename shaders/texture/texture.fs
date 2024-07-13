#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{
    vec3 texColor = vec3(texture(texture1, TexCoords));
    FragColor = vec4(texColor, 1.0);
}