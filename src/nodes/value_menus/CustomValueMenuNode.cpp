#include "CustomValueMenuNode.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

CustomValueMenuNode* CustomValueMenuNode::create(const Selected& selected, Popup* popup, CustomValueMenu& customValueMenu) {
    CustomValueMenuNode* menu = new CustomValueMenuNode();

    if (menu && menu->init(selected, popup, customValueMenu)) {
        menu->autorelease();

        return menu;
    } else {
        delete menu;

        return nullptr;
    }
}

bool CustomValueMenuNode::init(const Selected& selected, Popup* popup, CustomValueMenu& customValueMenu) {
    CustomValueMenu::CustomMenuFactory factory = customValueMenu.releaseFactory();
    std::vector<CCNode*> nodes;

    if (factory) {
        nodes.emplace_back(factory(selected, popup));
    }

    return this->initBaseMenuInverted(customValueMenu.getID(), customValueMenu.getTitle(), nullptr, std::move(nodes));
}