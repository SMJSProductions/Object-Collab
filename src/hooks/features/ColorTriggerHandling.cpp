#include "ColorTriggerHandling.hpp"

using namespace object_collab::prelude;

void ColorTriggerModLevelEditorLayer::addSpecial(GameObject* object) {
    LevelEditorLayer::addSpecial(object);

    CUSTOM_OBJECT_IMPLEMENT(object, CustomObjectInterface, custom, if (custom->isTriggerObject() && custom->getTraits().isColorTrigger()) {
        m_colorTriggers->addObject(object);
        m_colorTriggersChanged = true;
    });
}

void ColorTriggerModLevelEditorLayer::removeSpecial(GameObject* object) {
    LevelEditorLayer::removeSpecial(object);

    CUSTOM_OBJECT_IMPLEMENT(object, CustomObjectInterface, custom, if (custom->isTriggerObject() && custom->getTraits().isColorTrigger()) {
        m_colorTriggers->removeObject(object);
    });
}