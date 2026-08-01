#pragma once

#include <object_collab.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>

class $modify(ModEditorPauseLayer, EditorPauseLayer) {
    static void onModify(auto& self) {
        (void) self.setHookPriorityBeforePre("EditorPauseLayer::saveLevel", "alphalaneous.level-storage-api");
    }

    $override bool init(LevelEditorLayer* layer);
    $override void saveLevel();
};