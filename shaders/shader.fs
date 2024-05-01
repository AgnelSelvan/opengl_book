#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 textureCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    vec2 coord = textureCoord;
    float sinFactor = sin(135);
    float cosFactor = cos(135);
    coord = (coord - 0.5) * mat2(cosFactor, sinFactor, -sinFactor, cosFactor);
    coord += 0.5;

    FragColor = mix(texture(texture1, textureCoord), texture(texture2, coord), vec4(ourColor, 1.0));

}