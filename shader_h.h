#if !defined(SHADER_H)
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

class Shader{
    public:
        unsigned int ID;

        Shader(const char* vertexPath, const char* fragmentPath){
            std::string vertexCode;
            std::string fragmentCode;
            std::ifstream vShaderFile;
            std::ifstream fShaderFile;
            vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

            try
            {
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);
                std::stringstream vShaderStream, fShaderStream;

                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();

                vShaderFile.close();
                fShaderFile.close();

                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();
            }
            catch(const std::exception& e)
            {
                std::cerr<< "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ " << e.what() << '\n';
            }

            const char* vShaderCode = vertexCode.c_str();
            const char* fShaderCode = fragmentCode.c_str();

            unsigned int vertex = compileShader(GL_VERTEX_SHADER, vShaderCode, "VERTEX");
            unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, fShaderCode, "FRAGMENT");

            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            glLinkProgram(ID);
            checkCompileError(ID, "PROGRAM");

            glDeleteShader(vertex);
            glDeleteShader(fragment);

        }

        int compileShader(GLenum type, const char* shaderCode, std::string shaderType){
            unsigned int shader;
            
            shader = glCreateShader(type);
            glShaderSource(shader, 1, &shaderCode, NULL);
            glCompileShader(shader);

            checkCompileError(shader, "VERTEX");
            return shader;
        }

        void use(){
            glUseProgram(ID);
        }

       void setBool(const std::string &name, bool value) const
        {         
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
        }
        void setInt(const std::string &name, int value) const
        { 
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
        }
        void setFloat(const std::string &name, float value) const
        { 
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
        } 
        void setVec4(const std::string &name, float x, float y, float z, float a) const
        { 
            glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, a);
        } 

    private:
        void checkCompileError(GLuint shader, std::string type){
            int success;
            char infoLog[512];

            if(type != "PROGRAM"){
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if(!success)
                {
                    glGetShaderInfoLog(shader, 512, NULL, infoLog);
                    std::cout << "ERROR::SHADER::"<< type <<"::COMPILATION_FAILED\n" << infoLog << std::endl;
                };
            }else{
                glGetProgramiv(ID, GL_LINK_STATUS, &success);
                if(!success)
                {
                    glGetProgramInfoLog(ID, 512, NULL, infoLog);
                    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
                }
            }
        }
};


#endif // SHADER_H
