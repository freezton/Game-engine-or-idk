#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <memory>
#include <glm/glm.hpp>

#include "model.h"

struct Transform
{
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

struct ModelComponent
{
    std::shared_ptr<Model> model;
};

#endif