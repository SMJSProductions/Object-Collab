#pragma once

#include <object_collab.hpp>
#include "DownloaderPopup.hpp"
#include "utils/NewBorder.hpp"
#include "../utils/makeVector.hpp"
#include <vector>

class CompatPopup : public PopupExtra {
    enum class CurrentModState {
        DisabledMissing,
        Disabled,
        Missing
    };
public:
    static CompatPopup* create(geode::Function<void()> callback);
private:
    static CurrentModState getModState();

    const CurrentModState m_state;
    geode::Function<void()> m_callback;

    CompatPopup(geode::Function<void()> callback);
    bool init() override;
    cocos2d::CCNode* getModLogo(std::string modID, geode::Mod* mod);
    void onFix();
    void closeWithEnabled();
    void onClose(cocos2d::CCObject* object) override;
};