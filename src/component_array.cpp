#include "component_array.h"

template<typename T>
void ComponentArray<T>::InsertData(Entity entity, T component)
{
    assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() && "Component added to the same entity more than once.");
    size_t newIndex = mSize;
    mEntityToIndexMap[entity] = newIndex;
    mIndexToEntityMap[newIndex] = entity;
    mComponentArray[newIndex] = component;
    ++mSize;
}

template<typename T>
void ComponentArray<T>::RemoveData(Entity entity)
{
    assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Removing non-existent component.");

    // copying indexes
    size_t indexOfRemovedEntity = mEntityToIndexMap[entity];
    size_t indexOfLastElement = mSize - 1;
    mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

    Entity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
    mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
    mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

    mEntityToIndexMap.erase(entity);
    mIndexToEntityMap.erase(indexOfLastElement);

    --mSize;
}

template<typename T>
T& ComponentArray<T>::GetData(Entity entity)
{
    assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Retrieving non-existent component.");
    return mComponentArray[mEntityToIndexMap[entity]];
}

template<typename T>
void ComponentArray<T>::EntityDestroyed(Entity entity)
{
    if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
    {
        RemoveData(entity);
    }
}
