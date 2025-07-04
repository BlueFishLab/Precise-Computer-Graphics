#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>

class Shader {
public:
    unsigned int ID;

    Shader(const char* vertexSource, const char* fragmentSource);
    ~Shader();

    void use() const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setFloat(const std::string& name, float value) const;
    void setInt(const std::string& name, int value) const;

private:
    unsigned int compileShader(const char* source, GLenum type);
    void checkCompileErrors(unsigned int shader, std::string type);
};