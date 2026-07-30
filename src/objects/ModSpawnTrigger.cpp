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
            .description("Works exactly like the spawn trigger with the condition that the targeted mod is installed & loaded.")
            .build())
        .triggerToggles(true)
        .menu(EnumMenu::builder()
            .id("mod-id"_spr)
            .title("Mod ID")
            .values(std::move(mods))
            .onValue([](const std::string& value, const Selected& selected, Popup* popup) {
                applyValueToSelectedPropertyAndReport(selected, ModSpawnTrigger::MOD_KEY, value, &ModSpawnTrigger::checkMod);
            })
            .currentValue([](const Selected& selected, Popup* popup) {
                return getCommonPropertyValueOrDefault<std::string>(selected, ModSpawnTrigger::MOD_KEY);
            })
            .build())
        .menu(AxisLayoutMenu::builder()
            .axis(Axis::Row)
            .gap(20)
            .crossAxisAlignment(CrossAxisAlignment::Center)
            .menu(NumericMenu::builder()
                .id("group-id"_spr)
                .title("Group ID")
                .inputType(NumericMenu::InputType::Arrows)
                .min(0)
                .max(9999)
                .precision(0)
                .onValue([](const int value, const Selected& selected, Popup* popup) {
                    applyValueToSelectedProperty(selected, ModSpawnTrigger::TARGET_GROUP_ID, value);
                })
                .currentValue([](const Selected& selected, Popup* popup) {
                    return getCommonPropertyValueOrDefault<int>(selected, ModSpawnTrigger::TARGET_GROUP_ID);
                })
                .build())
            .menu(AxisLayoutMenu::builder()
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
                .inputType(NumericMenu::InputType::Slider)
                .min(0)
                .max(1)
                .precision(4)
                .stepSize(0.0001)
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
                .inputType(NumericMenu::InputType::Slider)
                .min(0)
                .max(1)
                .precision(4)
                .stepSize(0.0001)
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

ModSpawnTrigger::ModSpawnTrigger(): CustomObject({
    CustomObject::propertyFrom(ModSpawnTrigger::TARGET_GROUP_ID, m_targetGroupID, 0),
    CustomObject::propertyFrom(ModSpawnTrigger::MOD_KEY, m_mod, "Unknown"),
}, GameObjectType::Modifier) { }

void ModSpawnTrigger::postInit() {
    this->setHitbox({ 1, 1 });
    this->checkMod();
}

void ModSpawnTrigger::postEditorInit() {
    this->setTriggerTextProperty(ModSpawnTrigger::TARGET_GROUP_ID, { 0, -3 });
}

void ModSpawnTrigger::triggerObject(GJBaseGameLayer* layer, const int uniqueID, const gd::vector<int>* remapKeys) {
    if (m_active) {
        CustomObject::triggerObject(layer, m_uniqueID, remapKeys);
    }
}

std::vector<std::string> ModSpawnTrigger::getObjectDetails() {
    return DetailsBuilder::builder()
        .field("Mod ID: {}", m_mod)
        .field("Group ID: {}", m_targetGroupID)
        .field("Delay: {}", m_spawnDelay)
        .field("Delay+-: {}", m_delayRange)
        .field("Spawn ordered: {}", m_spawnOrdered ? "Yes" : "No")
        .field("Preview disabled: {}", m_previewDisable ? "Yes" : "No")
        .field("Active: {}", m_active ? "Yes" : "No")
        .build();
}

bool ModSpawnTrigger::ignoreEditorDuration() {
    return true;
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
        .editorTab(EditorTab::Triggers)
        .build());
}