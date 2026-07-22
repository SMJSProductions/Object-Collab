#pragma once

#include <object_collab.hpp>
#include "utils/PopupExtra.hpp"
#include "layouts/TableLayout.hpp"
#include "value_menus/AxisLayoutMenuNode.hpp"
#include "../utils/makeVector.hpp"

class EditorPopup : public PopupExtra {
public:
    static EditorPopup* create(object_collab::Selected selected, object_collab::editor_popup::PopupConfig& config);
private:
    object_collab::Selected m_selected;

    EditorPopup(object_collab::Selected selected);

    bool init(object_collab::editor_popup::PopupConfig& config);
    void addInfo(const object_collab::editor_popup::InfoPopup& info);
    void addMenus(std::vector<std::unique_ptr<object_collab::editor_popup::ValueMenu>> menus);
    void addToggles(object_collab::editor_popup::PopupConfig& config);
    bool addTriggerToggles(cocos2d::CCNode* container);
    void addNoMultiActivateToggle(cocos2d::CCNode* container);
    void addMultiTriggerToggle(cocos2d::CCNode* container);
};