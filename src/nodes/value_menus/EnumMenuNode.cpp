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
m_onValue(enumMenu.releaseOnValue()),
m_values(enumMenu.releaseValues()),
m_index(this->getSize()) { }

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
    const size_t size = this->getSize();
    const char* labelString = m_label->getString();
    float maxWidth = m_label->getContentWidth();

    for (size_t i = 0; i < size; i++) {
        const std::string_view display = this->getDisplayString(i);
        const float width = cocos::getLabelSize(display, "bigFont.fnt").width;

        if (width > maxWidth) maxWidth = width;
        if (
            display == labelString ||
            (std::holds_alternative<EnumMenu::EnumAliasList>(m_values) && std::get<EnumMenu::EnumAliasList>(m_values)[i].value == labelString)
        ) m_index = i;
    }

    return maxWidth;
}

void EnumMenuNode::onClick(CCMenuItemSpriteExtra* sender) {
    const size_t size = this->getSize();
    const float width = m_label->getContentWidth();

    if (sender == m_left) {
        m_index = (m_index ? m_index : size) - 1;
    } else {
        m_index = (m_index + (m_index == size ? 0 : 1)) % size;
    }

    if (m_onValue) m_onValue(std::string(this->getValueString(m_index)), m_selected, m_popup);
    m_label->setString(this->getDisplayString(m_index));
    m_label->getParent()->updateLayout();
}

size_t EnumMenuNode::getSize() {
    return std::visit(makeVisitor{
        [](const std::span<std::string> values) {
            return values.size();
        },
        [](const std::span<EnumMenu::AliasedValue> values) {
            return values.size();
        }
    }, m_values);
}

const char* EnumMenuNode::getDisplayString(const size_t index) {
    return std::visit(makeVisitor{
        [index](const std::span<std::string> values) {
            return values[index].c_str();
        },
        [index](const std::span<EnumMenu::AliasedValue> values) {
            return values[index].display.c_str();
        }
    }, m_values);
}

std::string_view EnumMenuNode::getValueString(const size_t index) {
    return std::visit<std::string_view>(makeVisitor{
        [index](const std::span<std::string> values) -> std::string_view {
            return values[index];
        },
        [index](const std::span<EnumMenu::AliasedValue> values) -> std::string_view {
            return values[index].value;
        }
    }, m_values);
}