#ifndef MODEL_COMPONENT_H
#define MODEL_COMPONENT_H

#include <memory>

#include "model.h"

struct ModelComponent
{
    std::shared_ptr<Model> model;
};

#endif