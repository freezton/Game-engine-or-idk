#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <glm/glm.hpp>
#include "components/components.h"
#include "system.h"
#include "coordinator.h"
#include "shader.h"

extern Coordinator coordinator;

class RenderSystem : public System
{
public:
    void Update(ShaderProgram& shader)
    {
        for (auto entity: mEntities)
        {
            auto& transform = coordinator.GetComponent<Transform>(entity);
            auto& modelComp = coordinator.GetComponent<ModelComponent>(entity);

            glm::mat4 model = glm::translate(glm::mat4(1.0f), transform.position);
            model = glm::rotate(model, transform.rotation.x, glm::vec3(1, 0, 0));
            model = glm::rotate(model, transform.rotation.y, glm::vec3(0, 1, 0));
            model = glm::rotate(model, transform.rotation.z, glm::vec3(0, 0, 1));
            model = glm::scale(model, transform.scale);

            shader.use();
            shader.setMat4("model", model);
            auto& callback = coordinator.GetComponent<UniformCallback>(entity);
            callback.applyUniforms(shader);
            modelComp.model->Draw(shader);
        }
    }

};

#endif