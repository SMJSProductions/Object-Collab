#include "EditLevelLayer.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

void ModEditLevelLayer::onEdit(CCObject* sender) {
    if (CCDirector::get()->m_bIsTransitioning) return EditLevelLayer::onEdit(sender);

    CustomLevelData::ACTIVE = CustomLevelData::load(this);

    if (CustomLevelData::ACTIVE.getMissingMods().size()) {
        CompatPopup::create([this, sender]() { EditLevelLayer::onEdit(sender); })->show();
    } else {
        EditLevelLayer::onEdit(sender);
    }
}

void ModEditLevelLayer::onPlay(CCObject* sender) {
    if (CCDirector::get()->m_bIsTransitioning) return EditLevelLayer::onPlay(sender);

    CustomLevelData::ACTIVE = CustomLevelData::load(this);

    if (CustomLevelData::ACTIVE.getMissingMods().size()) {
        CompatPopup::create([this, sender]() { EditLevelLayer::onPlay(sender); })->show();
    } else {
        EditLevelLayer::onPlay(sender);
    }
}