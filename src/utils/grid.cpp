#include "utils/grid.h"

Grid::Grid()
{
    setupGrid();
}

void Grid::Draw(Shader &shader, glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 viewPos)
{
    gridVertices.clear();
    int minX = (int)(viewPos.x - halfExtent);
    int maxX = (int)(viewPos.x + halfExtent);
    int minZ = (int)(viewPos.z - halfExtent);
    int maxZ = (int)(viewPos.z + halfExtent);
    for (int x = minX; x <= maxX; x += gridSize) {
        gridVertices.push_back((float)x); gridVertices.push_back(0.0f); gridVertices.push_back((float)minZ);
        gridVertices.push_back((float)x); gridVertices.push_back(0.0f); gridVertices.push_back((float)maxZ);
    }
    for (int z = minZ; z <= maxZ; z += gridSize) {
        gridVertices.push_back((float)minX); gridVertices.push_back(0.0f); gridVertices.push_back((float)z);
        gridVertices.push_back((float)maxX); gridVertices.push_back(0.0f); gridVertices.push_back((float)z);
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_DYNAMIC_DRAW);

    shader.use();
    shader.setVec3("viewPos", viewPos);

    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glLineWidth(2.0f);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, gridVertices.size() / 3);
    glBindVertexArray(0);
}

void Grid::setupGrid()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

}
