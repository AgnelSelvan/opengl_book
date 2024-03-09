#version 410 core

in vec3 v_vertexColors;
in vec2 v_textCord;

out vec4 color;

uniform float uOffset; // Uniform Variable
uniform sampler2D ourTexture;

void main(){
    // color = texture(ourTexture, v_textCord) * vec4(v_vertexColors.r + uOffset, v_vertexColors.g, v_vertexColors.b, 1.0f);
    color = texture(ourTexture, v_textCord);
}