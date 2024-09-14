// #version 330 core
// out vec4 FragColor;

// in vec2 TexCoords;

// uniform sampler2D texture1;

// void main()
// {
//     FragColor = texture(texture1, TexCoords);
// }

#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D texture1;

uniform vec2 screenResolution;

void main()
{
    // waving effect
    vec2 wavecoord = TexCoords;
    wavecoord.x += sin(wavecoord.y * 9*2*3.14159 + 200) / 100;

    FragColor = texture(texture1, wavecoord);
}