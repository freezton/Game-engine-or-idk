#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <memory>
#include <unordered_map>

#include "types.h"
#include "system.h"

class SystemManager
{
public:
    template<typename T>
    std::shared_ptr<T> RegisterSystem();

    template<typename T>
    void SetSignature(Signature signature);

    void EntityDestroyed(Entity entity);

    void EntitySignatureChanged(Entity entity, Signature entitySignature);

private:
    std::unordered_map<const char*, Signature> mSignatures{};
    std::unordered_map<const char*, std::shared_ptr<System>> mSystems{};
};

#endif