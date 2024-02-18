#include<iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

int main() { 
  // Initializing GLFW Library
  glfwInit();
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Creating a GLFW Window of width 500 and height 300 with title "Hello World"
  GLFWwindow* window = glfwCreateWindow(500, 300, "Hello World", NULL, NULL);
  // If window creating failed
  if(window == NULL){
    std::cout << "GLFW Window create error" << std::endl;
    glfwTerminate();
    return -1;
  }
  // To make sure all the drawing happing on window variable, so making it as current context
  glfwMakeContextCurrent(window);

  // Load Glad so it configures OpenGL
  gladLoadGL();

  // Creates a Vertex Shader and creating its reference
  GLuint vertextShader = glCreateShader(GL_VERTEX_SHADER);
  // Loads the source code in rhe shader
  glShaderSource(vertextShader, 1, &vertexShaderSource, NULL);
  // Compiles the shader
  glCompileShader(vertextShader);

  // Creates a Fragment Shader and creating its reference
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  // Loads the source code in rhe shader
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  // Compiles the shader
  glCompileShader(fragmentShader);

  // Creates a empty program object
  GLuint shaderProgram = glCreateProgram();
  // Attach the shaders to the program
  glAttachShader(shaderProgram, vertextShader);
  glAttachShader(shaderProgram, fragmentShader);
  // Based on the vertex, fragment or geometry shader attached to the program, it will create a executable that run on respective processor
  glLinkProgram(shaderProgram);

  // Delete the shader once after linking
  glDeleteShader(vertextShader);
  glDeleteShader(fragmentShader);

  // Vertices coordinates
	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f // Upper corner
	};

  GLuint VAO, VBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glViewport(0, 0, 500, 300);
  glClearColor(1.0f, 1.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);

  glfwSwapBuffers(window);

  while (!glfwWindowShouldClose(window))
  {
    glClearColor(1.0f, 1.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);
  

  glfwDestroyWindow(window);
  glfwTerminate();

  
  return 0;
}