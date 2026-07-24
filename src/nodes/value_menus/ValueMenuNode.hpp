#pragma once

#include <Geode/Geode.hpp>

class ValueMenuNode : public cocos2d::CCMenu {
    cocos2d::CCNode* m_valueNode;
    cocos2d::CCLabelBMFont* m_titleNode;
public:
    template<typename T> requires std::is_assignable_v<cocos2d::CCNode, std::remove_pointer_t<T>>
    std::remove_pointer_t<T>* getValueNode() {
        return reinterpret_cast<std::remove_pointer_t<T>*>(m_valueNode);
    }

    cocos2d::CCLabelBMFont* getTitle() {
        return m_titleNode;
    }
protected:
    bool initBaseMenu(const geode::ZStringView id, const geode::ZStringView title, const std::vector<CCNode*>& nodes);
    bool initBaseMenuInverted(const geode::ZStringView id, const geode::ZStringView title, CCNode* titleButton, const std::vector<CCNode*>& nodes);
    geode::SimpleAxisLayout* createStandardLayout(const geode::Axis axis, const float gap = 10);
    cocos2d::CCLabelBMFont* createMenuTitle(const geode::ZStringView title);
    void registerValue(cocos2d::CCNode* valueNode);
    void registerTitle(cocos2d::CCLabelBMFont* titleNode);
};