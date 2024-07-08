#if !defined(MESH_H)
#define MESH_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <shader_h.h>

#include <string>
#include <vector>

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    int mBoneIDs[MAX_BONE_INFLUENCE];
    float mWeights[MAX_BONE_INFLUENCE];
};

struct Texture{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh{
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        unsigned int VAO;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures){
            this->vertices = vertices;
            this->indices = indices;
            this->textures = textures;

            setupMesh();
        }

        void draw(Shader &shader, GLenum polygonMode){
            glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
            unsigned int diffuseNr = 1;
            unsigned int specularNr = 1;
            unsigned int normalNr = 1;
            unsigned int heightNr = 1;
            for (unsigned int i = 0; i < textures.size(); i++)
            {
                unsigned int id = GL_TEXTURE0 + i;
                glActiveTexture(id);
                std::string number;
                std::string name = textures[i].type;
                if (name == "texture_diffuse")
                    number = std::to_string(diffuseNr++);
                else if (name == "texture_specular")
                    number = std::to_string(specularNr++);
                else if (name == "texture_normal")
                    number = std::to_string(normalNr++);
                else if (name == "texture_height")
                    number = std::to_string(heightNr++);

                glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
                glBindTexture(GL_TEXTURE_2D, textures[i].id);
                shader.setFloat("material.shininess", 64.0f);
                // shader.setInt("material.diffuse", 0);
                // shader.setInt("material.specular", 1);
                // shader.setInt("material.emission", 2);
            }

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            glActiveTexture(GL_TEXTURE0);
            // glDrawArrays(GL_TRIANGLES, 0, 36);
        }

    private:
        unsigned int VBO, EBO;

        void setupMesh(){
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

            glEnableVertexAttribArray(5);
            glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, mBoneIDs));

            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mWeights));

            glBindVertexArray(0);

        }
};


#endif // MESH_H
