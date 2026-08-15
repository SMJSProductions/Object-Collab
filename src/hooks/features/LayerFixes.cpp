#include "LayerFixes.hpp"

using namespace geode::prelude;

void LayerFixesModGJBaseGameLayer::setupLayers() {
    GJBaseGameLayer::setupLayers();

    CCNodeContainer* container = CCNodeContainer::create();
    m_fields->m_specialPortalBackLayer = container;

    m_objectLayer->addChild(container, -5);
}

CCNode* LayerFixesModGJBaseGameLayer::parentForZLayer(const int zLayer, const bool blending, const int parentMode, const int uiObject) {
    if (!uiObject && parentMode == 4 && zLayer == 4) {
        return m_fields->m_specialPortalBackLayer;
    } else {
        return GJBaseGameLayer::parentForZLayer(zLayer, blending, parentMode, uiObject);
    }
}