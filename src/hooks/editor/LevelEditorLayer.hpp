#pragma once

#include <object_collab.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

class $modify(ModLevelEditorLayer, LevelEditorLayer) {
    $override void createObjectsFromSetup(gd::string& setup);
    $override cocos2d::CCArray* createObjectsFromString(const gd::string& str, const bool noUndo, const bool noLimit);
    $override GameObject* createObject(const int key, const cocos2d::CCPoint position, const bool noUndo);
};