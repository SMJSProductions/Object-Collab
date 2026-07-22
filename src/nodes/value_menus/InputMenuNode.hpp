#pragma once

#include <object_collab.hpp>
#include "ValueMenuNode.hpp"

class InputMenuNode : public ValueMenuNode {
public:
    static InputMenuNode* create(const object_collab::Selected& selected, geode::Popup* popup, object_collab::editor_popup::InputMenu& inputMenu);
private:
    bool init(const object_collab::Selected& selected, geode::Popup* popup, object_collab::editor_popup::InputMenu& inputMenu);
};