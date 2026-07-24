#include "LevelEditorLayer.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

void ModLevelEditorLayer::createObjectsFromSetup(gd::string& setup) {
    if (CustomLevelData::ACTIVE.isDefaulted()) {
        CustomLevelData::ACTIVE = CustomLevelData::load(this);
    }

    for (const std::string_view object : CustomLevelData::ACTIVE.getObjects()) {
        if (Result<ObjectVectors> objectVectorsResult = CustomObjectnterface::createObjectVectorsFromString(object)) {
            ObjectVectors objectVectors = std::move(objectVectorsResult).unwrap();
            CustomObjectnterface* customObject = typeinfo_cast<CustomObjectnterface*>(this->addObjectFromVector(objectVectors.first, objectVectors.second));

            customObject->internalPostInit();
            customObject->postInit();
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
        if (CustomObjectnterface* customObject = typeinfo_cast<CustomObjectnterface*>(object)) {
            customObject->internalPostInit();
            customObject->postInit();
        }
    }

    return object;
}