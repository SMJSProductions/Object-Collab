#include "TableLayout.hpp"

using namespace geode::prelude;

TableLayout* TableLayout::create(const Axis axis) {
    TableLayout* layout = new TableLayout(axis);

    layout->autorelease();

    return layout;
}

TableLayout::TableLayout(const Axis axis):
m_axis(axis),
m_gap(ccp(0, 0)),
m_minScale(0.5f),
m_maxScale(10),
m_scaleGaps(true),
m_inverseMainAxis(false),
m_inverseCrossAxis(false) { }

TableLayout* TableLayout::setGap(const float gap) {
    m_gap = ccp(gap, gap);

    return this;
}

TableLayout* TableLayout::setGap(CCSize gap) {
    m_gap = std::move(gap);

    return this;
}

TableLayout* TableLayout::setMainAxisGap(const float gap) {
    m_gap.width = gap;

    return this;
}

TableLayout* TableLayout::setCrossAxisGap(const float gap) {
    m_gap.height = gap;

    return this;
}

TableLayout* TableLayout::setMinScale(const float min) {
    m_minScale = min;

    return this;
}

TableLayout* TableLayout::setMaxScale(const float max) {
    m_maxScale = max;

    return this;
}

TableLayout* TableLayout::setPadding(Padding padding) {
    m_padding = std::move(padding);

    return this;
}

TableLayout* TableLayout::scaleGaps(const bool toggle) {
    m_scaleGaps = toggle;

    return this;
}

TableLayout* TableLayout::inverseMainAxis(const bool toggle) {
    m_inverseMainAxis = toggle;

    return this;
}

TableLayout* TableLayout::inverseCrossAxis(const bool toggle) {
    m_inverseCrossAxis = toggle;

    return this;
}

CCSize TableLayout::getSizeHint(CCNode* on) const {
    return { this->getWidthHint(on), this->getHeightHint(on) };
}

void TableLayout::apply(CCNode* on) {
    constexpr float SAMPLE_STEP_SIZE = 0.02f;
    float scale = m_maxScale;
    size_t requiredBlocks = 0;

    if (!on->getChildrenCount()) return;

    while (scale >= m_minScale) {
        if ((requiredBlocks = this->calculateCrossAxisBlocks(on, scale)) && this->testConfiguration(on, scale, requiredBlocks)) {
            break;
        } else if (scale - SAMPLE_STEP_SIZE > m_minScale) {
            scale = scale - SAMPLE_STEP_SIZE;
        } else {
            scale = m_minScale;

            break;
        }
    }

    this->applyWithConfiguration(on, scale, std::max<size_t>(requiredBlocks, 1));
}

void TableLayout::applyWithConfiguration(CCNode* on, const float scale, const size_t blocks) {
    const float parentMainAxis = this->getSizeHintForAxis(on);
    const float parentCrossAxis = this->getSizeHintForAxis(on, true);
    const float blockMainAxis = this->getBlockSizeForAxis(on, blocks, scale);
    const float mainAxisGap = this->getGapForAxis(scale);
    const float crossAxisGap = this->getGapForAxis(scale, true);
    const float mainAxisMultiplier = m_inverseMainAxis ? -1 : 1;
    const float crossAxisMultiplier = m_inverseCrossAxis ? -1 : 1;
    const float mainAxisReset = this->getInsetForAxis() + parentMainAxis * m_inverseMainAxis;
    float currentMainAxis = mainAxisReset;
    float currentCrossAxis = this->getInsetForAxis(true) + parentCrossAxis * m_inverseCrossAxis;
    float maxCrossAxis = 0;

    for (size_t i = 0; i < on->getChildrenCount(); i++) {
        CCNode* child = on->getChildByIndex(i);
        const float childMainAxis = this->getContentSizeForAxis(child, scale);
        const float childCrossAxis = this->getContentSizeForAxis(child, scale, true);

        if (i && i % blocks == 0) {
            currentMainAxis = mainAxisReset;
            currentCrossAxis += (maxCrossAxis + crossAxisGap) * crossAxisMultiplier;
            maxCrossAxis = childCrossAxis;
        } else if (childCrossAxis > maxCrossAxis) {
            maxCrossAxis = childCrossAxis;
        }

        child->setScale(scale);

        if (m_axis == Axis::Row) {
            child->setAnchorPoint({ 0.5f, 1 });
            child->setPosition(ccp(
                currentMainAxis + blockMainAxis / 2 * mainAxisMultiplier,
                currentCrossAxis + childCrossAxis * !m_inverseCrossAxis
            ));
        } else {
            child->setAnchorPoint({ 1, 0.5f });
            child->setPosition(ccp(
                currentCrossAxis + childCrossAxis * !m_inverseCrossAxis,
                currentMainAxis + blockMainAxis / 2 * mainAxisMultiplier
            ));
        }

        currentMainAxis += (blockMainAxis + mainAxisGap) * mainAxisMultiplier;
    }
}

bool TableLayout::testConfiguration(CCNode* on, const float scale, const size_t blocks) {
    const float parentCrossAxis = this->getSizeHintForAxis(on, true);
    float totalCrossAxis = 0;
    float maxCrossAxis = 0;

    for (size_t i = 0; i < on->getChildrenCount(); i++) {
        const float childCrossAxis = this->getContentSizeForAxis(on->getChildByIndex(i), scale, true);

        if (i && i % blocks == 0) {
            totalCrossAxis += maxCrossAxis + this->getGapForAxis(scale, true);
            maxCrossAxis = childCrossAxis;
        } else if (childCrossAxis > maxCrossAxis) {
            maxCrossAxis = childCrossAxis;
        }

        if (totalCrossAxis + maxCrossAxis > parentCrossAxis) return false;
    }

    return true;
}

size_t TableLayout::calculateCrossAxisBlocks(CCNode* on, const float scale) {
    const size_t childrenCount = on->getChildrenCount();
    size_t blocks = 1;

    for (float mainAxisSize = this->getBlockSizeForAxis(on, blocks, scale); mainAxisSize; mainAxisSize = this->getBlockSizeForAxis(on, ++blocks, scale)) {
        for (size_t i = 0; i < on->getChildrenCount(); i++) {
            if (this->getContentSizeForAxis(on->getChildByIndex(i), scale) > mainAxisSize) return blocks - 1;
        }

        // Don't try more if it matches the children count
        if (blocks == childrenCount) return blocks;
    }

    return 0;
}

float TableLayout::getInsetForAxis(const bool inverse) {
    return (m_axis == Axis::Row) != inverse ? m_padding.left : m_padding.bottom;
}

float TableLayout::getGapForAxis(const float scale, const bool inverse) {
    return ((m_axis == Axis::Row) != inverse ? m_gap.width : m_gap.height) * (scale * m_scaleGaps);
}

float TableLayout::getContentSizeForAxis(cocos2d::CCNode* from, const float scale, const bool inverse) {
    return ((m_axis == Axis::Row) != inverse ? from->getContentWidth() : from->getContentHeight()) * scale;
}

float TableLayout::getSizeHintForAxis(cocos2d::CCNode* on, const bool inverse) {
    return (m_axis == Axis::Row) != inverse ? this->getWidthHint(on) : this->getHeightHint(on);
}

float TableLayout::getBlockSizeForAxis(cocos2d::CCNode* on, const size_t blocks, const float scale, const bool inverse) {
    return (this->getSizeHintForAxis(on, inverse) - this->getGapForAxis(scale, inverse) * (blocks - 1)) / blocks;
}

float TableLayout::getWidthHint(CCNode* on) const {
    return on->getContentWidth() - m_padding.left - m_padding.right;
}

float TableLayout::getHeightHint(CCNode* on) const {
    return on->getContentHeight() - m_padding.top - m_padding.bottom;
}