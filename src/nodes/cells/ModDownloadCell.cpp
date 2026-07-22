#include "ModDownloadCell.hpp"

using namespace geode::prelude;

ModDownloadCell* ModDownloadCell::create(const DownloadManager::ModMetadata& mod, const CCSize& size) {
    ModDownloadCell* cell = new ModDownloadCell();

    if (cell && cell->init(mod, size)) {
        cell->autorelease();

        return cell;
    } else {
        delete cell;

        return nullptr;
    }
}

bool ModDownloadCell::init(const DownloadManager::ModMetadata& mod, const CCSize& size) {
    if (!CCLayer::init()) return false;

    NineSlice* bg = NineSlice::create("square02b_001.png");
    CCNode* titleContainer = CCNode::create();
    CCNode* infoContainer = CCNode::create();
    CCNode* contentContainer = CCNode::create();
    CCLabelBMFont* name = CCLabelBMFont::create(mod.name.c_str(), "bigFont.fnt");
    CCLabelBMFont* version = CCLabelBMFont::create(mod.version.c_str(), "bigFont.fnt");
    CCNode* logo = geode::createServerModLogo(mod.id);
    m_slider = Slider::create(nullptr, nullptr);
    CCSprite* sliderGroove = m_slider->getChildByType<CCSprite>(0);

    bg->setOpacity(0x20);
    bg->setContentSize(size);
    logo->setScale((size.height - 10) / logo->getContentHeight());
    name->setScale(0.3f);
    version->setScale(0.25f);
    sliderGroove->setAnchorPoint({ 0, 0 });
    m_slider->setValue(0);
    m_slider->getChildByType<SliderTouchLogic>(0)->setVisible(false);
    m_slider->setContentSize(sliderGroove->getContentSize());
    m_slider->setScale((size.height - 13 - name->getScaledContentHeight()) / m_slider->getContentHeight());
    version->setColor({ 0x70, 0xEB, 0x29 });
    titleContainer->addChild(name);
    titleContainer->addChild(version);
    titleContainer->setLayout(SimpleRowLayout::create()
        ->setMainAxisScaling(AxisScaling::Fit)
        ->setCrossAxisScaling(AxisScaling::Fit)
        ->setGap(2));
    infoContainer->addChild(titleContainer);
    infoContainer->addChild(m_slider);
    infoContainer->setLayout(SimpleColumnLayout::create()
        ->setMainAxisScaling(AxisScaling::Fit)
        ->setCrossAxisAlignment(CrossAxisAlignment::Start)
        ->setCrossAxisScaling(AxisScaling::Fit)
        ->setGap(3));
    contentContainer->setContentSize(size);
    contentContainer->addChild(logo);
    contentContainer->addChild(infoContainer);
    contentContainer->setLayout(SimpleRowLayout::create()
        ->setMainAxisAlignment(MainAxisAlignment::Start)
        ->setMainAxisScaling(AxisScaling::None)
        ->setCrossAxisScaling(AxisScaling::None)
        ->setPadding({ 5, 5, 5, 5 })
        ->setGap(5));
    this->setContentSize(size);
    this->addChildAtPosition(bg, Anchor::Center);
    this->addChildAtPosition(contentContainer, Anchor::BottomLeft);

    return true;
}

void ModDownloadCell::setProgress(const float progress) {
    m_slider->setValue(progress);
}