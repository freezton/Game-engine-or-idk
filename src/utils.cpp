#include "utils.h"

void drawGrid(Shader &shader, glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 viewPos)
{
    std::vector<float> gridVertices;
    float gridSize = 1.0f;
    int halfExtent = 100;

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
    GLuint gridVAO, gridVBO;
    glGenVertexArrays(1, &gridVAO);
    glBindVertexArray(gridVAO);

    glGenBuffers(1, &gridVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    shader.use();
    shader.setVec3("viewPos", viewPos);

    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glLineWidth(2.0f);
    glBindVertexArray(gridVAO);
    glDrawArrays(GL_LINES, 0, gridVertices.size() / 3);
    glBindVertexArray(0);
}