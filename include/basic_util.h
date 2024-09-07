#ifndef BASIC_UTIL_H
#define BASIC_UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <assert.h>
#include <glew/glew.h>

using namespace std;

bool readFile(const char* fileName, string& outFile);

char* readBinaryFile(const char* pFileName, int& size);

void writeBinaryFile(const char* pFileName, const char* pData, int size);

void basicError(const char* pFileName, int line, const char* pError, ... );
void basicFileError(const char* pFileName, uint line, const char* pError);

#define BASIC_ERROR0(message) basicError(__FILE__, __LINE__, message)
#define BASIC_ERROR(message, ...) basicError(__FILE__, __LINE__, message, __VA_ARGS__)
#define BASIC_FILE_ERROR(message) basicFileError(__FILE__, __LINE__, message)

#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ZERO_MEM_VAR(var) memset(&var, 0, sizeof(var))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

#ifndef MAX
#define MAX(a,b) ((a > b) ? a : b)
#endif

#ifdef _WIN64
#define SNPRINTF _snprintf_s
#define VSNPRINTF vsnprintf_s
#define RANDOM rand
#define SRANDOM srand((unsigned)time(NULL))
#pragma warning (disable: 4566)
#else
#define SNPRINTF snprintf
#define VSNPRINTF vsnprintf
#define RANDOM random
#define SRANDOM srandom(getpid())
#endif

#define INVALID_UNIFORM_LOCATION 0xffffffff
#define INVALID_OGL_VALUE 0xffffffff

#define NUM_CUBE_MAP_FACES 6

#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }

long long getCurrentTimeMillis();

#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)
#define NOT_IMPLEMENTED printf("Not implemented case in %s:%d\n", __FILE__, __LINE__); exit(0);

string getDirFromFileName(const string& fileName);

#define MAX_BONES (200)

#define CLAMP(val, start, end) val = std::min(std::max(val, start), end);

int getGLMajorVersion();
int getGLMinorVersion();

int isGLVersionHigher(int majorV, int minorV);

#ifndef BASIC_VULKAN
#define GLExitIfError                                                          \
{                                                                               \
    GLenum Error = glGetError();                                                \
                                                                                \
    if (Error != GL_NO_ERROR) {                                                 \
        printf("OpenGL error in %s:%d: 0x%x\n", __FILE__, __LINE__, Error);     \
        exit(0);                                                                \
    }                                                                           \
}

#define GLCheckError() (glGetError() == GL_NO_ERROR)

void gl_check_error(const char* function, const char *file, int line);

#define CHECK_GL_ERRORS

#ifdef CHECK_GL_ERRORS
#define GCE gl_check_error(__FUNCTION__, __FILE__, __LINE__);
#else
#define GCE
#endif

void glDebugOutput(GLenum source,
                   GLenum type,
                   unsigned int id,
                   GLenum severity,
                   GLsizei length,
                   const char *message,
                   const void *userParam);
#endif



#endif




