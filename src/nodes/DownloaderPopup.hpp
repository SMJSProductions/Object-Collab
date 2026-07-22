#pragma once

#include "utils/NewBorder.hpp"
#include "utils/PopupExtra.hpp"
#include "cells/ModDownloadCell.hpp"
#include "../utils/makeVector.hpp"
#include "../utils/DownloadManager.hpp"

class DownloaderPopup : public PopupExtra {
public:
    static DownloaderPopup* create(std::vector<std::string> modIDs);
private:
    geode::ScrollLayer* m_scrollLayer;
    arc::TaskHandle<> m_downloadTask;

    DownloaderPopup();
    bool init(std::vector<std::string> modIDs);
    void reportMod(const DownloadManager::ModMetadata& mod, const float progress);
    void keyBackClicked() override;
};