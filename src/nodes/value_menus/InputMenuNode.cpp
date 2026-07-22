#include "InputMenuNode.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

InputMenuNode* InputMenuNode::create(const Selected& selected, Popup* popup, InputMenu& inputMenu) {
    InputMenuNode* menu = new InputMenuNode();

    if (menu && menu->init(selected, popup, inputMenu)) {
        menu->autorelease();

        return menu;
    } else {
        delete menu;

        return nullptr;
    }
}

bool InputMenuNode::init(const Selected& selected, Popup* popup, InputMenu& inputMenu) {
    std::string currentValue = inputMenu.releaseCurrentValue()(selected, popup);
    TextInput* input = TextInput::create(240, inputMenu.getPlaceholder().empty() ? ZStringView(currentValue) : inputMenu.getPlaceholder());
    CCSprite* clearSprite = CCSprite::createWithSpriteFrameName("GJ_resetBtn_001.png");
    CCMenuItemSpriteExtra* clearButton = CCMenuItemExt::createSpriteExtra(clearSprite, [input](CCMenuItemSpriteExtra* sender) {
        input->setString("", true);
    });

    this->registerValue(input);
    input->setString(std::move(currentValue));
    input->setFilter(inputMenu.getAllowedChars());
    input->setMaxCharCount(inputMenu.getMaxSize());
    input->setCallbackEnabled(true);
    input->setCallback([selected, popup, onValue = inputMenu.releaseOnValue()](const std::string& value) mutable {
        onValue(value, selected, popup);
    });
    clearButton->setID("clear");

    return this->initBaseMenuInverted(inputMenu.getID(), inputMenu.getTitle(), clearButton, { input });
}