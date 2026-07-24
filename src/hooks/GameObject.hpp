#pragma once

#include <object_collab.hpp>
#include <Geode/modify/GameObject.hpp>

class $modify(ModGameObject, GameObject) {
    $override static GameObject* createWithKey(const int key);
};