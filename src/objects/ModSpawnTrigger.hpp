#pragma once

#include <object_collab.hpp>

class ModSpawnTrigger : public object_collab::CustomObject {
    static constexpr uint32_t MOD_KEY = 140;
    static constexpr uint32_t TARGET_GROUP = 51;
    static constexpr uint32_t DELAY = 63;
    static constexpr uint32_t DELAY_PLUS_MINUS = 556;
    static constexpr uint32_t SPAWN_ORDERED = 102;
    static constexpr uint32_t PREVIEW_DISABLE = 102;
public:
    static CustomObject* create();
    static object_collab::PopupOptions getEditObjectConfig(const object_collab::Selected& selected);
private:
    std::string m_mod;
public:
    ModSpawnTrigger();
    void postInit() override;
    void onAction() override;
    std::vector<std::string> getObjectDetails() override;
    object_collab::CustomProperties getCustomProperties() override;
    void initWithCustomProperties(const object_collab::CustomProperties& values) override;
};