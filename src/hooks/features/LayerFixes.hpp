#pragma once

#include <Geode/modify/GJBaseGameLayer.hpp>

class $modify(LayerFixesModGJBaseGameLayer, GJBaseGameLayer) {
    struct Fields {
        CCNodeContainer* m_specialPortalBackLayer;
    };

    $override void setupLayers();
    $override cocos2d::CCNode* parentForZLayer(const int zLayer, const bool blending, const int parentMode, const int uiObject);
};