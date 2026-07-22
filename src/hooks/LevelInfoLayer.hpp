#pragma once

#include <object_collab.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include "../nodes/CompatPopup.hpp"

class $modify(ModLevelInfoLayer, LevelInfoLayer) {
    $override void onPlay(cocos2d::CCObject* sender);
};