#pragma once

#include <object_collab.hpp>
#include <Geode/modify/GameObject.hpp>

class $modify(ModGameObject, GameObject) {
public:
    $override static GameObject* createWithKey(const int key);

    $override bool isTrigger();
    $override bool isSpawnableTrigger();
};