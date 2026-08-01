#pragma once

#include <object_collab.hpp>
#include "ValueMenuNode.hpp"

class EnumMenuNode : public ValueMenuNode {
public:
    static EnumMenuNode* create(const object_collab::Selected& selected, geode::Popup* popup, object_collab::editor_popup::EnumMenu& enumMenu);
private:
    const object_collab::Selected& m_selected;
    geode::Popup* m_popup;
    object_collab::editor_popup::ValueUpdateCallback<std::string> m_onValue;
    object_collab::editor_popup::EnumMenu::EnumValues m_values;
    CCMenuItemSpriteExtra* m_left;
    CCMenuItemSpriteExtra* m_right;
    cocos2d::CCLabelBMFont* m_label;
    size_t m_index;

    EnumMenuNode(const object_collab::Selected& selected, geode::Popup* popup, object_collab::editor_popup::EnumMenu& enumMenu);
    bool init(object_collab::editor_popup::EnumMenu& enumMenu);
    float getMaxLabelWidth();
    void onClick(CCMenuItemSpriteExtra* sender);
    size_t getSize();
    const char* getDisplayString(const size_t index);
    std::string_view getValueString(const size_t index);
};