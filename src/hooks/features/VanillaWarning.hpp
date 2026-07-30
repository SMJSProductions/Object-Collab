#pragma once

#include <object_collab.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/LevelTools.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

class $modify(VanillaModPlayLayer, PlayLayer) {
    $override void prepareCreateObjectsFromSetup(gd::string& levelString);
};

class $modify(VanillaModLevelEditorLayer, LevelEditorLayer) {
    $override void createObjectsFromSetup(gd::string& levelString);
    $override gd::string getLevelString();
};