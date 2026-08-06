#include "CustomVirtuals.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

bool VirtualModGameObject::isSpawnableTrigger() {
    OBJECT_CUSTOM_IMPLEMENT(this, custom, return custom->isTriggerObject() && custom->getTraits().isSpawnableTrigger());

    return GameObject::isSpawnableTrigger();
}

bool VirtualModGameObject::isSpecialObject() {
    OBJECT_CUSTOM_IMPLEMENT(this, custom, return custom->getTraits().omitTrashTexture());

    return GameObject::isSpecialObject();
}

bool VirtualModGameObject::isTrigger() {
    OBJECT_CUSTOM_IMPLEMENT(this, custom, return custom->isTriggerObject());

    return GameObject::isTrigger();
}

bool VirtualModGameObject::shouldLockX() {
    OBJECT_CUSTOM_IMPLEMENT(this, custom, return custom->getTraits().shouldLockX());

    return GameObject::shouldLockX();
}

void VirtualModGameObject::playShineEffect() {
    OBJECT_CUSTOM_IMPLEMENT(this, custom, return custom->getTraits().playShineEffect([this]() { GameObject::playShineEffect(); }));

    GameObject::playShineEffect();
}

// ARTIFICIAL_VIRTUAL(void, playShineEffect);

void VirtualModEnhancedGameObject::setupAnimationVariables() {
    EnhancedGameObject::setupAnimationVariables();

    OBJECT_CUSTOM_IMPLEMENT(this, custom, {
        const ObjectTraits& traits = custom->getTraits();
        const bool isFrozen = traits.usesFreezeAnimation();

        if (!isFrozen || m_disableDelayedLoop) {
            m_isDisabled2 = false;
        }

        m_visible = isFrozen;
        m_shouldNotHideAnimFreeze = traits.shouldNotHideAnimFreeze();
        m_usesSpecialAnimation = traits.usesSpecialAnimation();
    });
}

// COLOR TRIGGER HANDLING

void VirtualModLevelEditorLayer::addSpecial(GameObject* object) {
    LevelEditorLayer::addSpecial(object);

    OBJECT_CUSTOM_IMPLEMENT(object, custom, if (custom->isTriggerObject() && custom->getTraits().isColorTrigger()) {
        m_colorTriggers->addObject(object);
        m_colorTriggersChanged = true;
    });
}

void VirtualModLevelEditorLayer::removeSpecial(GameObject* object) {
    LevelEditorLayer::removeSpecial(object);

    OBJECT_CUSTOM_IMPLEMENT(object, custom, if (custom->isTriggerObject() && custom->getTraits().isColorTrigger()) {
        m_colorTriggers->removeObject(object);
    });
}

// COLLISION REPORTING

void VirtualModPlayLayer::destroyPlayer(PlayerObject* player, GameObject* object) {
    PlayLayer::destroyPlayer(player, object);

    OBJECT_CUSTOM_IMPLEMENT(object, custom, custom->collidedByPlayer(player));
}

void VirtualModLevelEditorLayer::playerTookDamage(PlayerObject* player) {
    LevelEditorLayer::playerTookDamage(player);

    for (GameObject* object : m_hazardCollisionObjects) {
        OBJECT_CUSTOM_IMPLEMENT(object, custom, custom->collidedByPlayer(player));
    }
}

bool VirtualModPlayerObject::collidedWithObjectInternal(const float dt, GameObject* object, CCRect rect, const bool skipCheck) {
    if (PlayerObject::collidedWithObjectInternal(dt, object, std::move(rect), skipCheck)) {
        OBJECT_CUSTOM_IMPLEMENT(object, custom, custom->collidedByPlayer(this));

        return true;
    }

    return false;
}

void VirtualModPlayerObject::collidedWithSlopeInternal(const float dt, GameObject* object, const bool forced) {
    PlayerObject::collidedWithSlopeInternal(dt, object, forced);

    OBJECT_CUSTOM_IMPLEMENT(object, custom, custom->collidedByPlayer(this));
}

// SPEED HANDLING

// void VirtualModEffectGameObject::updateSpeedModType() {
//     CUSTOM_IMPLEMENT(this, if (custom->isTrigger() && custom->isSpeedObject()) {
//         m_speedModType = custom->getSpeedMod();

//         return;
//     });

//     EffectGameObject::updateSpeedModType();
// }

void VirtualModPlayLayer::addObject(GameObject* object) {
    OBJECT_CUSTOM_IMPLEMENT(object, custom, {
        if (custom->getTraits().isEditorReserved()) return;

        // PlayLayer::addObject(object);

        // if (custom->isTrigger() && custom->isSpeedObject()) {
        //     EffectGameObject* effectObject = typeinfo_cast<EffectGameObject*>(custom->getGameObject());

        //     if (effectObject->m_cameraDisableGridSnap) {
        //         effectObject->updateSpeedModType();
        //         this->m_speedObjects->addObject(effectObject);
        //     }
        // }

        // return;
    });

    PlayLayer::addObject(object);
}

// bool VirtualModLevelEditorLayer::tryUpdateSpeedObject(EffectGameObject* object, const bool noPreview) {
//     CUSTOM_IMPLEMENT(object, if (custom->isTrigger() && custom->isSpeedObject()) {
//         EffectGameObject* effectObject = typeinfo_cast<EffectGameObject*>(custom->getGameObject());

//         if (effectObject->m_shouldPreview && !noPreview) {
//             m_drawGridLayer->addToSpeedObjects(effectObject);
//         } else {
//             m_drawGridLayer->removeFromSpeedObjects(effectObject);
//         }
//     });

//     return LevelEditorLayer::tryUpdateSpeedObject(object, noPreview);
// }

// void VirtualModEditorUI::moveObject(GameObject* object, cocos2d::CCPoint offset) {
//     EditorUI::moveObject(object, std::move(offset));

//     if (!m_speedObjectsUpdated) {
//         CUSTOM_IMPLEMENT(object, if (custom->isTrigger() && custom->isSpeedObject()) {
//             m_speedObjectsUpdated = true;
//         });
//     }
// }

// void VirtualModEditorUI::onCreateObject(const int id) {
//     GameObject* object = nullptr;

//     EditorUI::onCreateObject(id);

//     if (m_selectedObject) {
//         object = m_selectedObject;
//     } else if (m_selectedObjects->count()) {
//         object = reinterpret_cast<GameObject*>(m_selectedObjects->objectAtIndex(m_selectedObjects->count() - 1));
//     } else {
//         return;
//     }

//     CUSTOM_IMPLEMENT(object, if (custom->isTrigger() && custom->isSpeedObject()) {
//         EffectGameObject* effectObject = typeinfo_cast<EffectGameObject*>(custom->getGameObject());
//         effectObject->m_cameraDisableGridSnap = true;

//         m_editorLayer->m_drawGridLayer->addToSpeedObjects(effectObject);
//         this->updateSlider();
//     });
// }

// ACTION COMMANDS

// void VirtualModGJEffectManager::controlActionsForControlID(int id, GJActionCommand command) {
//     GJEffectManager::controlActionsForControlID(id, command);

//     for (const GroupCommandObject2& command : m_unkVector560) {
//         if (command.m_controlID != id) continue;

//         CUSTOM_IMPLEMENT(command.m_gameObject, custom, {
            
//         })
//     }
// }

// void VirtualModGJEffectManager::controlActionsForTrigger(EffectGameObject* object, GJActionCommand command) {
//     GJEffectManager::controlActionsForTrigger(object, command);
// }