#pragma once

#include <object_collab.hpp>

class ModSpawnTrigger : public object_collab::CustomObject<SpawnTriggerGameObject> {
    static constexpr uint32_t TARGET_GROUP_ID = 51;
    static constexpr uint32_t MOD_KEY = 140;
public:
    static ModSpawnTrigger* create();
    static object_collab::PopupOptions getEditObjectConfig(const object_collab::Selected& selected);
private:
    static std::string getModName(std::string modID);

    std::string m_mod;
    bool m_active;
public:
    ModSpawnTrigger();
    void postInit() override;
    void postEditorInit() override;
    void triggerObject(GJBaseGameLayer* layer, const int uniqueID, const gd::vector<int>* remapKeys) override;
    std::vector<std::string> getObjectDetails() override;
    bool ignoreEditorDuration() override;
    void checkMod();
};