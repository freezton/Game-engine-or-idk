#include "entity_manager.h" 

EntityManager::EntityManager()
{
    for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
    {
        mAvailableEntities.push(entity);
    }
}

Entity EntityManager::CreateEntity()
{
    assert(mLivingEntityCount < MAX_ENTITIES && "Too many entities");
    Entity id = mAvailableEntities.front();
    mAvailableEntities.pop();
    ++mLivingEntityCount;
    return id;
}

void EntityManager::DestroyEntity(Entity entity)
{
    assert(entity < MAX_ENTITIES && "Entity out of range");
    mSignatures[entity].reset();
    mAvailableEntities.push(entity);
    --mLivingEntityCount;
}

void EntityManager::SetSignature(Signature signature, Entity entity)
{
    assert(entity < MAX_ENTITIES && "Entity out of range");
    mSignatures[entity] = signature;
}

Signature EntityManager::GetSignature(Entity entity)
{
    assert(entity < MAX_ENTITIES && "Entity out of range");
    return mSignatures[entity];
}
