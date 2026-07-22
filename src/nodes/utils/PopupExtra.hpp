#pragma once

#include <Geode/Geode.hpp>

class PopupExtra : public geode::Popup {
public:
    enum class CloseSetup {
        DEFAULT,
        NO_BUTTON,
        NO_KEYBIND,
        NONE
    };

    struct ButtonConfig {
        geode::ZStringView title;
        geode::ZStringView sprite;
        geode::Function<void(CCMenuItemSpriteExtra*)> onClick;
    };

    cocos2d::CCLayer* m_contentLayer;
    cocos2d::CCLayer* m_containerLayer;
    cocos2d::CCMenu* m_extraButtons;
private:
    const CloseSetup m_closeSetup;
protected:
    PopupExtra(const CloseSetup closeSetup = CloseSetup::DEFAULT);
    bool init(const geode::ZStringView title, cocos2d::CCSize size, std::vector<ButtonConfig> buttons = {});
    void setButtons(std::vector<ButtonConfig> buttons);
    void keyBackClicked() override;
public:
    void close();
};