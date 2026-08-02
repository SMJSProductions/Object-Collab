#include "ModSpawnTrigger.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

ModSpawnTrigger* ModSpawnTrigger::create(ObjectInfo* info) {
    return new ModSpawnTrigger(info);
}

PopupOptions ModSpawnTrigger::getEditObjectConfig(const Selected& selected) {
    EnumMenu::EnumAliasList mods;

    for (Mod* mod : Loader::get()->getAllMods()) {
        mods.emplace_back(mod->getName(), mod->getID());
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
                applyValueToSelectedAndReport(selected, &ModSpawnTrigger::m_mod, value, &ModSpawnTrigger::checkMod);
            })
            .currentValue([](const Selected& selected, Popup* popup) {
                std::string modID = getCommonValueOrDefault(selected, &ModSpawnTrigger::m_mod);

                return ModSpawnTrigger::getModName(std::move(modID));
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
                    applyValueToSelected(selected, &ModSpawnTrigger::m_targetGroupID, value);
                })
                .currentValue([](const Selected& selected, Popup* popup) {
                    return getCommonValueOrDefault(selected, &ModSpawnTrigger::m_targetGroupID);
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
                        return getCommonValueOrDefault(selected, &ModSpawnTrigger::m_previewDisable);
                    })
                    .build())
                .menu(ToggleMenu::builder()
                    .id("spawn-ordered"_spr)
                    .title("Spawn\nOrdered")
                    .onValue([](const bool value, const Selected& selected, Popup* popup) {
                        applyValueToSelected(selected, &ModSpawnTrigger::m_spawnOrdered, value);
                    })
                    .currentValue([](const Selected& selected, Popup* popup) {
                        return getCommonValueOrDefault(selected, &ModSpawnTrigger::m_spawnOrdered);
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
                    return getCommonValueOrDefault(selected, &ModSpawnTrigger::m_spawnDelay);
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
                    return getCommonValueOrDefault(selected, &ModSpawnTrigger::m_delayRange);
                })
                .build())
            .build())
        .build();
}

std::string ModSpawnTrigger::getModName(std::string modID) {
    if (Mod* mod = Loader::get()->getInstalledMod(modID)) {
        return mod->getName();
    } else {
        return modID;
    }
}

ModSpawnTrigger::ModSpawnTrigger(ObjectInfo* info): CustomObject(info, GameObjectType::Modifier) { }

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
        .field("Mod ID", ModSpawnTrigger::getModName(m_mod))
        .field("Group ID", m_targetGroupID)
        .field("Delay", m_spawnDelay)
        .field("Delay+-", m_delayRange)
        .field("Spawn ordered", m_spawnOrdered)
        .field("Preview disabled", m_previewDisable)
        .field("Active", m_active)
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
        .construction(ComplexObject::builder()
            .factory(ModSpawnTrigger::create)
            .customProperties({
                PropertyInterface::from(ModSpawnTrigger::TARGET_GROUP_ID, &ModSpawnTrigger::m_targetGroupID, 0),
                PropertyInterface::from(ModSpawnTrigger::MOD_KEY, &ModSpawnTrigger::m_mod, "Unknown"),
            })
            .build())
        .editObject(ModSpawnTrigger::getEditObjectConfig)
        .editorTab(EditorTab::Triggers)
        .build());
}