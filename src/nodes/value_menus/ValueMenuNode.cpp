#include "ValueMenuNode.hpp"

using namespace geode::prelude;

bool ValueMenuNode::initBaseMenu(const ZStringView id, const ZStringView title, const std::vector<CCNode*>& nodes) {
    if (!this->init()) return false;

    CCMenu* horizontalMenu = CCMenu::create();

    if (CCLabelBMFont* titleNode = this->createMenuTitle(title)) this->addChild(titleNode);

    for (CCNode* node : nodes) {
        horizontalMenu->addChild(node);
    }

    horizontalMenu->setLayout(this->createStandardLayout(Axis::Row));

    this->setID(id);
    this->addChild(horizontalMenu);
    this->setLayout(this->createStandardLayout(Axis::Column));

    return true;
}

bool ValueMenuNode::initBaseMenuInverted(const ZStringView id, const ZStringView title, CCNode* titleButton, const std::vector<CCNode*>& nodes) {
    if (!this->init()) return false;

    CCMenu* horizontalMenu = CCMenu::create();

    if (CCLabelBMFont* titleNode = this->createMenuTitle(title)) horizontalMenu->addChild(titleNode);
    if (titleButton) horizontalMenu->addChild(titleButton);

    if (horizontalMenu->getChildrenCount()) {
        horizontalMenu->setLayout(this->createStandardLayout(Axis::Row));

        this->addChild(horizontalMenu);
    }

    this->setID(id);

    for (CCNode* node : nodes) {
        this->addChild(node);
    }

    this->setLayout(this->createStandardLayout(Axis::Column));

    return true;
}

SimpleAxisLayout* ValueMenuNode::createStandardLayout(const Axis axis) {
    return SimpleAxisLayout::create(axis)
        ->setGap(10)
        ->setMainAxisScaling(AxisScaling::Fit)
        ->setCrossAxisScaling(AxisScaling::Fit);
}

CCLabelBMFont* ValueMenuNode::createMenuTitle(const ZStringView title) {
    if (title.size()) {
        CCLabelBMFont* label = CCLabelBMFont::create(title.c_str() , "goldFont.fnt");

        label->setScale(0.7f);
        this->registerTitle(label);

        return label;
    } else {
        return nullptr;
    }
}

void ValueMenuNode::registerValue(CCNode* valueNode) {
    valueNode->setID("value");
    m_valueNode = valueNode;
}

void ValueMenuNode::registerTitle(CCLabelBMFont* titleNode) {
    titleNode->setID("title");
    m_titleNode = titleNode;
}