#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define RELATIVE_PATH "../../"

static std::string readFile(const std::string& path) 
{
    std::ifstream file(path);
    if (!file.is_open()) 
    {
        std::cerr << "Failed to open file" << path << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string readShader(const std::string& fileName)
{
    return readFile(RELATIVE_PATH"assets/shaders/"+fileName);
}