#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include <array>
#include <unordered_map>
#include <cassert>
#include <memory>

#include "types.h"
#include "component_array.h"

class ComponentManager
{
public:
    template<typename T>
    void RegisterComponent();
    
    template<typename T>
    ComponentType GetComponentType();
    
    template<typename T>
    void AddComponent(Entity entity, T component);

    template<typename T>
    void RemoveComponent(Entity entity);

    template<typename T>
    T& GetComponent(Entity entity);

    void EntityDestroyed(Entity entity);

private:
    std::unordered_map<const char*, ComponentType> mComponentTypes{};
    std::unordered_map<const char*, std::shared_ptr<IComponentArray>> mComponentArrays{};
    ComponentType mNextComponentType{};

    template<typename T>
    std::shared_ptr<ComponentArray<T>> GetComponentArray();
};

#endif