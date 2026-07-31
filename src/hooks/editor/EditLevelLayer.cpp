#include "EditLevelLayer.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

void ModEditLevelLayer::onEdit(CCObject* sender) {
    if (CCDirector::get()->m_bIsTransitioning) return EditLevelLayer::onEdit(sender);

    if (CustomLevelData::load(this).getMissingMods().size()) {
        CompatPopup::create([this, sender]() { EditLevelLayer::onEdit(sender); })->show();
    } else {
        EditLevelLayer::onEdit(sender);
    }
}

void ModEditLevelLayer::onPlay(CCObject* sender) {
    if (CCDirector::get()->m_bIsTransitioning) return EditLevelLayer::onPlay(sender);

    if (CustomLevelData::load(this).getMissingMods().size()) {
        CompatPopup::create([this, sender]() { EditLevelLayer::onPlay(sender); })->show();
    } else {
        EditLevelLayer::onPlay(sender);
    }
}