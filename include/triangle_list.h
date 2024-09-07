#if !defined(TRIANGLE_LIST_H)
#define TRIANGLE_LIST_H

#include <glew/glew.h>
#include <vector>

#include "basic_math.h"
#include "terrain.h"

class BaseTerrain;

class TriangleList {
    public:
        TriangleList();

        void createTriangleList(int width, int depth, const BaseTerrain* pTerrain);

        void render();

    private:
        struct Vertex{
            Vector3f pos;
            void initVertex(const BaseTerrain* pTerrain, int x, int z);
        };

        void createGLState();

        void populateBuffers(const BaseTerrain* pTerrain);
        void initVertices(const BaseTerrain* pTerrain, std::vector<Vertex>& vertices);
        void initIndices(std::vector<uint>& indices);

        int mWidth = 0;
        int mDepth = 0;

        GLuint mVAO;
        GLuint mVBO;
        GLuint mIBO;
};


#endif // TRIANGLE_LIST_H
