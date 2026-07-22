#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include "../../utils/DownloadManager.hpp"

class ModDownloadCell : public cocos2d::CCLayer {
public:
    static ModDownloadCell* create(const DownloadManager::ModMetadata& mod, const cocos2d::CCSize& size);
private:
    Slider* m_slider;

    bool init(const DownloadManager::ModMetadata& mod, const cocos2d::CCSize& size);
public:
    void setProgress(const float progress);
};