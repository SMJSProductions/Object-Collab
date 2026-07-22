#include "GameObject.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

GameObject* ModGameObject::createWithKey(const int key) {
    if (ObjectAPI::getBaseCustomObjectID() > key) {
        return GameObject::createWithKey(key);
    } else if (ObjectInfo* info = ObjectAPI::getCustomObject(key)) {
        CustomObject* object = info->hasFactory() ? info->factory() : new CustomObject();

        if (object) {
            object->m_objectID = key;
            object->m_addToNodeContainer = true;

            if (object->init(info->getSprite().c_str())) {
                object->autorelease();

                return object;
            }
        }

        delete object;

        return nullptr;
    } else {
        return GameObject::createWithKey(key);
    }
}

bool ModGameObject::isTrigger() {
    if (CustomObject* object = typeinfo_cast<CustomObject*>(this)) {
        return object->m_classType == GameObjectClassType::Effect && object->m_objectType == GameObjectType::Modifier;
    } else {
        return GameObject::isTrigger();
    }
}

bool ModGameObject::isSpawnableTrigger() {
    if (CustomObject* object = typeinfo_cast<CustomObject*>(this)) {
        return object->m_classType == GameObjectClassType::Effect && object->m_objectType == GameObjectType::Modifier;
    } else {
        return GameObject::isTrigger();
    }
}