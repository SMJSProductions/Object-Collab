#pragma once

#include <object_collab.hpp>
#include <Geode/modify/GJEffectManager.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

class $modify(ActionCommandsModGJEffectManager, GJEffectManager) {
    $override void controlActionsForControlID(const int id, const GJActionCommand command);
};

class $modify(ActionCommandsModGJBaseGameLayer, GJBaseGameLayer) {
    $override void controlTriggersInGroup(const int group, const GJActionCommand command);
};