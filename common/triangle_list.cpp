#include <stdio.h>
#include <vector>

#include "basic_math.h"
#include "triangle_list.h"
#include "terrain.h"

TriangleList::TriangleList(){

}

void TriangleList::createTriangleList(int width, int depth, const BaseTerrain* pTerrain){
    mWidth = width;
    mDepth = depth;

    createGLState();
    populateBuffers(pTerrain);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void TriangleList::createGLState(){
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glGenBuffers(1, &mIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);

    int POS_LOC = 0;
    size_t numFloats = 0;
    glEnableVertexAttribArray(POS_LOC);
    glVertexAttribPointer(POS_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(numFloats * sizeof(float)));
    numFloats += 3;
}

void TriangleList::populateBuffers(const BaseTerrain* pTerrain){
    std::vector<Vertex> vertices;
    vertices.resize(mWidth * mDepth);
    initVertices(pTerrain, vertices);

    std::vector<uint> indices;
    int numQuads = (mWidth - 1) * (mDepth - 1);
    indices.resize(numQuads * 6);
    initIndices(indices);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);
}

void TriangleList::Vertex::initVertex(const BaseTerrain* pTerrain, int x, int z){
    float y = pTerrain->getHeight(x, z);

    float worldScale = pTerrain->getWorldScale();

    pos = Vector3f(x * worldScale, y, z * worldScale);
}

void TriangleList::initVertices(const BaseTerrain* pTerrain, std::vector<Vertex>& vertices){
    int index = 0;
    for (int i = 0; i < mDepth; i++)
    {
        for (int x = 0; x < mWidth; x++)
        {
            assert(index < vertices.size());
            vertices[index].initVertex(pTerrain, x, i);
        }

    }

}





















