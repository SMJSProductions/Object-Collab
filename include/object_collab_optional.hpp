#pragma once

#include <Geode/loader/Dispatch.hpp>
#include "ObjectAPI.hpp"

#define MY_MOD_ID "smjs.object-collab"

namespace object_collab {
    namespace prelude {
        using namespace object_collab;
    }

    /// The simplified object info for optional inclusions
    struct OptionalObjectInfo {
        std::string id;
        std::string sprite;
        bool hasEditSpecial;
        bool hasEditObject;
        bool isComplex;
        std::vector<size_t> customProperties;
    };

    /// The simplified register for optional inclusions
    using OptionalRegister = std::unordered_map<uint32_t, OptionalObjectInfo>;

    /// Gets the simplified register
    /// @warning Numeric IDs are subject to change between levels. It's not recommended to cache the output of this method.
    inline geode::Result<OptionalRegister> getOptionalRegister() GEODE_EVENT_EXPORT(&getOptionalRegister, ());
}