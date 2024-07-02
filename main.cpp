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
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

struct Material{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

struct Light{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 position;
};

struct Cube{
    Material material;
    Light light;
    glm::vec3 position;
    uint32_t id;
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

Cube getEmptyCubeObject(){
    Material material;
    material.ambient = glm::vec3(0.0215f, 0.1745f, 0.0215f);
    material.diffuse = glm::vec3(0.07568, 0.61424, 0.07568);
    material.specular = glm::vec3(0.633, 0.727811, 0.633);
    material.shininess = 0.6f;

    Light light;
    light.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    light.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    light.specular = glm::vec3(1.0f, 1.0f, 1.0f);

    Cube cube;
    cube.material = material;
    cube.light = light;
    int index = rand() % 10;

    cube.position = cubePositions[index];
    cube.id = rand() % 100;

    return cube;
}
bool mouseCanDrag = true;

int main()
{
    std::list<Cube> cubeList;

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

    Shader ourShader("./shaders/shader.vs", "./shaders/shader.fs");
    Shader lightingShader("./shaders/lighting/lighting.vs", "./shaders/lighting/lighting.fs");

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    unsigned int cubeVAO, VBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
    //passing postion attribute to vertex shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //passing normal attribute to vertex shader
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    //passing texture attribute to vertex shader
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    //Light Cube
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    int width, height, nrChannels;
    unsigned int diffuseBoxMap;
    glGenTextures(1, &diffuseBoxMap);
    glBindTexture(GL_TEXTURE_2D, diffuseBoxMap);
    unsigned char *data = stbi_load("./assets/images/container2.png",&width, &height, &nrChannels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);

    unsigned int specularBoxMap;
    glGenTextures(1, &specularBoxMap);
    glBindTexture(GL_TEXTURE_2D, specularBoxMap);
    data = stbi_load("./assets/images/container2_specular.png",&width, &height, &nrChannels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);

    unsigned int emissionBoxMap;
    int nWidth, nHeight, nNrChannels;
    glGenTextures(1, &emissionBoxMap);
    glBindTexture(GL_TEXTURE_2D, emissionBoxMap);
    data = stbi_load("./assets/images/emission.png",&nWidth, &nHeight, &nNrChannels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, nWidth, nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);

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

        lightPos.z = 1.f + sin(glfwGetTime()) * 2.f;
        lightPos.y = cos(glfwGetTime())* 2.f ;

        lightingShader.use();
        lightingShader.setInt("material.diffuse", 0);
        lightingShader.setInt("material.specular", 1);
        lightingShader.setInt("material.emission", 2);

        lightingShader.setVec3("objectColor", 1.f, 0.84f, 0.f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("viewPos", camera.position.x, camera.position.y, camera.position.z);
        lightingShader.setVec3("light.position", lightPos.x, lightPos.y, lightPos.z);
        // lightingShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
        lightingShader.setFloat("time", currentFrame);


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

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);

        // directional light
        lightingShader.setVec3("directionalLight.direction", -0.2f, -1.0f, -0.3f);
        lightingShader.setVec3("directionalLight.ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("directionalLight.diffuse", 0.4f, 0.4f, 0.4f);
        lightingShader.setVec3("directionalLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        lightingShader.setVec3("pointLights[0].position", pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[0].constant", 1.0f);
        lightingShader.setFloat("pointLights[0].linear", 0.09f);
        lightingShader.setFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        lightingShader.setVec3("pointLights[1].position", pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
        lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[1].constant", 1.0f);
        lightingShader.setFloat("pointLights[1].linear", 0.09f);
        lightingShader.setFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        lightingShader.setVec3("pointLights[2].position", pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
        lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[2].constant", 1.0f);
        lightingShader.setFloat("pointLights[2].linear", 0.09f);
        lightingShader.setFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        lightingShader.setVec3("pointLights[3].position", pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
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

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        for (auto &&cube : cubeList)
        {
            lightingShader.setFloat("material.shininess", 64.0f);

            // lightingShader.setVec3("light.ambient", cube.light.ambient.x, cube.light.ambient.y, cube.light.ambient.z);
            // lightingShader.setVec3("light.diffuse", cube.light.diffuse.x, cube.light.diffuse.y, cube.light.diffuse.z);
            // lightingShader.setVec3("light.specular", cube.light.specular.x, cube.light.specular.y, cube.light.specular.z);
            // lightingShader.setVec3("light.position", camera.position.x, camera.position.y, camera.position.z);
            // lightingShader.setVec3("light.direction", camera.front.x, camera.front.y, camera.front.z);
            // lightingShader.setFloat("light.cutOff", glm::cos(glm::radians(8.5f)));
            // lightingShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
            // lightingShader.setFloat("light.constant", 1.f);
            // lightingShader.setFloat("light.linear", 0.09f);
            // lightingShader.setFloat("light.quadratic", 0.032f);


            model = glm::translate(model, glm::vec3(cube.position.x, cube.position.y, cube.position.z));
            // model = glm::rotate(model, (float)glfwGetTime() * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            lightingShader.setMat4("model", model);


            // bind diffuse map
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuseBoxMap);

            // bind specular map
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specularBoxMap);

            // bind emission map
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, emissionBoxMap);

            // render the 1 cube
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        ourShader.setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        ImGui::Begin("Change Settings");
        ImGui::Text("Add or Remove Cube");
        if(ImGui::Button("-")){
            cubeList.pop_front();
        }
        ImGui::SameLine();
        ImGui::Text("%zu", cubeList.size()),
        ImGui::SameLine();
        if(ImGui::Button("+")){
            cubeList.push_back(getEmptyCubeObject());
        }

        for (auto &&cube : cubeList)
        {
            if(ImGui::CollapsingHeader("Cube")){
                ImGui::Text("Cube Position");
                std::string text1 = "##1" + std::to_string(cube.id);
                std::string text2 = "##2" + std::to_string(cube.id);
                std::string text3 = "##3" + std::to_string(cube.id);
                ImGui::SliderFloat(text1.c_str(), &cube.position.x, -100.0f, 100.f);
                ImGui::SliderFloat(text2.c_str(), &cube.position.y, -100.0f, 100.f);
                ImGui::SliderFloat(text3.c_str(), &cube.position.z, -100.0f, 100.f);

                if(ImGui::CollapsingHeader("Material")){

                    ImGui::Text("Ambient");
                    text1 = "##4" + std::to_string(cube.id);
                    text2 = "##5" + std::to_string(cube.id);
                    text3 = "##6" + std::to_string(cube.id);
                    if(ImGui::BeginTable("Ambient", 3)){
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::SliderFloat(text1.c_str(), &cube.material.ambient.x, 0.f, 1.f);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::SliderFloat(text2.c_str(), &cube.material.ambient.y, 0.f, 1.f);
                        ImGui::TableSetColumnIndex(2);
                        ImGui::SliderFloat(text3.c_str(), &cube.material.ambient.z, 0.f, 1.f);
                        ImGui::EndTable();
                    }

                    ImGui::Text("Diffuse");
                    text1 = "##7" + std::to_string(cube.id);
                    text2 = "##8" + std::to_string(cube.id);
                    text3 = "##9" + std::to_string(cube.id);
                    if(ImGui::BeginTable("Diffuse", 3)){
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::SliderFloat(text1.c_str(), &cube.material.diffuse.x, 0.f, 1.f);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::SliderFloat(text2.c_str(), &cube.material.diffuse.y, 0.f, 1.f);
                        ImGui::TableSetColumnIndex(2);
                        ImGui::SliderFloat(text3.c_str(), &cube.material.diffuse.z, 0.f, 1.f);
                        ImGui::EndTable();
                    }

                    ImGui::Text("Specular");
                    text1 = "##10" + std::to_string(cube.id);
                    text2 = "##11" + std::to_string(cube.id);
                    text3 = "##12" + std::to_string(cube.id);
                    if(ImGui::BeginTable("Specular", 3)){
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::SliderFloat(text1.c_str(), &cube.material.specular.x, 0.f, 1.f);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::SliderFloat(text2.c_str(), &cube.material.specular.y, 0.f, 1.f);
                        ImGui::TableSetColumnIndex(2);
                        ImGui::SliderFloat(text3.c_str(), &cube.material.specular.z, 0.f, 1.f);
                        ImGui::EndTable();
                    }

                    ImGui::Text("Shininess");
                    text3 = "##13" + std::to_string(cube.id);
                    ImGui::SliderFloat(text3.c_str(), &cube.material.shininess, 0.f, 1.f);
                }

                if(ImGui::CollapsingHeader("Light")){
                    ImGui::Text("Ambient");
                    text1 = "##14" + std::to_string(cube.id);
                    text2 = "##15" + std::to_string(cube.id);
                    text3 = "##16" + std::to_string(cube.id);
                    if(ImGui::BeginTable("Ambient", 3)){
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::SliderFloat(text1.c_str(), &cube.light.ambient.x, 0.f, 1.f);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::SliderFloat(text2.c_str(), &cube.light.ambient.y, 0.f, 1.f);
                        ImGui::TableSetColumnIndex(2);
                        ImGui::SliderFloat(text3.c_str(), &cube.light.ambient.z, 0.f, 1.f);
                        ImGui::EndTable();
                    }

                    ImGui::Text("Diffuse");
                    text1 = "##17" + std::to_string(cube.id);
                    text2 = "##18" + std::to_string(cube.id);
                    text3 = "##19" + std::to_string(cube.id);
                    if(ImGui::BeginTable("Diffuse", 3)){
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::SliderFloat(text1.c_str(), &cube.light.diffuse.x, 0.f, 1.f);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::SliderFloat(text2.c_str(), &cube.light.diffuse.y, 0.f, 1.f);
                        ImGui::TableSetColumnIndex(2);
                        ImGui::SliderFloat(text3.c_str(), &cube.light.diffuse.z, 0.f, 1.f);
                        ImGui::EndTable();
                    }

                    ImGui::Text("Specular");
                    text1 = "##20" + std::to_string(cube.id);
                    text2 = "##21" + std::to_string(cube.id);
                    text3 = "##22" + std::to_string(cube.id);
                    if(ImGui::BeginTable("Diffuse", 3)){
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::SliderFloat(text1.c_str(), &cube.light.specular.x, 0.f, 1.f);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::SliderFloat(text2.c_str(), &cube.light.specular.y, 0.f, 1.f);
                        ImGui::TableSetColumnIndex(2);
                        ImGui::SliderFloat(text3.c_str(), &cube.light.specular.z, 0.f, 1.f);
                        ImGui::EndTable();
                    }

                }
            }
        }


        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

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