#pragma once

#include <object_collab.hpp>
#include "ValueMenuNode.hpp"
#include "ToggleMenuNode.hpp"
#include "NumericMenuNode.hpp"
#include "InputMenuNode.hpp"
#include "EnumMenuNode.hpp"
#include "CustomValueMenuNode.hpp"
#include "AxisLayoutMenuNode.hpp"

class AxisLayoutMenuNode : public ValueMenuNode {
public:
    static AxisLayoutMenuNode* create(const object_collab::Selected& selected, geode::Popup* popup, object_collab::editor_popup::AxisLayoutMenu& axisLayoutMenu);
private:
    bool init(const object_collab::Selected& selected, geode::Popup* popup, object_collab::editor_popup::AxisLayoutMenu& axisLayoutMenu);
};