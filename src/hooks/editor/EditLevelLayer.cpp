#include "EditLevelLayer.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

void ModEditLevelLayer::onEdit(CCObject* sender) {
    static bool skipCheck = false;

    if (skipCheck || CCDirector::get()->m_bIsTransitioning || CustomLevelData::load(this).getMissingMods().empty()) {
        skipCheck = false;

        EditLevelLayer::onEdit(sender);
    } else {
        CompatPopup::create([this, sender]() {
            skipCheck = true;

            EditLevelLayer::onEdit(sender);
        })->show();
    }
}

void ModEditLevelLayer::onPlay(CCObject* sender) {
    static bool skipCheck = false;

    if (skipCheck || CCDirector::get()->m_bIsTransitioning || CustomLevelData::load(this).getMissingMods().empty()) {
        skipCheck = false;

        EditLevelLayer::onPlay(sender);
    } else {
        CompatPopup::create([this, sender]() {
            skipCheck = true;

            EditLevelLayer::onPlay(sender);
        })->show();
    }
}