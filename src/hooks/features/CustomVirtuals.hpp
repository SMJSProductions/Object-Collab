#pragma once

#include <object_collab.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/EnhancedGameObject.hpp>

class $modify(VirtualModGameObject, GameObject) {
    $override bool isSpawnableTrigger();
    $override bool isSpecialObject();
    $override bool isTrigger();
    $override bool shouldLockX();
    $override void playShineEffect();
};

class $modify(VirtualModEnhancedGameObject, EnhancedGameObject) {
    $override void setupAnimationVariables();
};