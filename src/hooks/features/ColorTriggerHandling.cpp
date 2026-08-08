#include "ColorTriggerHandling.hpp"

void ColorTriggerModLevelEditorLayer::addSpecial(GameObject* object) {
    LevelEditorLayer::addSpecial(object);

    CUSTOM_OBJECT_IMPLEMENT(object, custom, if (custom->isTriggerObject() && custom->getTraits().isColorTrigger()) {
        m_colorTriggers->addObject(object);
        m_colorTriggersChanged = true;
    });
}

void ColorTriggerModLevelEditorLayer::removeSpecial(GameObject* object) {
    LevelEditorLayer::removeSpecial(object);

    CUSTOM_OBJECT_IMPLEMENT(object, custom, if (custom->isTriggerObject() && custom->getTraits().isColorTrigger()) {
        m_colorTriggers->removeObject(object);
    });
}