#pragma once

#include "ObjectAPI.hpp"

namespace object_collab {
    /// A map of numeric IDs and the associated object info.
    using AllocationsRegister = std::unordered_map<uint32_t, ObjectInfo*>;
    /// A map of numeric IDs and the string IDs of the objects which aren't able to be linked due to missing info.
    using MissingRegister = std::unordered_map<uint32_t, std::string>;

    /// This class is mostly used for internally solving the compatibility of levels.
    class OBJC_API_DLL CustomLevelData {
        friend class matjson::Serialize<CustomLevelData>;

        struct Impl;

        static CustomLevelData ACTIVE;

        std::unique_ptr<Impl> m_impl;
    public:
        /// Gets the currently active custom level data.
        [[nodiscard]] static const CustomLevelData& get();
        /// Loads the custom level data from a level and stores the state.
        /// @param baseGameLayer The layer to extract the data from.
        static const CustomLevelData& load(cocos2d::CCLayer* baseGameLayer);
        /// Resets the current active custom level data to the default state without overwriting the save of any level.
        static const CustomLevelData& reset();
        /// Gets the raw custom level data object of a level.
        /// @param baseGameLayer The layer to extract the data from.
        [[nodiscard]] static matjson::Value& raw(cocos2d::CCLayer* baseGameLayer);
        /// Clears the custom level data object from a level.
        /// @param editorLayer The layer to write the data to.
        static void clear(LevelEditorLayer* editorLayer);
        /// Saves the current active custom level data object into the level.
        /// @param editorLayer The layer to write the data to.
        /// @param customObjects The custom objects owned by the level.
        static void save(LevelEditorLayer* editorLayer, std::span<CustomObjectInterface*> customObjects);

        CustomLevelData& operator=(CustomLevelData&& other) noexcept;
        CustomLevelData& operator=(const CustomLevelData& other) noexcept = delete;

        CustomLevelData(CustomLevelData&& other) noexcept;
        CustomLevelData(const CustomLevelData& other) noexcept = delete;
        CustomLevelData(bool defaulted = true);
        ~CustomLevelData();
        /// If the object is default and should not be considered representative of the level.
        [[nodiscard]] bool isDefaulted() const;
        /// Returns the list of mod dependencies of the custom level data which are loaded.
        [[nodiscard]] const std::span<std::string> getMods() const;
        /// Returns the list of missing dependencies of the custom level data.
        [[nodiscard]] const std::span<std::string> getMissingMods() const;
        /// Returns the allocation register of the objects.
        [[nodiscard]] const AllocationsRegister& getAllocations() const;
        /// Returns the missing objects which couldn't be linked due to them not being registered.
        [[nodiscard]] const MissingRegister& getMissingObjects() const;
        /// The list of object strings the level runs on.
        [[nodiscard]] const std::span<std::string> getObjects() const;
    };
}