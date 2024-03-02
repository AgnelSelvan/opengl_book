#define GLM_ENABLE_EXPERIMENTA
#include <iostream>
#include <SDL2/SDL.h>
#include<glad/glad.h>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>

int gScreenWidth = 500;
int gScreenHeight = 300;
SDL_Window* gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr;
bool gQuit = false;
//VAO
GLuint gVertexArrayObject = 0;
//VBO
GLuint gVertexBufferObject = 0;
//IBO
GLuint gIndexBufferObject = 0;
GLuint gGraphicsPipelineShaderProgram = 0;
float gUOffset = 0.;

// Error Handling
//Clear all the error
static void glClearAllErrors(){
  while (glGetError() != GL_NO_ERROR)
  {}
}
// Check for OpenGL Error on which line and on which function
static bool glCheckErrorStatus(const char* function, int line){
  while (GLenum error = glGetError())
  {
    std::cout << "OpenGL Error: " << error << "\tLine: " << line << "\tfunction: " << function << std::endl;
    return true;
  }
  return false;
}
// Macros for defining a method for error handling
#define glCheck(x); glClearAllErrors(); x; glCheckErrorStatus(#x, __LINE__);

std::string loadShaderAsString(const std::string& filename){
  std::string result = "";

  std::string line = "";
  std::ifstream myFile(filename.c_str());
  if(myFile.is_open()){
    while (std::getline(myFile, line))
    {
      result += line + "\n";
    }
    myFile.close();
  }
  return result;
}

GLuint compileShader(GLuint type, const std::string& shaderSource){
  GLuint shaderObject = glCreateShader(type);
  
  const char* src = shaderSource.c_str();
  glShaderSource(shaderObject, 1, &src, nullptr);
  glCompileShader(shaderObject);

  return shaderObject;
}

GLuint createShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource){
  GLuint shaderProgram = glCreateProgram();

  GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
  GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glValidateProgram(shaderProgram);

  return shaderProgram;
}

void createGraphicsPipeline(){
  std::string vertexShaderSource = loadShaderAsString("./shaders/vert.glsl");
  std::string fragmentShaderSource = loadShaderAsString("./shaders/frag.glsl");

  gGraphicsPipelineShaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
}

// Gets OpenGL Current Version
void getOpenGLVersion(){
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "Shading Langugage: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

// Initialize SDL2, OpenGL and GLAD
void initializeProgram(){
  // Checks if SDL2 can open
  if(SDL_Init(SDL_INIT_VIDEO) < 0){
    std::cout << "SDL2 failed to load video subsystem" << std::endl;
    exit(1);
  }

  // Setting up SDL and OpenGL related configurations
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  // Creates a new window
  gGraphicsApplicationWindow = SDL_CreateWindow("OPENGL using SDL2", 0, 0, gScreenWidth, gScreenHeight, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
  // Exit the app on window creation failed
  if(gGraphicsApplicationWindow == nullptr){
    std::cout << "SDL2 window creating failed" << std::endl;
    exit(1);
  }

  // Setting up the OpenGL context to current Window and returns the contextt
  gOpenGLContext = SDL_GL_CreateContext(gGraphicsApplicationWindow);
  // Exit the app if getting current context failed
  if(gOpenGLContext == nullptr){
    std::cout << "OpenGL Current context not available" << std::endl;
    exit(1);
  }

  // If Glad Loads fails quitting the App
  if(!gladLoadGLLoader(SDL_GL_GetProcAddress)){
    std::cout << "Error in lading Glad" << std::endl;
    exit(1);
  }
  // responsible for printing the OpenGL Version the project uses
  getOpenGLVersion();
}

// Handle User input event on window open
void input(){
  SDL_Event e;
  while (SDL_PollEvent(&e)!=0)
  {
    //e.type: 768 means on click escape
    if(e.type == SDL_QUIT){
      std::cout << "Bubyee!!" << std::endl;
      gQuit = true;
    }
  }

  const Uint8 *state = SDL_GetKeyboardState(NULL);
  if(state[SDL_SCANCODE_UP]){
    gUOffset += 0.0001;
  }
  if(state[SDL_SCANCODE_DOWN]){
    gUOffset -= 0.0001;
  }
  
}

void preDraw(){
  // Making the OpenGL Viewport as per the window created
  glViewport(0, 0, gScreenWidth, gScreenHeight);
  
  // RGBA values, Used for clearing the buffer color values
  glClearColor(0.f, 0.f, 0.f, 1.f);
  // Clear the buffer values
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  // Using rhe Shader Program
  glUseProgram(gGraphicsPipelineShaderProgram);

  const GLchar* uniformName = "uModelMatrix";
  // Getting Uniform Location from Shader
  GLint mModelMatrixLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, uniformName);
  // Converting local coordinate to world space cordinates
  glm::mat4 translate = glm::translate(glm::mat4(1.), glm::vec3(0., gUOffset, 0.));
  // If location found
  if(mModelMatrixLocation >= 0){
    // Passing World space cordinates to Shader
    // Uniform is useful for passing data from CPU directly to GPU
    glUniformMatrix4fv(mModelMatrixLocation, 1, GL_FALSE, &translate[0][0]);
  }else{
    std::cout << "Location not found! Please check " << uniformName << " is spelled correctly" << std::endl;
    exit(EXIT_FAILURE);
  }

}

void draw(){
  // Binds Vertex Array Objects
  glBindVertexArray(gVertexArrayObject);
  // Binds Vertex Data
  glCheck(glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject));
  // Draws the triangle
  // glDrawArrays(GL_TRIANGLES, 0, 6);
  glCheck(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
}

// Main Game Engine Entry point
void mainLoop(){
  while (!gQuit)
  {
    // resposible for checking user inputs
    input();

    // resposible for handling any requirements before drawing on screen
    preDraw();

    // Responsible for drawing on screen
    draw();

    // Swapping background Frame to current frame
    SDL_GL_SwapWindow(gGraphicsApplicationWindow);
  }
  
}

void cleanUp(){
  // Clear the Window loaded on memory
  SDL_DestroyWindow(gGraphicsApplicationWindow);
  // quits the window
  SDL_Quit();
}

void vertexSpecification(){
  // This data lives on CPU
  const std::vector<GLfloat> vertexData {
    // 0 - Vertex
    -0.5f, -0.5f, 0.0f,  // bottom Left vertex position
    1.0f, 0.0f, 0.0f,    // Color
    // 1 - Vertex
    0.5f, -0.5f, 0.0f,   // bottom Right vertex position
    0.0f, 1.0f, 0.0f,    // Color
    // 2 - Vertex
    -0.5f, 0.5f, 0.0f,   // Top left vertex position
    0.0f, 0.0f, 1.0f,    // Color
    // 3 - Vertex
    0.5f, 0.5f, 0.0f,  // Top right vertex position
    1.0f, 0.0f, 0.0f,    // Color
  };

  //VAO
  glGenVertexArrays(1, &gVertexArrayObject);
  glBindVertexArray(gVertexArrayObject);
  
  //VBO
  glGenBuffers(1, &gVertexBufferObject);
  glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GL_FLOAT), vertexData.data(), GL_STATIC_DRAW);

  const std::vector<GLuint> indexBufferData {2,0,1,3,2,1};
  //IBO i.e (EBO)
  glGenBuffers(1, &gIndexBufferObject);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBufferObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferData.size() * sizeof(GLuint), indexBufferData.data(), GL_STATIC_DRAW);

  // Loading position buffer on 0 location vertex shader
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
    0, 
    3,
    GL_FLOAT,
    GL_FALSE,
    sizeof(GL_FLOAT) * 6,
    (void*)0
  );
 
  // Loading color buffer on 1 location on vertex shader
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*6, (void*)(sizeof(GL_FLOAT) * 3));

  glBindVertexArray(0);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

}


int main(int argc, char const *argv[])
{
  // responsible for Initializing GLAD, OpenGL and SDL2
  initializeProgram();
  // Responsible for loading the data from CPU to Graphics Memory that can be vertex, texture data, color and etc
  vertexSpecification();
  // responsible for setting the Graphics pipeling. 
  // Loading the shaders stuffs, creting program and etc
  createGraphicsPipeline();
  // responsible for painting on the screen
  mainLoop();
  // Destroys window.
  cleanUp();
  return 0;
}
