#pragma once

#include <object_collab.hpp>
#include "ValueMenuNode.hpp"

class CustomValueMenuNode : public ValueMenuNode {
public:
    static CustomValueMenuNode* create(const object_collab::Selected& selected, geode::Popup* popup, object_collab::editor_popup::CustomValueMenu& customValueMenu);
private:
    bool init(const object_collab::Selected& selected, geode::Popup* popup, object_collab::editor_popup::CustomValueMenu& customValueMenu);
};