#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

class ShaderProgram
{
public:

    ShaderProgram();

    GLuint getID();
    void use();
    void link();
    void attachShader(GLenum type, const std::string& source);
    
    // uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, glm::vec3 value) const;
    void setMat4(const std::string& name, glm::mat4 value) const;

private:
    GLuint mProgramID;
    std::vector<GLuint> mAttachedShaders;

    std::string shaderTypeToString(GLenum type);
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif