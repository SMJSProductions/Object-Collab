#include "ModSpawnTrigger.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

ModSpawnTrigger* ModSpawnTrigger::create() {
    return new ModSpawnTrigger();
}

PopupOptions ModSpawnTrigger::getEditObjectConfig(const Selected& selected) {
    std::vector<std::string> mods;

    for (Mod* mod : Loader::get()->getAllMods()) {
        mods.emplace_back(mod->getID());
    }

    return PopupConfig::builder()
        .width(320)
        .height(250)
        .gapY(20)
        .title("Loaded Mod Spawn Group")
        .info(InfoPopup::builder()
            .title("Help")
            .description("Works exactly like the spawn trigger with the condition that the targetted mod is installed & loaded.")
            .build())
        .triggerToggles(true)
        .menu(EnumMenu::builder()
            .id("mod-id"_spr)
            .title("Mod ID")
            .values(std::move(mods))
            .onValue([](const std::string& value, const Selected& selected, Popup* popup) {
                applyValueToSelectedAndReport(selected, &ModSpawnTrigger::m_mod, &ModSpawnTrigger::checkMod, value);
            })
            .currentValue([](const Selected& selected, Popup* popup) {
                return getCommonValueOrDefault(selected, &ModSpawnTrigger::m_mod, "Unknown");
            })
            .build())
        .menu(AxisLayoutMenu::builder()
            .axis(Axis::Row)
            .gap(20)
            .crossAxisAlignment(CrossAxisAlignment::Center)
            .menu(NumericMenu::builder()
                .id("group-id"_spr)
                .title("Group ID")
                .inputType(NumericMenu::InputType::ARROWS)
                .min(0)
                .max(9999)
                .precision(0)
                .placeholder("Num")
                .onValue([](const int value, const Selected& selected, Popup* popup) {
                    applyValueToSelected(selected, &ModSpawnTrigger::m_targetGroupID, value);
                })
                .currentValue([](const Selected& selected, Popup* popup) {
                    return getCommonValueOrDefault(selected, &ModSpawnTrigger::m_targetGroupID, 0);
                })
                .build())
            .menu(AxisLayoutMenu::builder()
                .axis(Axis::Column)
                .gap(10)
                .menu(ToggleMenu::builder()
                    .id("preview-disable"_spr)
                    .title("Preview\nDisable")
                    .onValue([](const bool value, const Selected& selected, Popup* popup) {
                        applyValueToSelected(selected, &ModSpawnTrigger::m_previewDisable, value);
                    })
                    .currentValue([](const Selected& selected, Popup* popup) {
                        return getCommonValueOrDefault(selected, &ModSpawnTrigger::m_previewDisable, false);
                    })
                    .build())
                .menu(ToggleMenu::builder()
                    .id("spawn-ordered"_spr)
                    .title("Spawn\nOrdered")
                    .onValue([](const bool value, const Selected& selected, Popup* popup) {
                        applyValueToSelected(selected, &ModSpawnTrigger::m_spawnOrdered, value);
                    })
                    .currentValue([](const Selected& selected, Popup* popup) {
                        return getCommonValueOrDefault(selected, &ModSpawnTrigger::m_spawnOrdered, false);
                    })
                    .build())
                .build())
            .build())
        .menu(AxisLayoutMenu::builder()
            .axis(Axis::Row)
            .gap(20)
            .menu(NumericMenu::builder()
                .id("delay"_spr)
                .title("Delay")
                .inputType(NumericMenu::InputType::SLIDER)
                .min(0)
                .max(1)
                .precision(4)
                .stepSize(0.0001)
                .placeholder("Num")
                .onValue([](const float value, const Selected& selected, Popup* popup) {
                    applyValueToSelected(selected, &ModSpawnTrigger::m_spawnDelay, value);
                })
                .currentValue([](const Selected& selected, Popup* popup) {
                    return getCommonValueOrDefault(selected, &ModSpawnTrigger::m_spawnDelay, 0);
                })
                .build())
            .menu(NumericMenu::builder()
                .id("+-"_spr)
                .title("+-")
                .inputType(NumericMenu::InputType::SLIDER)
                .min(0)
                .max(1)
                .precision(4)
                .stepSize(0.0001)
                .placeholder("Num")
                .onValue([](const float value, const Selected& selected, Popup* popup) {
                    applyValueToSelected(selected, &ModSpawnTrigger::m_delayRange, value);
                })
                .currentValue([](const Selected& selected, Popup* popup) {
                    return getCommonValueOrDefault(selected, &ModSpawnTrigger::m_delayRange, 0);
                })
                .build())
            .build())
        .build();
}

ModSpawnTrigger::ModSpawnTrigger(): CustomObject(GameObjectType::Modifier), m_mod("Unknown") { }

void ModSpawnTrigger::postInit() {
    m_duration = 0;

    this->setHitbox({ 1, 1 });
}

void ModSpawnTrigger::onAction(GJBaseGameLayer* layer, const int uniqueID, const gd::vector<int>* remapKeys) {
    if (m_active) {
        SpawnTriggerGameObject::triggerObject(layer, m_uniqueID, remapKeys);
    }
}

std::vector<std::string> ModSpawnTrigger::getObjectDetails() {
    return {
        fmt::format("Mod ID: {}", m_mod),
        fmt::format("Group ID: {}", m_targetGroupID),
        fmt::format("Delay: {}", m_spawnDelay),
        fmt::format("Delay+-: {}", m_delayRange),
        fmt::format("Spawn ordered: {}", m_spawnOrdered ? "Yes" : "No"),
        fmt::format("Preview disabled: {}", m_previewDisable ? "Yes" : "No"),
        fmt::format("Active: {}", m_active ? "Yes" : "No")
    };
}

CustomProperties ModSpawnTrigger::getCustomProperties() {
    SpawnTriggerGameObject::create();
    return {
        CustomObject::toProperty(ModSpawnTrigger::MOD_KEY, m_mod),
        CustomObject::toProperty(ModSpawnTrigger::TARGET_GROUP, m_targetGroupID),
        CustomObject::toProperty(ModSpawnTrigger::DELAY, m_spawnDelay),
        CustomObject::toProperty(ModSpawnTrigger::DELAY_PLUS_MINUS, m_delayRange),
        CustomObject::toProperty(ModSpawnTrigger::SPAWN_ORDERED, m_spawnOrdered),
        CustomObject::toProperty(ModSpawnTrigger::PREVIEW_DISABLE, m_previewDisable)
    };
}

void ModSpawnTrigger::initWithCustomProperties(const CustomProperties& values) {
    CustomObject::propertyInto(m_mod, ModSpawnTrigger::MOD_KEY, values);
    CustomObject::propertyInto(m_targetGroupID, ModSpawnTrigger::TARGET_GROUP, values);
    CustomObject::propertyInto(m_spawnDelay, ModSpawnTrigger::DELAY, values);
    CustomObject::propertyInto(m_delayRange, ModSpawnTrigger::DELAY_PLUS_MINUS, values);
    CustomObject::propertyInto(m_spawnOrdered, ModSpawnTrigger::SPAWN_ORDERED, values);
    CustomObject::propertyInto(m_previewDisable, ModSpawnTrigger::PREVIEW_DISABLE, values);

    this->checkMod();
}

void ModSpawnTrigger::checkMod() {
    m_active = Loader::get()->isModLoaded(m_mod);
}

$on_mod(Loaded) {
    ObjectAPI::registerObject(ObjectInfo::builder()
        .id("mod-spawn-trigger"_spr)
        .sprite("mod-trigger.png"_spr)
        .factory(ModSpawnTrigger::create)
        .editObject(ModSpawnTrigger::getEditObjectConfig)
        .editorTab(EditorTab::TRIGGERS)
        .build());
}