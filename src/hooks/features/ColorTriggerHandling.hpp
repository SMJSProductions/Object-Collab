#pragma once

#include <object_collab.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

class $modify(ColorTriggerModLevelEditorLayer, LevelEditorLayer) {
    $override void addSpecial(GameObject* object);
    $override void removeSpecial(GameObject* object);
};