#include "CustomVirtuals.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

bool VirtualModGameObject::isSpawnableTrigger() {
    CUSTOM_OBJECT_IMPLEMENT(this, CustomObjectInterface, custom, return custom->isTriggerObject() && custom->getTraits().isSpawnableTrigger());

    return GameObject::isSpawnableTrigger();
}

bool VirtualModGameObject::isSpecialObject() {
    CUSTOM_OBJECT_IMPLEMENT(this, CustomObjectInterface, custom, return custom->getTraits().omitTrashTexture());

    return GameObject::isSpecialObject();
}

bool VirtualModGameObject::isTrigger() {
    CUSTOM_OBJECT_IMPLEMENT(this, CustomObjectInterface, custom, return custom->isTriggerObject());

    return GameObject::isTrigger();
}

bool VirtualModGameObject::shouldLockX() {
    CUSTOM_OBJECT_IMPLEMENT(this, CustomObjectInterface, custom, {
        return custom->isTriggerObject() || custom->getTraits().shouldLockX();
    });

    return GameObject::shouldLockX();
}

void VirtualModGameObject::playShineEffect() {
    CUSTOM_OBJECT_IMPLEMENT(this, CustomObjectInterface, custom, return custom->getTraits().playShineEffect([this]() { GameObject::playShineEffect(); }));

    GameObject::playShineEffect();
}

void VirtualModEnhancedGameObject::setupAnimationVariables() {
    EnhancedGameObject::setupAnimationVariables();

    CUSTOM_OBJECT_IMPLEMENT(this, CustomObjectInterface, custom, {
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