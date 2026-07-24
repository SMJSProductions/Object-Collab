#include "PlayLayer.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

void ModPlayLayer::processCreateObjectsFromSetup() {
    if (CustomLevelData::ACTIVE.isDefaulted()) {
        CustomLevelData::ACTIVE = CustomLevelData::load(this);
    }

    for (const std::string_view object : CustomLevelData::ACTIVE.getObjects()) {
        if (Result<ObjectVectors> objectVectorsResult = CustomObjectInterface::createObjectVectorsFromString(object)) {
            ObjectVectors objectVectors = std::move(objectVectorsResult).unwrap();
            
            if (CustomObjectInterface* customObject = typeinfo_cast<CustomObjectInterface*>(GameObject::objectFromVector(
                objectVectors.first,
                objectVectors.second,
                this,
                m_lowDetailMode
            ))) {
                this->addObject(customObject->getGameObject());
                customObject->postInit();
            } else {
                log::warn("Failed to initialize object");
            }
        } else {
            log::warn("Failed to load object: {}", std::move(objectVectorsResult).unwrapErr());
        }
    }

    // Reset to no longer rely on the saved allocation setup
    CustomLevelData::ACTIVE = true;

    PlayLayer::processCreateObjectsFromSetup();
}