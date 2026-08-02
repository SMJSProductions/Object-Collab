#pragma once

#include <object_collab.hpp>

class $object(ModSpawnTrigger, SpawnTriggerGameObject) {
public:
    static constexpr size_t TARGET_GROUP_ID = 51;
    static constexpr size_t MOD_KEY = 140;

    static ModSpawnTrigger* create(object_collab::ObjectInfo* info);
    static object_collab::PopupOptions getEditObjectConfig(const object_collab::Selected& selected);
private:
    static std::string getModName(std::string modID);

    bool m_active;
public:
    std::string m_mod;

    ModSpawnTrigger(object_collab::ObjectInfo* info);
    void postInit() override;
    void postEditorInit() override;
    void triggerObject(GJBaseGameLayer* layer, const int uniqueID, const gd::vector<int>* remapKeys) override;
    std::vector<std::string> getObjectDetails() override;
    bool ignoreEditorDuration() override;
    void checkMod();
};