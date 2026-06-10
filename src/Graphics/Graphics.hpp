#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

namespace Silicon {
    // ураа шейдеры!!
    class Shader {
    public:
        unsigned int ID;
        Shader(const char* vertexPath, const char* fragmentPath) {
            std::string vertexCode, fragmentCode;
            std::ifstream vShaderFile, fShaderFile;
            vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            try {
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);
                std::stringstream vShaderStream, fShaderStream;
                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();
                vShaderFile.close();
                fShaderFile.close();
                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();
            } catch (std::ifstream::failure& e) {
                std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n";
            }
            const char* vShaderCode = vertexCode.c_str();
            const char* fShaderCode = fragmentCode.c_str();
            unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
            unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);
            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            glLinkProgram(ID);
            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }
        void use() { glUseProgram(ID); }
        void setMat4(const std::string &name, const glm::mat4 &mat) const {
            glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        void setVec3(const std::string &name, const glm::vec3 &value) const {
            glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        }
    };

    // ура первые текстуркии урикии
    class Texture {
    public:
        unsigned int ID;
        int width, height;
        void load(const std::string& path) {
            glGenTextures(1, &ID);
            glBindTexture(GL_TEXTURE_2D, ID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            int nrChannels;
            unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
            if (data) {
                GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            } else {
                std::cout << "Failed to load texture: " << path << std::endl;
            }
            stbi_image_free(data);
        }
    };

    // спрайты или же персонажи хз кароче
    class Sprite {
    public:
        glm::vec2 position{0.0f, 0.0f};
        glm::vec2 size{50.0f, 50.0f};
        glm::vec3 color{1.0f, 1.0f, 1.0f};
        Texture texture;

        void load(const std::string& path) {
            texture.load(path);
            size = glm::vec2(texture.width, texture.height); // размер по дефолчику капецц
        }
        Silicon::AABB getAABB() const {
            return {position.x, position.y, size.x, size.y};
        }
    };

    //мне скучно так что это рендреры
    class Renderer {
    private:
        Shader* shader;
        unsigned int quadVAO;
    public:
        Renderer(Shader* s) : shader(s) {
            unsigned int VBO;
            float vertices[] = { 
                // зачем     // чо
                0.0f, 1.0f, 0.0f, 1.0f,
                1.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f, 
                0.0f, 1.0f, 0.0f, 1.0f,
                1.0f, 1.0f, 1.0f, 1.0f,
                1.0f, 0.0f, 1.0f, 0.0f
            };
            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            glBindVertexArray(quadVAO);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
            glBindBuffer(GL_ARRAY_BUFFER, 0);  
            glBindVertexArray(0);
        }

        void draw(const Sprite& sprite) {
            shader->use();
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(sprite.position, 0.0f));  
            model = glm::scale(model, glm::vec3(sprite.size, 1.0f)); 
            
            shader->setMat4("model", model);
            shader->setVec3("spriteColor", sprite.color);
            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, sprite.texture.ID);
            glBindVertexArray(quadVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    };
}
