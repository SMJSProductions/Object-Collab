#include "LevelInfoLayer.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

void ModLevelInfoLayer::onPlay(CCObject* sender) {
    static bool skipCheck = false;

    if (skipCheck || CCDirector::get()->m_bIsTransitioning || CustomLevelData::load(this).getMissingMods().empty()) {
        skipCheck = false;

        LevelInfoLayer::onPlay(sender);
    } else {
        CompatPopup::create([this, sender]() {
            skipCheck = true;

            LevelInfoLayer::onPlay(sender);
        })->show();
    }
}