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
unsigned int loadTexture(char const * path);

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

float randomNumber(float min, float max){
    return min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
}

int main()
{
    //Variables
    glm::vec3 modelTranslate(0.0f, 0.0f, 0.0f);
    static int polygonModeInt = 1;
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
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    Shader lightingShader("./shaders/lighting/lighting.vs", "./shaders/lighting/lighting.fs");
    Shader textureShader("./shaders/texture/texture.vs", "./shaders/texture/texture.fs");
    Shader outlineShader("./shaders/outline/outline.vs", "./shaders/outline/outline.fs");
    Shader blendShader("./shaders/blend/blend.vs", "./shaders/blend/blend.fs");

    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };


    float planeVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         100.0f, -0.5f,  100.0f,  1.0f, 0.0f,
        -100.0f, -0.5f,  100.0f,  0.0f, 0.0f,
        -100.0f, -0.5f, -100.0f,  0.0f, 1.0f,

         100.0f, -0.5f,  100.0f,  1.0f, 0.0f,
        -100.0f, -0.5f, -100.0f,  0.0f, 1.0f,
         100.0f, -0.5f, -100.0f,  1.0f, 1.0f
    };
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    float grassVertices[] = {
        // positions          // texture Coords
         1.0f, -2.5f,  1.0f,  1.0f, 1.0f,
        -1.0f, -2.5f,  1.0f,  0.0f, 1.0f,
        -1.0f, -2.5f, -1.0f,  0.0f, 0.0f,

         1.0f, -2.5f,  1.0f,  1.0f, 1.0f,
        -1.0f, -2.5f, -1.0f,  0.0f, 0.0f,
         1.0f, -2.5f, -1.0f,  1.0f, 0.0f
    };
    unsigned int grassVAO, grassVBO;
    glGenVertexArrays(1, &grassVAO);
    glGenBuffers(1, &grassVBO);
    glBindVertexArray(grassVAO);
    glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grassVertices), &grassVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    unsigned int floorTexture = loadTexture("assets/images/marble.jpg");
    unsigned int transparentWindow = loadTexture("assets/images/blending_transparent_window.png");
    unsigned int grassTexture = loadTexture("assets/images/grass.png");

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
    Model formula1Model(std::filesystem::path("assets/models/formula1/Formula 1 mesh.obj"));
    Model tankModel(std::filesystem::path("assets/models/tank/14077_WWII_Tank_Germany_Panzer_III_v1_L2.obj"));

    lightingShader.use();

    std::vector<glm::vec3> grassPosition = {};
    for (size_t i = 0; i < 500; i++)
    {
        float x = randomNumber(-50.f, 50.f);
        float z = randomNumber(-50.f, 50.f);
        grassPosition.push_back(glm::vec3(x, -1.0f, z));
    }

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.85f, 0.85f, 0.90f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        if(ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem)){
            mouseCanDrag = false;
        }else{
            mouseCanDrag = true;
        }

        lightingShader.setInt("material.diffuse", 0);
        lightingShader.setInt("material.specular", 1);
        lightingShader.setInt("material.emission", 2);

        lightingShader.setVec3("objectColor", 1.f, 0.84f, 0.f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("viewPos", camera.position.x, camera.position.y, camera.position.z);

        // lightingShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
        lightingShader.setFloat("time", currentFrame);

        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();

        lightingShader.use();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // view/projection transformations
        projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = camera.getViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

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


        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0, -1.0f, 0));
        model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
        model = glm::rotate(model, glm::radians(rotateDegree), glm::vec3(40.f, 0.f, 0.f));
        lightingShader.setMat4("model", model);
        formula1Model.draw(lightingShader, getPolygon(polygonModeInt));

        glm::mat4 tankModelMatrix = glm::mat4(1.0f);
        tankModelMatrix = glm::translate(tankModelMatrix, glm::vec3(-1.5f, -1.5f, -16.5f));
        tankModelMatrix = glm::rotate(tankModelMatrix, -1.63f, glm::vec3(40.f, 0.f, 0.f));
        tankModelMatrix = glm::scale(tankModelMatrix, glm::vec3(1.4f, 1.4f, 1.4f));
        lightingShader.setMat4("model", tankModelMatrix);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        tankModel.draw(lightingShader, getPolygon(polygonModeInt));

        // Blend grass
        blendShader.use();
        textureShader.setMat4("projection", projection);
        textureShader.setMat4("view", view);

        for (size_t i = 0; i < grassPosition.size(); i++)
        {
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glBindVertexArray(grassVAO);
            if(i % 2 == 0){
                glBindTexture(GL_TEXTURE_2D,transparentWindow);
            }else{
                glBindTexture(GL_TEXTURE_2D,grassTexture);
            }
            model = glm::mat4(1.0f);
            model = glm::translate(model, grassPosition[i]);
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
            blendShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        textureShader.use();
        textureShader.setMat4("projection", projection);
        textureShader.setMat4("view", view);

        // floor
        glBindVertexArray(planeVAO);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(100.0f, 1.0f, 100.0f));
        textureShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        outlineShader.use();
        outlineShader.setMat4("projection", projection);
        outlineShader.setMat4("view", view);


        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);
        outlineShader.setMat4("model", tankModelMatrix);
        tankModel.draw(outlineShader, GL_FILL);

        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glEnable(GL_DEPTH_TEST);


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

unsigned int loadTexture(const char* path){
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if(data){
        GLenum format;
        if(nrComponents == 1)
            format = GL_RED;
        else if(nrComponents == 3)
            format = GL_RGB;
        else if(nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }else{
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    return textureID;
}


