#include "CompatPopup.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;
using namespace web;
using namespace arc;
using namespace matjson;

CompatPopup* CompatPopup::create(Function<void()> callback) {
    CompatPopup* popup = new CompatPopup(std::move(callback));

    if (popup && popup->init()) {
        popup->autorelease();

        return popup;
    } else {
        delete popup;

        return nullptr;
    }
}

CompatPopup::CurrentModState CompatPopup::getModState() {
    Loader* loader = Loader::get();
    std::optional<CompatPopup::CurrentModState> state;

    for (const std::string_view mod : CustomLevelData::get().getMissingMods()) {
        if (loader->isModInstalled(mod)) {
            if (state == CompatPopup::CurrentModState::Missing) {
                return CompatPopup::CurrentModState::DisabledMissing;
            } else {
                state = CompatPopup::CurrentModState::Disabled;
            }
        } else {
            if (state == CompatPopup::CurrentModState::Disabled) {
                return CompatPopup::CurrentModState::DisabledMissing;
            } else {
                state = CompatPopup::CurrentModState::Missing;
            }
        }
    }

    return state ? state.value() : CompatPopup::CurrentModState::DisabledMissing;
}

CompatPopup::CompatPopup(Function<void()> callback): PopupExtra(PopupExtra::CloseSetup::NoButton),
m_state(CompatPopup::getModState()),
m_callback(std::move(callback)) { }

bool CompatPopup::init() {
    static const std::unordered_map<CompatPopup::CurrentModState, std::pair<std::string, std::string>> STATE_STRINGS {
        { CompatPopup::CurrentModState::DisabledMissing, { "enabled and downloaded", "fix" } },
        { CompatPopup::CurrentModState::Disabled, { "enabled", "enable" } },
        { CompatPopup::CurrentModState::Missing, { "downloaded", "download" } }
    };
    const std::pair<std::string, std::string>& state = STATE_STRINGS.at(m_state);
    const CustomLevelData& customLevelData = CustomLevelData::get();

    if (!PopupExtra::init("Missing Mods!", { 350, 270 }, makeVector<PopupExtra::ButtonConfig>(
        PopupExtra::ButtonConfig{ "cancel", "GJ_button_06.png", [this](CCMenuItemSpriteExtra*) { this->closeWithEnabled(); } },
        PopupExtra::ButtonConfig{ state.second, "GJ_button_01.png", [this](CCMenuItemSpriteExtra*) { this->onFix(); } }
    ))) return false;

    const CCSize& contentSize = m_contentLayer->getContentSize();
    Loader* loader = Loader::get();
    CCLabelBMFont* label = CCLabelBMFont::create(
        fmt::format("This level requires objects of mods which you currently don't have {}.", state.first).c_str(),
        "chatFont.fnt",
        contentSize.width - 10
    );

    label->setScale(0.7f);

    NewBorder* textBorder = NewBorder::create({ contentSize.width, label->getScaledContentHeight() + 10 });
    std::string mdText;

    for (const std::string_view modID : customLevelData.getMissingMods()) {
        mdText.append(fmt::format("\n\n<mod:{}>", modID));
    }

    for (const std::string_view modID : customLevelData.getMods()) {
        mdText.append(fmt::format("\n\n<mod:{}>", modID));
    }

    CCLayer* modsContainer = CCLayer::create();
    NewBorder* modsBorder = NewBorder::create({ contentSize.width - 15, contentSize.height - textBorder->getContentHeight() - 10 });
    MDTextArea* textArea = MDTextArea::create(std::move(mdText), modsBorder->getContentSize() + ccp(10, 0));
    NineSlice* textAreaBg = textArea->getChildByType<NineSlice>(0);
    Scrollbar* scrollbar = Scrollbar::create(textArea->getChildByType<ScrollLayer>(0));

    this->setID("compat-popup"_spr);
    textBorder->addChild(label);
    textBorder->updateLayout();
    textAreaBg->setContentWidth(modsBorder->getContentWidth() / textAreaBg->getScaleX());
    scrollbar->getTrack()->setOpacity(0x40);
    modsBorder->addChildAtPosition(textArea, Anchor::Center);
    modsContainer->addChild(modsBorder);
    modsContainer->addChild(scrollbar);
    modsContainer->setLayout(SimpleRowLayout::create()
        ->setMainAxisScaling(AxisScaling::Fit)
        ->setCrossAxisScaling(AxisScaling::Fit)
        ->setGap(5));
    m_contentLayer->addChild(textBorder);
    m_contentLayer->addChild(modsContainer);
    m_contentLayer->updateLayout();

    return true;
}

void CompatPopup::onFix() {
    Loader* loader = Loader::get();
    std::vector<std::string> missingMods;

    for (const std::string_view modID : CustomLevelData::get().getMissingMods()) {
        if (Mod* mod = loader->getInstalledMod(modID)) {
            if (Result<> result = mod->enable(); result.isErr()) log::error("Error activating mod: {}", std::move(result).unwrapErr());
        } else {
            missingMods.emplace_back(modID);
        }
    }

    if (missingMods.size()) {
        DownloaderPopup::create(std::move(missingMods))->show();
    } else {
        this->closeWithEnabled();
    }
}

void CompatPopup::closeWithEnabled() {
    Loader* loader = Loader::get();

    for (const std::string_view modID : CustomLevelData::get().getMissingMods()) {
        if (Mod* mod = loader->getInstalledMod(modID); mod->isOrWillBeEnabled()) {
            geode::createQuickPopup(
                "Restart GD?",
                "To load the now enabled mods you must restart Geometry Dash.",
                "cancel",
                "restart",
                [this](FLAlertLayer*, const bool state) {
                    if (state) {
                        game::restart(true);
                    } else {
                        this->close();
                    }
                },
                true,
                true
            );

            return;
        }
    }

    this->close();
}

void CompatPopup::onClose(CCObject* object) {
    m_callback();
    PopupExtra::onClose(object);
}