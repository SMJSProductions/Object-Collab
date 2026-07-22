#pragma once

#include <Geode/Geode.hpp>

class NewBorder : public cocos2d::CCLayerColor {
public:
    static NewBorder* create(
        const cocos2d::CCSize& size,
        const cocos2d::ccColor4B& backgroundColor = { 0xBF, 0x72, 0x3E, 0xFF },
        const geode::Axis axis = geode::Axis::Column
    );
private:
    geode::NineSlice* m_border;
    cocos2d::CCLayer* m_mainLayer;

    bool init(const cocos2d::CCSize& size, const cocos2d::ccColor4B& backgroundColor, const geode::Axis axis);
public:
    void addChild(CCNode* child) override;
    void addChild(CCNode* child, const int zOrder) override;
    void addChild(CCNode* child, const int zOrder, const int tag) override;
    void addChildAtPosition(CCNode* child, const geode::Anchor anchor, const cocos2d::CCPoint& offset = { 0, 0 }, const bool useAnchorLayout = true);
    geode::SimpleAxisLayout* getLayout();
    void setLayout(geode::Layout* layout, const bool apply = true, const bool respectAnchor = true);
    void updateLayout(const bool updateChildOrder = true);
    void setContentWidth(const float width);
    void setContentHeight(const float height);
    void setContentSize(const cocos2d::CCSize& size) override;
};