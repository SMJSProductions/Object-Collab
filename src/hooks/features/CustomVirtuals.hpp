#pragma once

#include <object_collab.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/EffectGameObject.hpp>
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

class $modify(VirtualModEffectGameObject, EffectGameObject) {
    $override void updateSpeedModType();
};

class $modify(VirtualModPlayLayer, PlayLayer) {
    $override void addObject(GameObject* object);
    $override void destroyPlayer(PlayerObject* player, GameObject* object);
};

class $modify(VirtualModLevelEditorLayer, LevelEditorLayer) {
    $override void addSpecial(GameObject* object);
    $override void removeSpecial(GameObject* object);
    $override void playerTookDamage(PlayerObject* player);
    $override bool tryUpdateSpeedObject(EffectGameObject* object, const bool noPreview);
};

class $modify(VirtualModPlayerObject, PlayerObject) {
    $override bool collidedWithObjectInternal(const float dt, GameObject* object, cocos2d::CCRect rect, const bool skipCheck);
    $override void collidedWithSlopeInternal(const float dt, GameObject* object, const bool forced);
};

class $modify(VirtualModEditorUI, EditorUI) {
    $override void moveObject(GameObject* object, cocos2d::CCPoint offset);
    $override void onCreateObject(const int id);
};