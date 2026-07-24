#include "NumericMenuNode.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

NumericMenuNode* NumericMenuNode::create(const Selected& selected, Popup* popup, NumericMenu& numericMenu) {
    NumericMenuNode* menu = new NumericMenuNode();

    if (menu && menu->init(selected, popup, numericMenu)) {
        menu->autorelease();

        return menu;
    } else {
        delete menu;

        return nullptr;
    }
}

bool NumericMenuNode::init(const Selected& selected, Popup* popup, NumericMenu& numericMenu) {
    const NumericMenu::InputType inputType = numericMenu.getInputType();
    const float currentValue = numericMenu.releaseCurrentValue()(selected, popup);
    std::string currentStringValue = utils::numToString(currentValue, numericMenu.getPrecision());
    std::vector<CCNode*> nodes;
    TextInput* input = TextInput::create(70, numericMenu.getPlaceholder().empty() ? ZStringView(currentStringValue) : numericMenu.getPlaceholder());

    this->setFilter(numericMenu, input);
    this->registerValue(input);

    if (inputType == NumericMenu::InputType::SLIDER) {
        nodes.emplace_back(this->getSlider(selected, popup, numericMenu, input, currentValue));
    } else {
        input->setString(std::move(currentStringValue));
        input->setCallbackEnabled(true);
        input->setCallback([selected, popup, onValue = numericMenu.releaseOnValue()](const std::string& strValue) mutable {
            float value;

            if (GEODE_UNWRAP_INTO_IF_OK(value, utils::numFromString<float>(strValue))) onValue(value, selected, popup);
        });
    }

    if (inputType == NumericMenu::InputType::ARROWS) {
        std::pair<CCMenuItemSpriteExtra*, CCMenuItemSpriteExtra*> arrows = this->getArrows(numericMenu, input);

        input->setScale(0.9f);

        return this->initBaseMenu(numericMenu.getID(), numericMenu.getTitle(), { arrows.first, input, arrows.second }); 
    } else {
        return this->initBaseMenuInverted(numericMenu.getID(), numericMenu.getTitle(), input, nodes);
    }
}

SliderNode* NumericMenuNode::getSlider(const Selected& selected, Popup* popup, NumericMenu& numericMenu, TextInput* input, const float currentValue) {
    SliderNode* slider = SliderNode::create([
        selected,
        popup,
        onValue = numericMenu.releaseOnValue()
    ](SliderNode* sender, const float value) mutable { onValue(value, selected, popup); });

    slider->setID("slider");
    slider->setMin(numericMenu.getMin().value_or(0));
    slider->setMax(numericMenu.getMax().value_or(100));
    slider->setValue(currentValue);
    slider->setSnapStep(numericMenu.getStepSize());
    slider->setScale(0.75f);
    slider->linkTextInput(input, numericMenu.getPrecision());

    return slider;
}

std::pair<CCMenuItemSpriteExtra*, CCMenuItemSpriteExtra*> NumericMenuNode::getArrows(NumericMenu& numericMenu, TextInput* input) {
    CCSprite* leftSprite = CCSprite::createWithSpriteFrameName("edit_leftBtn_001.png");
    CCSprite* rightSprite = CCSprite::createWithSpriteFrameName("edit_rightBtn_001.png");

    leftSprite->setScale(0.96f);
    rightSprite->setScale(0.96f);

    CCMenuItemSpriteExtra* left = CCMenuItemExt::createSpriteExtra(leftSprite, [
        stepSize = numericMenu.getStepSize(),
        min = numericMenu.getMin(),
        input
    ](CCMenuItemSpriteExtra* sender) {
        if (Result<float> result = utils::numFromString<float>(input->getString())) {
            const float number = std::move(result).unwrap() - stepSize;

            input->setString(utils::numToString(min ? std::max(min.value(), number) : number), true);
        }
    });
    CCMenuItemSpriteExtra* right = CCMenuItemExt::createSpriteExtra(rightSprite, [
        stepSize = numericMenu.getStepSize(),
        max = numericMenu.getMax(),
        input
    ](CCMenuItemSpriteExtra* sender) {
        if (Result<float> result = utils::numFromString<float>(input->getString())) {
            const float number = std::move(result).unwrap() + stepSize;

            input->setString(utils::numToString(max ? std::min(max.value(), number) : number), true);
        }
    });

    left->setID("left");
    right->setID("right");

    return std::make_pair(left, right);
}

void NumericMenuNode::setFilter(NumericMenu& numericMenu, TextInput* input) {
    if (numericMenu.getMin().value_or(numericMenu.getInputType() == NumericMenu::InputType::SLIDER ? 0 : -1) < 0) {
        input->setCommonFilter(numericMenu.getPrecision() ? CommonFilter::Float : CommonFilter::Int);
    } else if (numericMenu.getPrecision()) {
        input->setFilter(".0123456789");
    } else {
        input->setCommonFilter(CommonFilter::Uint);
    }
}