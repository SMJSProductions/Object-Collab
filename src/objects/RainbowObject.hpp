#pragma once

#include <object_collab.hpp>

class RainbowObject : public object_collab::CustomObject<GameObject> {
    static constexpr uint32_t SPEED_KEY = 200;
    static constexpr uint32_t ENABLED_KEY = 201;
public:
    static RainbowObject* create();
    static std::variant<geode::Popup*,object_collab:: editor_popup::PopupConfig> getEditSpecialConfig(const object_collab::Selected& selected);

    RainbowObject();
    std::vector<std::string> getObjectDetails() override;
    object_collab::CustomProperties getCustomProperties() override;
    void initWithCustomProperties(const object_collab::CustomProperties& values) override;
    void postInit() override;
    void onAction(GJBaseGameLayer* layer, const int uniqueID, const gd::vector<int>* remapKeys) override;
    void update(const float dt) override;
private:
    float m_targetSpeed;
    bool m_enabled;
};