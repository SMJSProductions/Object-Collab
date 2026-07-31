#include "EnumMenuNode.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

EnumMenuNode* EnumMenuNode::create(const Selected& selected, Popup* popup, EnumMenu& enumMenu) {
    EnumMenuNode* menu = new EnumMenuNode(selected, popup, enumMenu);

    if (menu && menu->init(enumMenu)) {
        menu->autorelease();

        return menu;
    } else {
        delete menu;

        return nullptr;
    }
}

EnumMenuNode::EnumMenuNode(const Selected& selected, Popup* popup, EnumMenu& enumMenu):
m_selected(selected),
m_popup(popup),
m_onValue(enumMenu.releaseOnValue()) {
    const std::span<std::string> values = enumMenu.getValues();

    m_index = values.size();
    m_values.assign(values.begin(), values.end());
}

bool EnumMenuNode::init(EnumMenu& enumMenu) {
    CCNode* container = CCNode::create();
    CCSprite* leftSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    CCSprite* rightSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    CurrentValueCallback<std::string> currentValueCallback = enumMenu.releaseCurrentValue();

    leftSprite->setScale(0.56f);
    rightSprite->setScale(0.56f);
    rightSprite->setFlipX(true);

    m_left = CCMenuItemExt::createSpriteExtra(leftSprite, [this](CCMenuItemSpriteExtra* sender) {
        this->onClick(sender);
    });
    m_right = CCMenuItemExt::createSpriteExtra(rightSprite, [this](CCMenuItemSpriteExtra* sender) {
        this->onClick(sender);
    });
    m_label = CCLabelBMFont::create(currentValueCallback ? currentValueCallback(m_selected, m_popup).c_str() : "", "bigFont.fnt");

    this->registerValue(m_label);
    m_left->setID("left");
    m_right->setID("right");
    m_label->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
    container->setScale(0.56f);
    container->setContentWidth(std::min(this->getMaxLabelWidth(), 300.0f));
    container->addChild(m_label);
    container->setLayout(SimpleRowLayout::create()
        ->setMainAxisAlignment(MainAxisAlignment::Center)
        ->setMainAxisScaling(AxisScaling::ScaleDown)
        ->setCrossAxisScaling(AxisScaling::Fit));

    return this->initBaseMenu(enumMenu.getID(), enumMenu.getTitle(), { m_left, container, m_right });
}

float EnumMenuNode::getMaxLabelWidth() {
    float maxWidth = m_label->getContentWidth();

    for (size_t i = 0; i < m_values.size(); i++) {
        const std::string_view value = m_values[i];
        const float width = cocos::getLabelSize(value, "bigFont.fnt").width;

        if (width > maxWidth) maxWidth = width;
        if (value.compare(m_label->getString()) == 0) m_index = i;
    }

    return maxWidth;
}

void EnumMenuNode::onClick(CCMenuItemSpriteExtra* sender) {
    const float width = m_label->getContentWidth();

    if (sender == m_left) {
        m_index = (m_index ? m_index : m_values.size()) - 1;
    } else {
        m_index = (m_index + (m_index == m_values.size() ? 0 : 1)) % m_values.size();
    }

    if (m_onValue) m_onValue(m_values[m_index], m_selected, m_popup);
    m_label->setString(m_values[m_index].c_str());
    m_label->getParent()->updateLayout();
}