#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

void drawGrid(Shader &shader, glm::mat4 model, glm::mat4 view, glm::mat4 proj, glm::vec3 viewPos);
// void drawGrid(Shader &shader, glm::mat4 model, glm::mat4 view, glm::mat4 proj);

#endif