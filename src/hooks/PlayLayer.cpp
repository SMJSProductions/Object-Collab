#include "PlayLayer.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

void ModPlayLayer::processCreateObjectsFromSetup() {
    if (CustomLevelData::ACTIVE.isDefaulted()) {
        CustomLevelData::ACTIVE = CustomLevelData::load(this);
    }

    for (const std::string_view object : CustomLevelData::ACTIVE.getObjects()) {
        if (Result<ObjectVectors> objectVectorsResult = CustomObject::createObjectVectorsFromString(object)) {
            ObjectVectors objectVectors = std::move(objectVectorsResult).unwrap();
            CustomObject* object = reinterpret_cast<CustomObject*>(GameObject::objectFromVector(
                objectVectors.first,
                objectVectors.second,
                this,
                m_lowDetailMode
            ));

            this->addObject(object);

            object->internalPostInit();
            object->postInit();
        } else {
            log::warn("Failed to load object: {}", std::move(objectVectorsResult).unwrapErr());
        }
    }

    // Reset to no longer rely on the saved allocation setup
    CustomLevelData::ACTIVE = true;

    PlayLayer::processCreateObjectsFromSetup();
}