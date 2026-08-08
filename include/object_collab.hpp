#pragma once

#include "CustomLevelData.hpp"
#include "CustomObject.hpp"
#include "DetailsBuilder.hpp"
#include "EditorPopupConfig.hpp"
#include "ObjectAPI.hpp"
#include "ObjectIDSwap.hpp"
#include "ObjectInfo.hpp"
#include "Property.hpp"

#define CUSTOM_OBJECT_IMPLEMENT(source, custom, ...) \
    if (source && source->m_objectID >= object_collab::ObjectAPI::getBaseCustomObjectID()) { \
        if (object_collab::CustomObjectInterface* custom = geode::cast::typeinfo_cast<object_collab::CustomObjectInterface*>(source)) __VA_ARGS__; \
    }

#define $object(name, inheritance) name : public object_collab::CustomObject<inheritance>

namespace object_collab {
    namespace prelude {
        using namespace editor_popup;
    }
}