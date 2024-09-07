#if !defined(BASIC_GLFW)
#define BASIC_GLFW

#include <glew/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

GLFWwindow* glfwInit(int majorVersion, int minorVersion, int width, int height, bool isFullScreen, const char* title);

#endif // BASIC_GLFW
