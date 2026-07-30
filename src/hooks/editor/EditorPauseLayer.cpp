#include "EditorPauseLayer.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

void ModEditorPauseLayer::onModify(auto& self) {
    (void) self.setHookPriorityBeforePre("EditorPauseLayer::saveLevel", "alphalaneous.level-storage-api");
}

bool ModEditorPauseLayer::init(LevelEditorLayer* layer) {
    if (!EditorPauseLayer::init(layer)) return false;

    CCNode* infoMenu = this->getChildByID("info-menu");
    CCNode* settingsMenu = this->getChildByID("settings-menu");
    CCNode* actionsMenu = this->getChildByID("actions-menu");
    ButtonSprite* buttonSprite = ButtonSprite::create("Copy\nC-Data", 50, 0, 0.4f, true, "goldFont.fnt", "GJ_button_04.png", 30);
    const float topY = infoMenu->getPositionY() + infoMenu->getContentHeight() * (1 - infoMenu->getAnchorPoint().y);

    buttonSprite->setScale(0.8f);

    CCMenuItemSpriteExtra* button = CCMenuItemExt::createSpriteExtra(buttonSprite, [this, topY](CCMenuItemSpriteExtra* sender) {
        TextAlertPopup* alert = TextAlertPopup::create("Copied custom level data", 0.5f, 0.6f, 150, "");

        utils::clipboard::write(CustomLevelData::raw(m_editorLayer).dump(0));

        alert->setPosition({ this->getContentWidth() / 2, topY - alert->m_height * (1 - alert->getAnchorPoint().y) });
        this->addChild(alert, 100);
    });

    settingsMenu->setPositionY(topY - settingsMenu->getScaledContentHeight() * (1 - settingsMenu->getAnchorPoint().y));
    actionsMenu->insertBefore(button, actionsMenu->getChildByID("keys-button"));
    actionsMenu->updateLayout();

    return true;
}

void ModEditorPauseLayer::saveLevel() {
    CCArray* objectsBackup = m_editorLayer->m_objects;
    std::vector<CustomObjectInterface*> customObjects;

    m_editorLayer->m_objects = CCArray::create();
    m_editorLayer->m_objects->addObjectsFromArray(objectsBackup);

    for (size_t i = 0; i < m_editorLayer->m_objects->count(); i++) {
        if (CustomObjectInterface* object = typeinfo_cast<CustomObjectInterface*>(m_editorLayer->m_objects->objectAtIndex(i))) {
            customObjects.emplace_back(object);
            m_editorLayer->m_objects->removeObjectAtIndex(i--, false);
        }
    }

    if (customObjects.empty()) {
        CustomLevelData::clear(m_editorLayer);
    } else {
        CustomLevelData::save(m_editorLayer, customObjects);
    }

    EditorPauseLayer::saveLevel();

    m_editorLayer->m_objects = objectsBackup;
}