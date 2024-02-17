#include<iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main() { 
  
  glfwInit();
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow* window = glfwCreateWindow(500, 300, "Hello World", NULL, NULL);
  if(window == NULL){
    std::cout << "GLFW Window create error" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  gladLoadGL();

  glViewport(0, 0, 500, 300);
  glClearColor(1.0f, 1.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);

  glfwSwapBuffers(window);

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
  }
  

  glfwDestroyWindow(window);
  glfwTerminate();

  
  return 0;
}