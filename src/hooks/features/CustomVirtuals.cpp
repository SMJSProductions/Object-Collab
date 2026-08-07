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
    OBJECT_CUSTOM_IMPLEMENT(this, custom, {
        return custom->isTriggerObject() || custom->getTraits().shouldLockX();
    });

    return GameObject::shouldLockX();
}

void VirtualModGameObject::playShineEffect() {
    OBJECT_CUSTOM_IMPLEMENT(this, custom, return custom->getTraits().playShineEffect([this]() { GameObject::playShineEffect(); }));

    GameObject::playShineEffect();
}

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