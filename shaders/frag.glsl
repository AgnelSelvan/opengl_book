#version 410 core

in vec3 v_vertexColors;

out vec4 color;

uniform float uOffset; // Uniform Variable

void main(){
    color = vec4(v_vertexColors.r + uOffset, v_vertexColors.g, v_vertexColors.b, 1.0f);
}