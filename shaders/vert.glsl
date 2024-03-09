#version 410 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 vertexColors;
layout(location=2) in vec2 textCord;

out vec3 v_vertexColors;
out vec2 v_textCord;

uniform mat4 uModelMatrix; // Uniform Variable
uniform mat4 uPerspectiveMatrix; // Uniform Variable
uniform mat4 uViewMatrix; // Uniform Variable

void main(){
    v_vertexColors = vertexColors;
    v_textCord = textCord;
    vec4 newPosition = uPerspectiveMatrix * uViewMatrix * uModelMatrix * vec4(position, 1.0f);
    gl_Position = newPosition;
}