#include "CustomVirtuals.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

#define CUSTOM_IMPLEMENT(source, ...) \
    if (source && source->m_objectID >= ObjectAPI::getBaseCustomObjectID()) { \
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

void VirtualModPlayLayer::destroyPlayer(PlayerObject* player, GameObject* object) {
    PlayLayer::destroyPlayer(player, object);

    CUSTOM_IMPLEMENT(object, custom->collidedByPlayer(player));
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

void VirtualModLevelEditorLayer::playerTookDamage(PlayerObject* player) {
    LevelEditorLayer::playerTookDamage(player);

    for (GameObject* object : m_hazardCollisionObjects) {
        CUSTOM_IMPLEMENT(object, custom->collidedByPlayer(player));
    }
}

bool VirtualModPlayerObject::collidedWithObjectInternal(const float dt, GameObject* object, CCRect rect, const bool skipCheck) {
    if (PlayerObject::collidedWithObjectInternal(dt, object, std::move(rect), skipCheck)) {
        CUSTOM_IMPLEMENT(object, custom->collidedByPlayer(this));

        return true;
    }

    return false;
}

void VirtualModPlayerObject::collidedWithSlopeInternal(const float dt, GameObject* object, const bool forced) {
    PlayerObject::collidedWithSlopeInternal(dt, object, forced);

    CUSTOM_IMPLEMENT(object, custom->collidedByPlayer(this));
}