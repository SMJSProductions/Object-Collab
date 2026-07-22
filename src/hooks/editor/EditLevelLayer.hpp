#pragma once

#include <object_collab.hpp>
#include <Geode/modify/EditLevelLayer.hpp>
#include "../../nodes/CompatPopup.hpp"

class $modify(ModEditLevelLayer, EditLevelLayer) {
    $override void onEdit(cocos2d::CCObject* sender);
    $override void onPlay(cocos2d::CCObject* sender);
};