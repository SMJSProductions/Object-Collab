#pragma once

#include <Geode/Geode.hpp>
#include "dll.hpp"

namespace object_collab {
    class OBJC_API_DLL ObjectIDSwap final {
        int m_originalID;
        GameObject* m_object;
    public:
        ObjectIDSwap& operator=(ObjectIDSwap&& other) noexcept = default;
        ObjectIDSwap& operator=(const ObjectIDSwap& other) noexcept = delete;

        ObjectIDSwap(ObjectIDSwap&& other) noexcept = default;
        ObjectIDSwap(const ObjectIDSwap& other) noexcept = delete;
        /// Swaps the object ID of the given object. Resets to the original ID once the object dies. This allows you to simulate hardcoded object behavior for specific methods.
        /// @note This object is meant to only stay stored in a scope to make it automatically reset once the scope expires.
        /// @warning Do not use this unless you know what you're doing! RobTop's code makes a lot of blind assumptions about states and can thus easily fail when used incorrectly.
        /// @param object The object to swap the ID on.
        /// @param newID The ID to temporarily set on the object.
        ObjectIDSwap(GameObject* object, int newID);
        /// This will reset the object ID to the original.
        ~ObjectIDSwap();
        /// Resets the object ID to the original.
        void reset();
        /// Sets the object ID to the given ID.
        /// @param newID The ID to set.
        void set(int newID);
    };
}