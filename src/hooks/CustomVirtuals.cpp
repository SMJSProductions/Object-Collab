#include "CustomVirtuals.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

#define CUSTOM_IMPLEMENT(source, ...) \
    if (source->m_objectID >= ObjectAPI::getBaseCustomObjectID()) { \
        if (CustomObjectInterface* custom = typeinfo_cast<CustomObjectInterface*>(source)) __VA_ARGS__; \
    }
#define ARTIFICIAL_VIRTUAL(method) \
    bool VirtualModGameObject::method() { \
        CUSTOM_IMPLEMENT(this, return custom->method()); \
        return GameObject::method(); \
    }

ARTIFICIAL_VIRTUAL(isSpawnableTrigger);
ARTIFICIAL_VIRTUAL(isSpecialObject);
ARTIFICIAL_VIRTUAL(isTrigger);
ARTIFICIAL_VIRTUAL(shouldLockX);

void VirtualModEnhancedGameObject::setupAnimationVariables() {
    EnhancedGameObject::setupAnimationVariables();

    CUSTOM_IMPLEMENT(this, {
        const bool isFrozen = custom->usesFreezeAnimation();

        if (!isFrozen || m_disableDelayedLoop) {
            m_isDisabled2 = false;
        }

        m_visible = isFrozen;
        m_shouldNotHideAnimFreeze = custom->shouldNotHideAnimFreeze();
        m_usesSpecialAnimation = custom->usesSpecialAnimation();
        
        return;
    });
}

void VirtualModPlayLayer::addObject(GameObject* object) {
    CUSTOM_IMPLEMENT(object, if (custom->isSettingsObject()) return);

    PlayLayer::addObject(object);
}

void VirtualModLevelEditorLayer::addSpecial(GameObject* object) {
    LevelEditorLayer::addSpecial(object);

    CUSTOM_IMPLEMENT(object, if (custom->isColorTrigger()) {
        m_colorTriggers->addObject(object);
        m_colorTriggersChanged = true;
    });
}

void VirtualModLevelEditorLayer::removeSpecial(GameObject* object) {
    LevelEditorLayer::removeSpecial(object);

    CUSTOM_IMPLEMENT(object, if (custom->isColorTrigger()) {
        m_colorTriggers->removeObject(object);
    });
}