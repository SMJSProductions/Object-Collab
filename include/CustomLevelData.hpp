#pragma once

#include <ObjectAPI.hpp>
#include <alphalaneous.level-storage-api/include/LevelStorageAPI.hpp>

namespace object_collab {
    using AllocationsRegister = std::unordered_map<uint32_t, ObjectInfo*>;
    using MissingRegister = std::unordered_map<uint32_t, std::string>;

    class OBJC_API_DLL CustomLevelData {
        friend class matjson::Serialize<CustomLevelData>;

        struct Impl;

        std::unique_ptr<Impl> m_impl;
    public:
        static CustomLevelData ACTIVE;

        static matjson::Value raw(cocos2d::CCLayer* baseGameLayer);
        static CustomLevelData load(cocos2d::CCLayer* baseGameLayer);
        static void save(LevelEditorLayer* editorLayer, std::span<CustomObjectnterface*> customObjects);

        CustomLevelData& operator=(CustomLevelData&& other);
        CustomLevelData& operator=(const CustomLevelData& other) = delete;

        CustomLevelData(CustomLevelData&& other);
        CustomLevelData(const CustomLevelData& other) = delete;
        CustomLevelData(bool defaulted = true);
        ~CustomLevelData();
        bool isDefaulted() const;
        const std::vector<std::string>& getMods() const;
        const std::vector<std::string>& getMissingMods() const;
        const AllocationsRegister& getAllocations() const;
        const MissingRegister& getMissingObjects() const;
        const std::vector<std::string>& getObjects() const;
    };
}