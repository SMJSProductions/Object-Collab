#include "DownloaderPopup.hpp"

using namespace geode::prelude;
using namespace arc;

DownloaderPopup* DownloaderPopup::create(std::vector<std::string> modIDs) {
    DownloaderPopup* popup = new DownloaderPopup();

    if (popup && popup->init(std::move(modIDs))) {
        popup->autorelease();

        return popup;
    } else {
        delete popup;

        return nullptr;
    }
}

DownloaderPopup::DownloaderPopup(): PopupExtra(PopupExtra::CloseSetup::NO_BUTTON) { }

bool DownloaderPopup::init(std::vector<std::string> modIDs) {
    if (!PopupExtra::init("Downloading Mods...", { 250, 250 }, makeVector<PopupExtra::ButtonConfig>(PopupExtra::ButtonConfig{
        "cancel",
        "GJ_button_06.png",
        [this](CCMenuItemSpriteExtra*) {
            m_downloadTask.abort();
            this->close();
        }
    }))) return false;

    const CCSize& contentSize = m_contentLayer->getContentSize();
    CCLayer* modsContainer = CCLayer::create();
    NewBorder* modsBorder = NewBorder::create({ contentSize.width - 15, contentSize.height });
    m_scrollLayer = ScrollLayer::create(modsBorder->getContentSize());
    Scrollbar* scrollbar = Scrollbar::create(m_scrollLayer);

    scrollbar->getTrack()->setOpacity(0x40);
    m_scrollLayer->m_contentLayer->setLayout(SimpleColumnLayout::create()
        ->setMainAxisDirection(AxisDirection::TopToBottom)
        ->setMainAxisAlignment(MainAxisAlignment::End)
        ->setMainAxisScaling(AxisScaling::Fit)
        ->setPadding({ 0, 5, 0, 5 })
        ->setGap(2.5f)
        ->ignoreInvisibleChildren(false));
    modsBorder->addChildAtPosition(m_scrollLayer, Anchor::Center);
    modsBorder->updateLayout();
    modsBorder->setColor({ 0x87, 0x50, 0x2C });
    modsContainer->addChild(modsBorder);
    modsContainer->addChild(scrollbar);
    modsContainer->setLayout(SimpleRowLayout::create()
        ->setMainAxisScaling(AxisScaling::Fit)
        ->setCrossAxisScaling(AxisScaling::Fit)
        ->setGap(5));
    m_contentLayer->addChild(modsContainer);
    m_contentLayer->updateLayout();

    m_downloadTask = arc::spawn([this](std::vector<std::string> modIDs) -> Future<void> {
        Result<> result = co_await DownloadManager::get()->downloadAll(std::move(modIDs), [this](const DownloadManager::ModMetadata& mod, const float progress) {
            this->reportMod(mod, progress);
        });

        co_return geode::queueInMainThread([this, result = std::move(result)] {
            this->setButtons(makeVector<PopupExtra::ButtonConfig>(PopupExtra::ButtonConfig{
                "ok",
                "GJ_button_01.png",
                [this](CCMenuItemSpriteExtra* sprite) {
                    geode::createQuickPopup(
                        "Restart GD?",
                        "To load the newly installed mods you must restart Geometry Dash.",
                        "cancel",
                        "restart",
                        [this](FLAlertLayer*, const bool state) {
                            if (state) {
                                game::restart(true);
                            } else {
                                this->close();
                                reinterpret_cast<PopupExtra*>(CCDirector::get()->getRunningScene()->getChildByID("compat-popup"_spr))->close();
                            }
                        },
                        true,
                        true
                    );
                }
            }));
        });
    }(std::move(modIDs)));

    return true;
}

void DownloaderPopup::reportMod(const DownloadManager::ModMetadata& mod, const float progress) {
    if (ModDownloadCell* cell = reinterpret_cast<ModDownloadCell*>(m_scrollLayer->m_contentLayer->getChildByID(mod.id))) {
        cell->setProgress(progress);
    } else {
        cell = ModDownloadCell::create(mod, { m_scrollLayer->getContentWidth() - 10, 30 });

        cell->setID(mod.id);
        m_scrollLayer->m_contentLayer->addChild(cell);
        m_scrollLayer->m_contentLayer->updateLayout();
        m_scrollLayer->scrollToTop();
    }
}

void DownloaderPopup::keyBackClicked() { }