#pragma once

#include "CustomLevelData.hpp"
#include "CustomObject.hpp"
#include "DetailsBuilder.hpp"
#include "EditorPopupConfig.hpp"
#include "ObjectAPI.hpp"
#include "ObjectIDSwap.hpp"
#include "ObjectInfo.hpp"
#include "Property.hpp"

#define CUSTOM_OBJECT_IMPLEMENT(sourceVar, type, customVar, ...) \
    if (sourceVar && sourceVar->m_objectID >= object_collab::ObjectAPI::getBaseCustomObjectID()) { \
        if (type* customVar = geode::cast::typeinfo_cast<type*>(sourceVar)) __VA_ARGS__; \
    }

#define $object(name, inheritance) name : public object_collab::CustomObject<inheritance>

namespace object_collab {
    namespace prelude {
        using namespace editor_popup;
    }
}