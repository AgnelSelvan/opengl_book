#include <stdio.h>

#include "basic_util.h"
#include "basic_glfw.h"

static int glMajorVersion = 0;
static int glMinorVersion = 0;

int getGlMajorVersion(){
    return glMajorVersion;
}

int getGlMinorVersion(){
    return glMinorVersion;
}

int isGLVersionHigher(int majorVersion, int minorVersion){
    return (glMajorVersion >= majorVersion) && (glMinorVersion >= minorVersion);
}

static void glfwLibInit(){
    if(!glfwInit()){
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(1);
    }

    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);

    fprintf(stdout, "GLFW Version: %d.%d.%d\n", major, minor, revision);
}

static void enableDebugOutput(){
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_TRUE);
}

static void initGlew(){
    glewExperimental = GL_TRUE;
    GLenum res = glewInit();
    if(res != GLEW_OK){
        BASIC_ERROR0((const char*)glewGetErrorString(res));
        exit(1);
    }
}

GLFWwindow* glfwInit(int majorVersion, int minorVersion, int width, int height, bool isFullScreen, const char* title){
    glfwLibInit();

    GLFWmonitor* monitor = isFullScreen ? glfwGetPrimaryMonitor() : NULL;

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    if(majorVersion > 0){
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
    }

    if(minorVersion > 0){
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
    }

    GLFWwindow* window = glfwCreateWindow(width, height, title, monitor, NULL);

    if(!window){
        const char* pDesc = NULL;
        int error_code = glfwGetError(&pDesc);

        BASIC_ERROR("Error creating window %s", pDesc);
        exit(1);
    }

    glfwMakeContextCurrent(window);

    glGetIntegerv(GL_MAJOR_VERSION, &glMajorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &glMinorVersion);

    if(majorVersion > 0){
        if(majorVersion != glMajorVersion){
            BASIC_ERROR("Requested major version %d is not the same as created version %d", majorVersion, glMajorVersion);
            exit(0);
        }
    }

    if(minorVersion > 0){
        if(minorVersion != glMinorVersion){
            BASIC_ERROR("Requested minor version %d is not the same as created version %d", minorVersion, glMinorVersion);
            exit(0);
        }
    }

    initGlew();

    enableDebugOutput();

    glfwSwapInterval(1);

    return window;
}







