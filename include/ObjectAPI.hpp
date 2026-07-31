#pragma once

#include <ObjectInfo.hpp>

namespace object_collab {
    using Register = std::unordered_map<uint32_t, ObjectInfo>;

    class OBJC_API_DLL ObjectAPI {
        static constexpr uint32_t BASE_CUSTOM_OBJECT_ID = 100000000;
    public:
        /// @returns The object registry with all numeric ID & info pairs.
        [[nodiscard]] static const Register& getRegister();
        /// @returns The inclusive base numeric ID constant used for custom object keys.
        [[nodiscard]] static constexpr uint32_t getBaseCustomObjectID() { return ObjectAPI::BASE_CUSTOM_OBJECT_ID; }
        /// Looks up the numeric ID of the given object ID.
        /// @param objectID The string ID including mod ID under which the object is registered.
        /// @returns The found numeric ID. A std::nullopt result means the object isn't registered.
        [[nodiscard]] static std::optional<uint32_t> getCustomObjectNumericID(std::string_view objectID);
        /// Looks up the object ID of the given numeric ID.
        /// @param numericID The numeric ID under which the object is registered.
        /// @returns The found object ID. A std::nullopt result means the object isn't registered.
        [[nodiscard]] static std::optional<std::string_view> getCustomObjectID(int numericID);
        /// Looks up the object info of the given object ID.
        /// @param objectID The string ID including mod ID under which the object is registered.
        /// @returns The found object info. A nullptr result means the object isn't registered.
        [[nodiscard]] static ObjectInfo* getCustomObject(std::string_view objectID);
        /// Looks up the object info of the given numeric ID.
        /// @param numericID The numeric ID under which the object is registered.
        /// @returns The found object info. A nullptr result means the object isn't registered.
        [[nodiscard]] static ObjectInfo* getCustomObject(int numericID);
        /// Registers an object into the registry.
        /// @param info The object info required for the object to function.
        /// @param mod The mod the object is registered under. This is automatically set.
        static void registerObject(ObjectInfo info, geode::Mod* mod = geode::Mod::get());
        /// Unregisters an object from the registry.
        /// @param objectID The string ID including mod ID under which the object is registered.
        /// @returns The result. False if not found.
        static bool unregisterObject(std::string_view objectID);
        /// Unregisters an object from the registry.
        /// @param numericID The numeric ID under which the object is registered.
        /// @returns The result. False if not found.
        static bool unregisterObject(int numericID);

        ObjectAPI() = delete;
    };
}