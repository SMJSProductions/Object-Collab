#include "ToggleMenuNode.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

ToggleMenuNode* ToggleMenuNode::create(const Selected& selected, Popup* popup, ToggleMenu& toggleMenu) {
    ToggleMenuNode* menu = new ToggleMenuNode();

    if (menu && menu->init(selected, popup, toggleMenu)) {
        menu->autorelease();

        return menu;
    } else {
        delete menu;

        return nullptr;
    }
}

bool ToggleMenuNode::init(const Selected& selected, Popup* popup, ToggleMenu& toggleMenu) {
    if (!ValueMenuNode::init()) return false;

    CurrentValueCallback<bool> currentValueCallback = toggleMenu.releaseCurrentValue();
    CCMenuItemToggler* toggler = CCMenuItemExt::createTogglerWithStandardSprites(0.7f, [
        selected, popup, onValue = toggleMenu.releaseOnValue()
    ](CCMenuItemToggler* toggler) mutable {
        if (onValue) onValue(!toggler->isToggled(), selected, popup);
    });
    CCLabelBMFont* label = CCLabelBMFont::create(toggleMenu.getTitle().c_str(), "bigFont.fnt");

    label->setScale(0.35f);
    this->registerValue(toggler);
    this->registerTitle(label);
    this->setID(toggleMenu.getID());
    this->addChild(toggler);
    this->addChild(label);
    this->setContentHeight(toggler->getContentHeight());
    this->setLayout(this->createStandardLayout(Axis::Row)->setCrossAxisScaling(AxisScaling::None));
    toggler->toggle(currentValueCallback ? currentValueCallback(selected, popup) : false);

    return true;
}