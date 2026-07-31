#include "AxisLayoutMenuNode.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

AxisLayoutMenuNode* AxisLayoutMenuNode::create(const Selected& selected, Popup* popup, AxisLayoutMenu& axisLayoutMenu) {
    AxisLayoutMenuNode* menu = new AxisLayoutMenuNode();

    if (menu && menu->init(selected, popup, axisLayoutMenu)) {
        menu->autorelease();

        return menu;
    } else {
        delete menu;

        return nullptr;
    }
}

bool AxisLayoutMenuNode::init(const Selected& selected, Popup* popup, AxisLayoutMenu& axisLayoutMenu) {
    CCNode* menuContainer = CCNode::create();

    for (const std::unique_ptr<ValueMenu>& menu : axisLayoutMenu.releaseMenus()) {
        if (ToggleMenu* toggleMenu = typeinfo_cast<ToggleMenu*>(menu.get())) {
            menuContainer->addChild(ToggleMenuNode::create(selected, popup, *toggleMenu));
        } else if (NumericMenu* numericMenu = typeinfo_cast<NumericMenu*>(menu.get())) {
            menuContainer->addChild(NumericMenuNode::create(selected, popup, *numericMenu));
        } else if (InputMenu* inputMenu = typeinfo_cast<InputMenu*>(menu.get())) {
            menuContainer->addChild(InputMenuNode::create(selected, popup, *inputMenu));
        } else if (EnumMenu* enumMenu = typeinfo_cast<EnumMenu*>(menu.get())) {
            menuContainer->addChild(EnumMenuNode::create(selected, popup, *enumMenu));
        } else if (CustomValueMenu* customValueMenu = typeinfo_cast<CustomValueMenu*>(menu.get())) {
            menuContainer->addChild(CustomValueMenuNode::create(selected, popup, *customValueMenu));
        } else if (AxisLayoutMenu* subAxisLayoutMenu = typeinfo_cast<AxisLayoutMenu*>(menu.get())) {
            menuContainer->addChild(AxisLayoutMenuNode::create(selected, popup, *subAxisLayoutMenu));
        }
    }

    menuContainer->setLayout(SimpleAxisLayout::create(axisLayoutMenu.getAxis())
        ->setMainAxisDirection(axisLayoutMenu.getMainAxisDirection())
        ->setMainAxisAlignment(axisLayoutMenu.getMainAxisAlignment())
        ->setMainAxisScaling(AxisScaling::Fit)
        ->setCrossAxisDirection(axisLayoutMenu.getCrossAxisDirection())
        ->setCrossAxisAlignment(axisLayoutMenu.getCrossAxisAlignment())
        ->setCrossAxisScaling(AxisScaling::Fit)
        ->setGap(axisLayoutMenu.getGap())
        ->ignoreInvisibleChildren(axisLayoutMenu.getIgnoreInvisibleChildren()));

    return this->initBaseMenuInverted(axisLayoutMenu.getID(), axisLayoutMenu.getTitle(), nullptr, {
        menuContainer
    });
}