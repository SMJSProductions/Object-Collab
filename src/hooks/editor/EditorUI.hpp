#pragma once

#include <object_collab.hpp>
#include <Geode/modify/EditorUI.hpp>
#include "../../nodes/EditorPopup.hpp"

class $modify(ModEditorUI, EditorUI) {
    static EditorUI* INSTANCE;
public:
    static void onModify(auto& self);

    static EditorUI* getEarly();

    ~ModEditorUI();
    $override bool init(LevelEditorLayer* editorLayer);
    $override void toggleEditObjectButton();
    $override void editObject(cocos2d::CCObject* sender);
    $override void editObjectSpecial(const int type);
    $override void updateObjectInfoLabel();
    void toggleButton(CCMenuItemSpriteExtra* button, const bool enabled);
    void fixEditSpecial(const std::optional<int>& selectedKey);
    std::optional<int> currentSelectedObjectID();
    std::vector<GameObject*> currentSelectedObjects();
};