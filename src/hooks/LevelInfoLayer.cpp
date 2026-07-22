#include "LevelInfoLayer.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

void ModLevelInfoLayer::onPlay(CCObject* sender) {
    if (CCDirector::get()->m_bIsTransitioning) return ModLevelInfoLayer::onPlay(sender);

    CustomLevelData::ACTIVE = CustomLevelData::load(this);

    if (CustomLevelData::ACTIVE.getMissingMods().size()) {
        CompatPopup::create([this, sender]() { LevelInfoLayer::onPlay(sender); })->show();
    } else {
        LevelInfoLayer::onPlay(sender);
    }
}