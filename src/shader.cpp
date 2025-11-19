#include "shader.h"

ShaderProgram::ShaderProgram()
{
    mProgramID = glCreateProgram();
}

void ShaderProgram::use()
{
    glUseProgram(mProgramID);
}

void ShaderProgram::link()
{
    glLinkProgram(mProgramID);
    checkCompileErrors(mProgramID, "PROGRAM");
    for (GLuint shader: mAttachedShaders)
        glDeleteShader(shader);
    mAttachedShaders.clear();
}

void ShaderProgram::attachShader(GLenum type, const std::string& source)
{
    std::string code;
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        file.open("assets/shaders/" + source);
        std::stringstream vShaderStream;
        vShaderStream << file.rdbuf();
        file.close();
        code = vShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << e.what() << std::endl;
    }

    const char* shaderCode = code.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);
    checkCompileErrors(shader, shaderTypeToString(type)); 
    glAttachShader(mProgramID, shader);
    mAttachedShaders.push_back(shader);
}

void ShaderProgram::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(mProgramID, name.c_str()), (int)value);
}

void ShaderProgram::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(mProgramID, name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(mProgramID, name.c_str()), value);
}

void ShaderProgram::setVec3(const std::string& name, glm::vec3 value) const
{
    glUniform3fv(glGetUniformLocation(mProgramID, name.c_str()), 1, glm::value_ptr(value));
}

void ShaderProgram::setMat4(const std::string& name, glm::mat4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(mProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

std::string ShaderProgram::shaderTypeToString(GLenum type) {
    switch (type) {
        case GL_VERTEX_SHADER: return "VERTEX";
        case GL_FRAGMENT_SHADER: return "FRAGMENT";
        case GL_GEOMETRY_SHADER: return "GEOMETRY";
        case GL_COMPUTE_SHADER: return "COMPUTE";
        case GL_TESS_CONTROL_SHADER: return "TESS_CONTROL";
        case GL_TESS_EVALUATION_SHADER: return "TESS_EVALUATION";
        default: return "UNKNOWN";
    }
}

void ShaderProgram::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
