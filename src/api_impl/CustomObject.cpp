#include <CustomObject.hpp>
#include <CustomLevelData.hpp>

using namespace object_collab;
using namespace geode::prelude;

struct CustomObjectInterface::Impl {
    const CustomProperties& customProperties;
    const ObjectTraits traits;
    const bool trigger;
    std::optional<size_t> triggerTextProperty;
    CCPoint triggerTextPropertyOffset;
    float triggerTextPropertyScale = 0;
};

std::vector<std::string_view> CustomObjectInterface::split(const std::string_view string, const char delimiter) {
    std::vector<std::string_view> results;
    size_t offset = 0;

    results.reserve(std::count(string.begin(), string.end(), delimiter) + 1);

    do {
        const size_t nextOffset = string.find(delimiter, offset);

        results.emplace_back(string.substr(offset, nextOffset - offset));
        offset = nextOffset;
    } while (offset++ != std::string_view::npos);

    return results;
}

Result<ObjectVectors> CustomObjectInterface::createObjectVectorsFromString(std::string_view object) {
    const std::vector<std::string_view> properties = CustomObjectInterface::split(object, ',');
    const CustomLevelData& customLevelData = CustomLevelData::get();
    GJBaseGameLayer* baseGame = GJBaseGameLayer::get();
    // Rob OMFG use a map FFS
    // Yes this needs a size of 601, otherwise Rob will just offset hard to random memory with 0 bound checks... I wish I were kidding
    gd::vector<gd::string> values(601);
    gd::vector<void*> exists(601);

    // Why is there an odd amount of entries?
    if (properties.size() % 2 != 0) {
        return Err("Uneven amount of properties");
    }

    for (size_t i = 0; i < properties.size(); i += 2) {
        GEODE_UNWRAP_INTO(const uint32_t key, utils::numFromString<uint32_t>(properties[i]));

        if (key >= values.size()) {
            values.resize(key + 1);
            exists.resize(key + 1);
        }

        if (key == 1 && !customLevelData.isDefaulted()) {
            GEODE_UNWRAP_INTO(const int objectID, utils::numFromString<int>(properties[i + 1]));
            const AllocationsRegister& allocations = customLevelData.getAllocations();
            const auto& entry = allocations.find(objectID);

            if (entry == allocations.end()) {
                return Err(fmt::format("Unknown object allocation ID {}", objectID));
            } else {
                values[key] = utils::numToString(ObjectAPI::getCustomObjectNumericID(entry->second->getID()).value_or(objectID));
            }
        } else {
            values[key] = std::string(properties[i + 1]);
        }

        exists[key] = baseGame;
    }

    return Ok(std::make_pair(std::move(values), std::move(exists)));
}

CustomObjectInterface& CustomObjectInterface::operator=(CustomObjectInterface&& other) noexcept = default;

CustomObjectInterface::CustomObjectInterface(CustomObjectInterface&& other) noexcept = default;

CustomObjectInterface::CustomObjectInterface(ObjectInfo* info, ObjectTraits&& traits, bool isEffect) {
    static const CustomProperties DEFAULT_PROPERTIES = CustomProperties();
    const bool isTrigger = isEffect && traits.getGameObjectType() == GameObjectType::Modifier;

    m_impl = std::make_unique<Impl>(std::visit(makeVisitor{
        [](const QuickObject& object) -> const CustomProperties& {
            return DEFAULT_PROPERTIES;
        },
        [](const ComplexObject& object) -> const CustomProperties& {
            return object.getCustomProperties();
        }
    }, info->getConstruction()), std::forward<ObjectTraits>(traits), isTrigger);
}

CustomObjectInterface::~CustomObjectInterface() = default;

const CustomProperties& CustomObjectInterface::getCustomProperties() {
    return m_impl->customProperties;
}

const ObjectTraits& CustomObjectInterface::getTraits() {
    return m_impl->traits;
}

bool CustomObjectInterface::isTriggerObject() {
    return m_impl->trigger;
}

std::optional<size_t> CustomObjectInterface::getTriggerTextProperty() {
    return m_impl->triggerTextProperty;
}

void CustomObjectInterface::setTriggerTextProperty(std::optional<size_t> property) {
    m_impl->triggerTextProperty = property;
}

const cocos2d::CCPoint& CustomObjectInterface::getTriggerTextPropertyOffset() {
    return m_impl->triggerTextPropertyOffset;
}

void CustomObjectInterface::setTriggerTextPropertyOffset(cocos2d::CCPoint offset) {
    m_impl->triggerTextPropertyOffset = std::move(offset);
}

float CustomObjectInterface::getTriggerTextPropertyScale() {
    return m_impl->triggerTextPropertyScale;
}

void CustomObjectInterface::setTriggerTextPropertyScale(float scale) {
    m_impl->triggerTextPropertyScale = scale;
}
