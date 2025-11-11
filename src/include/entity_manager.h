#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <array>
#include <queue>
#include <bitset>
#include <cassert>
#include "types.h"

class EntityManager
{
public:
    EntityManager();

    Entity CreateEntity();
    void DestroyEntity(Entity entity);

    void SetSignature(Signature signature, Entity entity);
    Signature GetSignature(Entity entity);

private:
    std::queue<Entity> mAvailableEntities{};
    std::array<Signature, MAX_ENTITIES> mSignatures{};
    uint32_t mLivingEntityCount{};
};

#endif