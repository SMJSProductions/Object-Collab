#include "RainbowObject.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

RainbowObject* RainbowObject::create() {
    return new RainbowObject();
}

std::variant<Popup*, editor_popup::PopupConfig> RainbowObject::getEditSpecialConfig(const Selected& selected) {
    using namespace editor_popup;

    return PopupConfig::builder()
        .width(320)
        .height(150)
        .title("Rainbow Object")
        .triggerToggles(true)
        .info(InfoPopup::builder()
            .title("Test")
            .description("hi")
            .build())
        .menu(NumericMenu::builder()
            .title("Hue p/sec")
            .placeholder("Speed")
            .onValue([](const float value, const Selected& selected, Popup* popup) {
                applyValueToSelected(selected, &RainbowObject::m_targetSpeed, value);
            })
            .currentValue([](const Selected& selected, Popup* popup) {
                return getCommonValueOrDefault(selected, &RainbowObject::m_targetSpeed, 180);
            })
            .inputType(NumericMenu::InputType::SLIDER)
            .min(0)
            .max(360)
            .build())
        .toggle(ToggleMenu::builder()
            .title("enabled")
            .onValue([](const bool value, const Selected& selected, Popup* popup) {
                applyValueToSelected(selected, &RainbowObject::m_enabled, value);
            })
            .currentValue([](const Selected& selected, Popup* popup) {
                return getCommonValueOrDefault(selected, &RainbowObject::m_enabled, true);
            })
            .build())
        .build();
}

RainbowObject::RainbowObject(): CustomObject({
    CustomObject::propertyFrom(RainbowObject::ENABLED_KEY, m_enabled, true),
    CustomObject::propertyFrom(RainbowObject::SPEED_KEY, m_targetSpeed, 180)
}) { }

std::vector<std::string> RainbowObject::getObjectDetails() {
    return {
        fmt::format("Shift p/s: {}deg", m_targetSpeed),
        fmt::format("Enabled: {}", m_enabled ? "yes" : "no")
    };
}

void RainbowObject::postInit() {
    m_baseColor->m_usesHSV = true;
    m_baseColor->m_hsv.h = 0;

    this->scheduleUpdate();
}

void RainbowObject::triggerObject(GJBaseGameLayer* layer, const int uniqueID, const gd::vector<int>* remapKeys) {
    #define RAND_COLOR_CHANNEL static_cast<GLubyte>(std::rand() % 256)
    #define RAND_COLOR { RAND_COLOR_CHANNEL, RAND_COLOR_CHANNEL, RAND_COLOR_CHANNEL }
    const ccColor3B color1 = RAND_COLOR;
    const ccColor3B color2 = RAND_COLOR;

    layer->m_player1->setColor(color1);
    layer->m_player1->setSecondColor(color2);

    if (layer->m_player2) {
        layer->m_player1->setColor(color2);
        layer->m_player1->setSecondColor(color1);
    }
}

void RainbowObject::update(const float dt) {
    if (m_enabled) {
        m_baseColor->m_hsv.h = std::fmod(m_baseColor->m_hsv.h + m_targetSpeed * dt, 360);
    }
}

$on_game(Loaded) {
    ObjectAPI::registerObject(ObjectInfo::builder()
        .id("rainbow-object"_spr)
        .sprite("test.png"_spr)
        .factory(RainbowObject::create)
        .editSpecial(RainbowObject::getEditSpecialConfig)
        .build());
}