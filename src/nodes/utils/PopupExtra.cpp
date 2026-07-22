#include "PopupExtra.hpp"

using namespace geode::prelude;

PopupExtra::PopupExtra(const CloseSetup closeSetup): m_closeSetup(closeSetup) { }

bool PopupExtra::init(const ZStringView title, CCSize size, std::vector<ButtonConfig> buttons) {
    if (!Popup::init(std::move(size))) return false;
    if (m_closeSetup == CloseSetup::NO_BUTTON || m_closeSetup == CloseSetup::NONE) m_closeBtn->removeFromParent();

    this->setTitle(title);

    m_containerLayer = CCLayer::create();
    m_contentLayer = CCLayer::create();
    m_extraButtons = CCMenu::create();

    m_title->setZOrder(0);
    m_title->removeFromParentAndCleanup(false);
    m_contentLayer->setContentWidth(m_size.width - 20);
    m_contentLayer->setLayout(SimpleColumnLayout::create()
        ->setMainAxisAlignment(MainAxisAlignment::Start)
        ->setMainAxisScaling(AxisScaling::None)
        ->setCrossAxisScaling(AxisScaling::None)
        ->setGap(10));
    m_extraButtons->setLayout(SimpleRowLayout::create()
        ->setMainAxisScaling(AxisScaling::Fit)
        ->setCrossAxisScaling(AxisScaling::Fit)
        ->setGap(10));
    m_containerLayer->addChild(m_title);
    m_containerLayer->addChild(m_contentLayer);
    m_containerLayer->addChild(m_extraButtons);
    m_containerLayer->setContentSize(m_size);
    m_containerLayer->setLayout(SimpleColumnLayout::create()
        ->setMainAxisScaling(AxisScaling::None)
        ->setCrossAxisScaling(AxisScaling::None)
        ->setPadding({ 10, 10, 10, 10 })
        ->setGap(10));
    m_mainLayer->addChildAtPosition(m_containerLayer, Anchor::Center);

    this->setButtons(std::move(buttons));

    return true;
}

void PopupExtra::setButtons(std::vector<ButtonConfig> buttons) {
    m_extraButtons->removeAllChildren();

    for (ButtonConfig& button : buttons) {
        m_extraButtons->addChild(CCMenuItemExt::createSpriteExtra(ButtonSprite::create(
            button.title.c_str(),
            40,
            0,
            0.8f,
            true,
            "goldFont.fnt",
            button.sprite.c_str(),
            30
        ), std::move(button.onClick)));
    }

    m_extraButtons->updateLayout();
    m_contentLayer->setContentHeight(m_size.height - 40 - m_title->getScaledContentHeight() - m_extraButtons->getScaledContentHeight());
    m_containerLayer->updateLayout(false);
}

void PopupExtra::keyBackClicked() {
    if (m_closeSetup != CloseSetup::NO_KEYBIND && m_closeSetup != CloseSetup::NONE) Popup::keyBackClicked();
}

void PopupExtra::close() {
    this->onClose(nullptr);
}