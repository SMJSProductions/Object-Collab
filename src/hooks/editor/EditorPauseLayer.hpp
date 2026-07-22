#pragma once

#include <object_collab.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>

class $modify(ModEditorPauseLayer, EditorPauseLayer) {
    static void onModify(auto& self);

    $override bool init(LevelEditorLayer* layer);
    $override void saveLevel();
};