#include "NewBorder.hpp"

using namespace geode::prelude;

NewBorder* NewBorder::create(const CCSize& size, const ccColor4B& backgroundColor, const Axis axis) {
    NewBorder* border = new NewBorder();

    if (border && border->init(size, backgroundColor, axis)) {
        border->autorelease();

        return border;
    } else {
        delete border;

        return nullptr;
    }
}

bool NewBorder::init(const CCSize& size, const ccColor4B& backgroundColor, const Axis axis) {
    if (!this->initWithColor(backgroundColor)) return false;

    m_mainLayer = CCLayer::create();
    m_border = NineSlice::create("geode.loader/inverseborder.png");

    m_mainLayer->setAnchorPoint({ 0, 0 });
    m_border->setAnchorPoint({ 0, 0 });

    this->addChild(m_mainLayer);
    this->addChild(m_border);
    this->setContentSize(size);
    this->ignoreAnchorPointForPosition(false);
    m_mainLayer->setLayout(SimpleAxisLayout::create(axis)
        ->setMainAxisScaling(AxisScaling::None)
        ->setCrossAxisScaling(AxisScaling::None)
        ->setCrossAxisAlignment(CrossAxisAlignment::Center));

    return true;
}

void NewBorder::addChild(CCNode* child) {
    if (child == m_mainLayer || child == m_border) {
        CCLayerColor::addChild(child);
    } else {
        m_mainLayer->addChild(child);
    }
}

void NewBorder::addChild(CCNode* child, const int zOrder) {
    if (child == m_mainLayer || child == m_border) {
        CCLayerColor::addChild(child, zOrder);
    } else {
        m_mainLayer->addChild(child, zOrder);
    }
}

void NewBorder::addChild(CCNode* child, const int zOrder, const int tag) {
    if (child == m_mainLayer || child == m_border) {
        CCLayerColor::addChild(child, zOrder, tag);
    } else {
        m_mainLayer->addChild(child, zOrder, tag);
    }
}

void NewBorder::addChildAtPosition(CCNode* child, const Anchor anchor, const CCPoint& offset, const bool useAnchorLayout) {
    m_mainLayer->addChildAtPosition(child, anchor, offset, useAnchorLayout);
}

SimpleAxisLayout* NewBorder::getLayout() {
    return reinterpret_cast<SimpleAxisLayout*>(m_mainLayer->getLayout());
}

void NewBorder::setLayout(Layout* layout, const bool apply, const bool respectAnchor) {
    m_mainLayer->setLayout(layout, apply, respectAnchor);
}

void NewBorder::updateLayout(const bool updateChildOrder) {
    m_mainLayer->updateLayout(updateChildOrder);
}

void NewBorder::setContentWidth(const float width) {
    m_border->setContentWidth(width);
    m_mainLayer->setContentWidth(width);

    CCLayerColor::setContentWidth(width);
}

void NewBorder::setContentHeight(const float height) {
    m_border->setContentHeight(height);
    m_mainLayer->setContentHeight(height);

    CCLayerColor::setContentHeight(height);
}

void NewBorder::setContentSize(const CCSize& size) {
    if (m_border) m_border->setContentSize(size);
    if (m_mainLayer) m_mainLayer->setContentSize(size);

    CCLayerColor::setContentSize(size);
}