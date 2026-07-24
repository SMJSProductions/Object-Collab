#include "GameObject.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

GameObject* ModGameObject::createWithKey(const int key) {
    if (ObjectAPI::getBaseCustomObjectID() > key) {
        return GameObject::createWithKey(key);
    } else if (ObjectInfo* info = ObjectAPI::getCustomObject(key)) {
        CustomObjectInterface* object = info->hasFactory() ? info->factory() : new CustomObject<GameObject>();

        if (object && object->init(info->getSprite().c_str())) {
            GameObject* gameObject = object->getGameObject();

            gameObject->m_objectID = key;

            return gameObject;
        }

        log::error("Failed to initialize object with ID {}", key);

        delete object;

        return nullptr;
    } else {
        return GameObject::createWithKey(key);
    }
}