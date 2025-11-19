#ifndef GRID_H
#define GRID_H

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include "shader.h"

class Grid
{
public:
    Grid();

    void Draw(ShaderProgram& shader, glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 viewPos);

private:

    void setupGrid();

    GLuint VAO, VBO;
    std::vector<float> gridVertices;
    float gridSize = 1.0f;
    int halfExtent = 100;
};

#endif