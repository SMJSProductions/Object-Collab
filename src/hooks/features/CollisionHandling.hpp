#pragma once

#include <object_collab.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

class $modify(CollisionModPlayLayer, PlayLayer) {
    $override void destroyPlayer(PlayerObject* player, GameObject* object);
};

class $modify(CollisionModLevelEditorLayer, LevelEditorLayer) {
    $override void playerTookDamage(PlayerObject* player);
};

class $modify(CollisionModPlayerObject, PlayerObject) {
    $override bool collidedWithObjectInternal(const float dt, GameObject* object, cocos2d::CCRect rect, const bool skipCheck);
    $override void collidedWithSlopeInternal(const float dt, GameObject* object, const bool forced);
};