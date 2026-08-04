#include "CustomVirtuals.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

#define CUSTOM_IMPLEMENT(source, ...) \
    if (source && source->m_objectID >= ObjectAPI::getBaseCustomObjectID()) { \
        if (CustomObjectInterface* custom = typeinfo_cast<CustomObjectInterface*>(source)) __VA_ARGS__; \
    }
#define ARTIFICIAL_VIRTUAL(type, method) \
    type VirtualModGameObject::method() { \
        CUSTOM_IMPLEMENT(this, return custom->method()); \
        return GameObject::method(); \
    }

ARTIFICIAL_VIRTUAL(bool, isSpawnableTrigger);
ARTIFICIAL_VIRTUAL(bool, isSpecialObject);
ARTIFICIAL_VIRTUAL(bool, isTrigger);
ARTIFICIAL_VIRTUAL(bool, shouldLockX);
ARTIFICIAL_VIRTUAL(void, playShineEffect);

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

// COLOR TRIGGER HANDLING

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

// COLLISION REPORTING

void VirtualModPlayLayer::destroyPlayer(PlayerObject* player, GameObject* object) {
    PlayLayer::destroyPlayer(player, object);

    CUSTOM_IMPLEMENT(object, custom->collidedByPlayer(player));
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

// SPEED HANDLING

void VirtualModPlayLayer::addObject(GameObject* object) {
    PlayLayer::addObject(object);

    CUSTOM_IMPLEMENT(object, {
        if (custom->isSettingsObject()) return;

        if (custom->isTrigger() && custom->isSpeedObject()) {
            EffectGameObject* effectObject = typeinfo_cast<EffectGameObject*>(custom);

            if (effectObject->m_cameraDisableGridSnap) {
                effectObject->updateSpeedModType();
                this->m_speedObjects->addObject(effectObject);
            }
        }
    });
}

bool VirtualModLevelEditorLayer::tryUpdateSpeedObject(EffectGameObject* object, const bool noPreview) {
    CUSTOM_IMPLEMENT(object, if (custom->isTrigger() && custom->isSpeedObject()) {
        EffectGameObject* effectObject = typeinfo_cast<EffectGameObject*>(custom);

        if (effectObject->m_shouldPreview && !noPreview) {
            m_drawGridLayer->addToSpeedObjects(effectObject);
        } else {
            m_drawGridLayer->removeFromSpeedObjects(effectObject);
        }
    });

    return LevelEditorLayer::tryUpdateSpeedObject(object, noPreview);
}

void VirtualModPlayerObject::collidedWithSlopeInternal(const float dt, GameObject* object, const bool forced) {
    PlayerObject::collidedWithSlopeInternal(dt, object, forced);

    CUSTOM_IMPLEMENT(object, custom->collidedByPlayer(this));
}

void VirtualModEditorUI::moveObject(GameObject* object, cocos2d::CCPoint offset) {
    EditorUI::moveObject(object, std::move(offset));

    if (!m_speedObjectsUpdated) {
        CUSTOM_IMPLEMENT(object, if (custom->isTrigger() && custom->isSpeedObject()) {
            m_speedObjectsUpdated = true;
        });
    }
}

void VirtualModEditorUI::onCreateObject(const int id) {
    EditorUI::onCreateObject(id);

    CUSTOM_IMPLEMENT(reinterpret_cast<GameObject*>(m_selectedObjects->objectAtIndex(m_selectedObjects->count() - 1)), if (custom->isTrigger() && custom->isSpeedObject()) {
        EffectGameObject* effectObject = typeinfo_cast<EffectGameObject*>(custom);
        effectObject->m_cameraDisableGridSnap = true;

        m_editorLayer->m_drawGridLayer->addToSpeedObjects(effectObject);
        this->updateSlider();
    });
}