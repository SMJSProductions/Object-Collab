#include "LevelEditorLayer.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

void ModLevelEditorLayer::createObjectsFromSetup(gd::string& setup) {
    if (CustomLevelData::ACTIVE.isDefaulted()) {
        CustomLevelData::ACTIVE = CustomLevelData::load(this);
    }

    for (const std::string_view object : CustomLevelData::ACTIVE.getObjects()) {
        if (Result<ObjectVectors> objectVectorsResult = CustomObjectInterface::createObjectVectorsFromString(object)) {
            ObjectVectors objectVectors = std::move(objectVectorsResult).unwrap();

            if (CustomObjectInterface* customObject = typeinfo_cast<CustomObjectInterface*>(this->addObjectFromVector(
                objectVectors.first,
                objectVectors.second
            ))) {
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

    LevelEditorLayer::createObjectsFromSetup(setup);
}

GameObject* ModLevelEditorLayer::createObject(const int key, const CCPoint position, const bool noUndo) {
    GameObject* object = LevelEditorLayer::createObject(key, position, noUndo);

    if (key >= ObjectAPI::getBaseCustomObjectID()) {
        if (CustomObjectInterface* customObject = typeinfo_cast<CustomObjectInterface*>(object)) {
            customObject->postInit();
        } else {
            log::warn("Failed to initialize object");
        }
    }

    return object;
}