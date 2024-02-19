#include <iostream>
#include <SDL2/SDL.h>
#include<glad/glad.h>

int gScreenWidth = 500;
int gScreenHeight = 300;
SDL_Window* gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr;
bool gQuit = false;

// Gets OpenGL Current Version
void getOpenGLVersion(){
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "Shading Langugage: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

// Initialize SDL2, OpenGL and GLAD
void initializeProgram(){
  if(SDL_Init(SDL_INIT_VIDEO) < 0){
    std::cout << "SDL2 failed to load video subsystem" << std::endl;
    exit(1);
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  gGraphicsApplicationWindow = SDL_CreateWindow("OPENGL using SDL2", 0, 0, gScreenWidth, gScreenHeight, SDL_WINDOW_OPENGL);
  if(gGraphicsApplicationWindow == nullptr){
    std::cout << "SDL2 window creating failed" << std::endl;
    exit(1);
  }

  gOpenGLContext = SDL_GL_CreateContext(gGraphicsApplicationWindow);
  if(gOpenGLContext == nullptr){
    std::cout << "OpenGL Current context not available" << std::endl;
    exit(1);
  }

  if(!gladLoadGLLoader(SDL_GL_GetProcAddress)){
    std::cout << "Error in lading Glad" << std::endl;
    exit(1);
  }

  getOpenGLVersion();
}

// Handle User input event on window open
void input(){
  SDL_Event e;
  while (SDL_PollEvent(&e)!=0)
  {
    //e.type: 768 means on click escape
    if(e.type == SDL_QUIT || e.type == 768){
      std::cout << "Bubyee!!" << std::endl;
      gQuit = true;
    }
  }
  
}

void preDraw(){}

void draw(){}

// Main Game Engine Entry point
void mainLoop(){
  while (!gQuit)
  {
    input();

    preDraw();

    draw();

    // Swapping background Frame to current frame
    SDL_GL_SwapWindow(gGraphicsApplicationWindow);
  }
  
}

void cleanUp(){
  SDL_DestroyWindow(gGraphicsApplicationWindow);
  SDL_Quit();
}

int main(int argc, char const *argv[])
{
  initializeProgram();
  mainLoop();
  cleanUp();
  return 0;
}
