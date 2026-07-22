#include "LevelEditorLayer.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

void ModLevelEditorLayer::createObjectsFromSetup(gd::string& setup) {
    if (CustomLevelData::ACTIVE.isDefaulted()) {
        CustomLevelData::ACTIVE = CustomLevelData::load(this);
    }

    for (const std::string_view object : CustomLevelData::ACTIVE.getObjects()) {
        if (Result<ObjectVectors> objectVectorsResult = CustomObject::createObjectVectorsFromString(object)) {
            ObjectVectors objectVectors = std::move(objectVectorsResult).unwrap();
            CustomObject* object = reinterpret_cast<CustomObject*>(this->addObjectFromVector(objectVectors.first, objectVectors.second));

            object->internalPostInit();
            object->postInit();
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
        if (CustomObject* customObject = typeinfo_cast<CustomObject*>(object)) {
            customObject->internalPostInit();
            customObject->postInit();
        }
    }

    return object;
}