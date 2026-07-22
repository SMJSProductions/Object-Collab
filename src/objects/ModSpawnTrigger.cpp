#include "ModSpawnTrigger.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

CustomObject* ModSpawnTrigger::create() {
    return new ModSpawnTrigger();
}

PopupOptions ModSpawnTrigger::getEditObjectConfig(const Selected& selected) {
    return PopupConfig::builder()
        .width(320)
        .height(150)
        .title("Loaded Mod Spawn Group")
        .info(InfoPopup::builder()
            .title("Help")
            .description("Works exactly like the spawn trigger with the condition that the targetted mod is installed & loaded.")
            .build())
        .triggerToggles(true)
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
                .onValue([](const uint32_t value, const Selected& selected, Popup* popup) {
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
        .build();
}

ModSpawnTrigger::ModSpawnTrigger(): CustomObject(GameObjectType::Modifier) { }

void ModSpawnTrigger::postInit() {
    m_duration = 0;

    this->setHitbox({ 1, 1 });
}

void ModSpawnTrigger::onAction() {
    GJBaseGameLayer::get()->m_effectManager->spawnGroup(
        m_targetGroupID,
        m_spawnTriggerDelay,
        m_spawnOrdered,
        {},
        m_uniqueID,
        m_controlID
    );
}

std::vector<std::string> ModSpawnTrigger::getObjectDetails() {
    return {
        fmt::format("Mod ID: {}", m_mod.empty() ? "Unknown" : m_mod),
        fmt::format("Group ID: {}", m_targetGroupID),
        fmt::format("Spawn trigger delay: {}", m_spawnTriggerDelay),
        fmt::format("Is spawn ordered: {}", m_spawnOrdered ? "Yes" : "No")
    };
}

CustomProperties ModSpawnTrigger::getCustomProperties() {
    SpawnTriggerGameObject::create();
    return {
        CustomObject::toProperty(ModSpawnTrigger::MOD_KEY, m_mod),
        CustomObject::toProperty(ModSpawnTrigger::TARGET_GROUP, m_targetGroupID),
        CustomObject::toProperty(ModSpawnTrigger::DELAY, m_spawnTriggerDelay),
        // CustomObject::toProperty(ModSpawnTrigger::DELAY_PLUS_MINUS, ), // TODO:
        CustomObject::toProperty(ModSpawnTrigger::SPAWN_ORDERED, m_spawnOrdered),
        CustomObject::toProperty(ModSpawnTrigger::PREVIEW_DISABLE, m_previewDisable)
    };
}

void ModSpawnTrigger::initWithCustomProperties(const CustomProperties& values) {
    CustomObject::propertyInto(m_mod, ModSpawnTrigger::MOD_KEY, values);
    CustomObject::propertyInto(m_targetGroupID, ModSpawnTrigger::TARGET_GROUP, values);
    CustomObject::propertyInto(m_spawnTriggerDelay, ModSpawnTrigger::DELAY, values);
    // CustomObject::propertyInto(, ModSpawnTrigger::DELAY_PLUS_MINUS, values); // TODO:
    CustomObject::propertyInto(m_spawnOrdered, ModSpawnTrigger::SPAWN_ORDERED, values);
    CustomObject::propertyInto(m_previewDisable, ModSpawnTrigger::PREVIEW_DISABLE, values);
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