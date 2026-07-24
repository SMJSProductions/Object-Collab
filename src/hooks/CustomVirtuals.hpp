#pragma once

#include <object_collab.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/EnhancedGameObject.hpp>

class $modify(VirtualModGameObject, GameObject) {
    $override bool isSpawnableTrigger();
    $override bool isSpecialObject();
    $override bool isTrigger();
    $override bool shouldLockX();
};

class $modify(VirtualModEnhancedGameObject, EnhancedGameObject) {
    $override void setupAnimationVariables();
};

class $modify(VirtualModPlayLayer, PlayLayer) {
    $override void addObject(GameObject* object);
};

class $modify(VirtualModLevelEditorLayer, LevelEditorLayer) {
    $override void addSpecial(GameObject* object);
    $override void removeSpecial(GameObject* object);
};