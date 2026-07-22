#pragma once

#include <object_collab.hpp>
#include "ValueMenuNode.hpp"

class ToggleMenuNode : public ValueMenuNode {
public:
    static ToggleMenuNode* create(const object_collab::Selected& selected, geode::Popup* popup, object_collab::editor_popup::ToggleMenu& toggleMenu);
private:
    bool init(const object_collab::Selected& selected, geode::Popup* popup, object_collab::editor_popup::ToggleMenu& toggleMenu);
};