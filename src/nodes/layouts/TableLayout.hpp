#pragma once

#include <Geode/Geode.hpp>

class TableLayout : public geode::Layout {
public:
    static TableLayout* create(const geode::Axis axis);
private:
    geode::Axis m_axis;
    cocos2d::CCSize m_gap;
    float m_minScale;
    float m_maxScale;
    geode::Padding m_padding;
    bool m_scaleGaps;
    bool m_inverseMainAxis;
    bool m_inverseCrossAxis;
public:
    TableLayout(const geode::Axis axis);
    TableLayout* setGap(const float gap);
    TableLayout* setGap(cocos2d::CCSize gap);
    TableLayout* setMainAxisGap(const float gap);
    TableLayout* setCrossAxisGap(const float gap);
    TableLayout* setMinScale(const float min);
    TableLayout* setMaxScale(const float max);
    TableLayout* setPadding(geode::Padding padding);
    TableLayout* scaleGaps(const bool toggle);
    TableLayout* inverseMainAxis(const bool toggle);
    TableLayout* inverseCrossAxis(const bool toggle);

    cocos2d::CCSize getSizeHint(cocos2d::CCNode* on) const override;
    void apply(cocos2d::CCNode* on) override;
private:
    void applyWithConfiguration(cocos2d::CCNode* on, const float scale, const size_t blocks);
    bool testConfiguration(cocos2d::CCNode* on, const float scale, const size_t blocks);
    size_t calculateCrossAxisBlocks(cocos2d::CCNode* on, const float scale);
    float getInsetForAxis(const bool inverse = false);
    float getGapForAxis(const float scale, const bool inverse = false);
    float getContentSizeForAxis(cocos2d::CCNode* from, const float scale, const bool inverse = false);
    float getSizeHintForAxis(cocos2d::CCNode* on, const bool inverse = false);
    float getBlockSizeForAxis(cocos2d::CCNode* on, const size_t blocks, const float scale, const bool inverse = false);
    float getWidthHint(cocos2d::CCNode* on) const;
    float getHeightHint(cocos2d::CCNode* on) const;
};