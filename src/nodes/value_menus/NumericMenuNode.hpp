#pragma once

#include <object_collab.hpp>
#include <Geode/ui/SliderNode.hpp>
#include "ValueMenuNode.hpp"

class NumericMenuNode : public ValueMenuNode {
public:
    static NumericMenuNode* create(const object_collab::Selected& selected, geode::Popup* popup, object_collab::editor_popup::NumericMenu& numericMenu);
private:
    bool init(const object_collab::Selected& selected, geode::Popup* popup, object_collab::editor_popup::NumericMenu& numericMenu);
    geode::SliderNode* getSlider(
        const object_collab::Selected& selected,
        geode::Popup* popup,
        object_collab::editor_popup::NumericMenu& numericMenu,
        geode::TextInput* input,
        const float currentValue
    );
    std::pair<CCMenuItemSpriteExtra*, CCMenuItemSpriteExtra*> getArrows(object_collab::editor_popup::NumericMenu& numericMenu, geode::TextInput* input);
    void setFilter(object_collab::editor_popup::NumericMenu& numericMenu, geode::TextInput* input);
};