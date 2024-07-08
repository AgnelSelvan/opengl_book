#define STB_IMAGE_IMPLEMENTATION
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <list>
#include "shader_h.h"
#include "camera.h"
#include "model.h"

void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool firstMouse = true;

float deltaTime = 0.f;
float lastFrame = 0.f;

float lastX =  SCR_WIDTH / 2.0;
float lastY =  SCR_HEIGHT / 2.0;

bool mouseCanDrag = true;

GLenum getPolygon(int polygonModeInt){
    switch (polygonModeInt)
    {
    case 0:
        return GL_LINE;
    case 1:
        return GL_FILL;
    case 2:
        return GL_POINT;
    default:
        return GL_FILL;
    }
}

int main()
{
    //Variables
    glm::vec3 modelTranslate(0.0f, 0.0f, 0.0f);
    static int polygonModeInt = 0;
    glm::vec3 tankRotate(0.0f, 0.0f, 0.0f);
    float rotateDegree = 0.0f;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    SCR_WIDTH = mode->width;
    SCR_HEIGHT = mode->height;

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Playground", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader lightingShader("./shaders/lighting/lighting.vs", "./shaders/lighting/lighting.fs");

    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);
    Model tankModel(std::filesystem::path("assets/models/tank/14077_WWII_Tank_Germany_Panzer_III_v1_L2.obj"));
    Model backpackModel(std::filesystem::path("assets/models/backpack/backpack.obj"));

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        if(ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem)){
            mouseCanDrag = false;
        }else{
            mouseCanDrag = true;
        }

        lightingShader.use();
        lightingShader.setInt("material.diffuse", 0);
        lightingShader.setInt("material.specular", 1);
        lightingShader.setInt("material.emission", 2);

        lightingShader.setVec3("objectColor", 1.f, 0.84f, 0.f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("viewPos", camera.position.x, camera.position.y, camera.position.z);

        // lightingShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
        lightingShader.setFloat("time", currentFrame);


        // modelShader.use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(modelTranslate.x, modelTranslate.y, modelTranslate.z));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        lightingShader.setMat4("model", model);
        backpackModel.draw(lightingShader, getPolygon(polygonModeInt));

        glm::mat4 tankModelMatrix = glm::mat4(1.0f);
        tankModelMatrix = glm::translate(tankModelMatrix, glm::vec3(-4.5f, -6.5f, -16.5f));
        tankModelMatrix = glm::rotate(tankModelMatrix, -1.63f, glm::vec3(40.f, 0.f, 0.f));
        tankModelMatrix = glm::scale(tankModelMatrix, glm::vec3(6.5f, 6.5f, 6.5f));
        lightingShader.setMat4("model", tankModelMatrix);
        tankModel.draw(lightingShader, getPolygon(polygonModeInt));


        // directional light
        lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        glm::vec3 pointLightPosition = pointLightPositions[0];
        lightingShader.setVec3("pointLights[0].position", pointLightPosition.x, pointLightPosition.y, pointLightPosition.z);
        lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[0].constant", 1.0f);
        lightingShader.setFloat("pointLights[0].linear", 0.09f);
        lightingShader.setFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        pointLightPosition = pointLightPositions[1];
        lightingShader.setVec3("pointLights[1].position", pointLightPosition.x, pointLightPosition.y, pointLightPosition.z);
        lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[1].constant", 1.0f);
        lightingShader.setFloat("pointLights[1].linear", 0.09f);
        lightingShader.setFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        pointLightPosition = pointLightPositions[2];
        lightingShader.setVec3("pointLights[2].position", pointLightPosition.x, pointLightPosition.y, pointLightPosition.z);
        lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[2].constant", 1.0f);
        lightingShader.setFloat("pointLights[2].linear", 0.09f);
        lightingShader.setFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        pointLightPosition = pointLightPositions[3];
        lightingShader.setVec3("pointLights[3].position", pointLightPosition.x, pointLightPosition.y, pointLightPosition.z);
        lightingShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[3].constant", 1.0f);
        lightingShader.setFloat("pointLights[3].linear", 0.09f);
        lightingShader.setFloat("pointLights[3].quadratic", 0.032f);
        // spotLight
        lightingShader.setVec3("spotLight.position", camera.position.x, camera.position.y, camera.position.z);
        lightingShader.setVec3("spotLight.direction", camera.front.x, camera.front.y, camera.front.z);
        lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("spotLight.constant", 1.0f);
        lightingShader.setFloat("spotLight.linear", 0.09f);
        lightingShader.setFloat("spotLight.quadratic", 0.032f);
        lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        // directional light
        lightingShader.setVec3("directionalLight.direction", -0.2f, -1.0f, -0.3f);
        lightingShader.setVec3("directionalLight.ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("directionalLight.diffuse", 0.4f, 0.4f, 0.4f);
        lightingShader.setVec3("directionalLight.specular", 0.5f, 0.5f, 0.5f);


        // view/projection transformations
        projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = camera.getViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        ImGui::Begin("Change Settings");
        ImGui::Text("Model Position");
        ImGui::SliderFloat("X", &modelTranslate.x, -100.f, 100.f);
        ImGui::SliderFloat("Y", &modelTranslate.y, -100.f, 100.f);
        ImGui::SliderFloat("Z", &modelTranslate.z, -100.f, 100.f);

        ImGui::Text("Polygon Mode");
        ImGui::RadioButton("Line", &polygonModeInt, 0); ImGui::SameLine();
        ImGui::RadioButton("Fill", &polygonModeInt, 1); ImGui::SameLine();
        ImGui::RadioButton("Point", &polygonModeInt, 2);

        ImGui::Text("Tank Rotate");
        ImGui::SliderFloat("X", &tankRotate.x, -100.f, 100.f);
        ImGui::SliderFloat("Y", &tankRotate.y, -100.f, 100.f);
        ImGui::SliderFloat("Z", &tankRotate.z, -100.f, 100.f);
        ImGui::SliderFloat("Degree", &rotateDegree, -100.f, 100.f);

        ImGui::SameLine();

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}



void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(RIGHT, deltaTime);

}

void mouseCallback(GLFWwindow* window, double xPosIn, double yPosIn){
    if(!mouseCanDrag){
        return;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        firstMouse = true;
        return;
    }
    float xPos = static_cast<float>(xPosIn);
    float yPos = static_cast<float>(yPosIn);

    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos; // reversed since y-coordinates go from bottom to top

    lastX = xPos;
    lastY = yPos;

    camera.processMouseMovement(xOffset, yOffset);
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    camera.processMouseScroll(static_cast<float>(yOffset));
}