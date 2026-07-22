#include <CustomObject.hpp>
#include <CustomLevelData.hpp>

using namespace object_collab;
using namespace geode::prelude;

struct CustomObject::Impl {
    bool m_isUpdating = false;
};

std::vector<std::string_view> split(const std::string_view string, const char delimiter) {
    std::vector<std::string_view> results;
    size_t offset = 0;

    do {
        const size_t nextOffset = string.find(delimiter, offset);

        results.emplace_back(string.substr(offset, nextOffset - offset));
        offset = nextOffset;
    } while (offset++ != std::string_view::npos);

    return results;
}

Result<ObjectVectors> CustomObject::createObjectVectorsFromString(std::string_view object) {
    const std::vector<std::string_view> properties = split(object, ',');
    // Rob OMFG use a map FFS
    // Yes this needs a size of 600, otherwise Rob will just offset hard to random memory with 0 bound checks... I wish I were kidding
    gd::vector<gd::string> values(600);
    gd::vector<void*> exists(600);

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

        if (key == 1 && !CustomLevelData::ACTIVE.isDefaulted()) {
            GEODE_UNWRAP_INTO(const int objectID, utils::numFromString<int>(properties[i + 1]));
            const AllocationsRegister& allocations = CustomLevelData::ACTIVE.getAllocations();
            const auto& entry = allocations.find(objectID);

            if (entry == allocations.end()) {
                return Err("Unknown object allocation ID");
            } else {
                values[key] = utils::numToString(ObjectAPI::getCustomObjectNumericID(entry->second->getID()).value_or(objectID));
            }
        } else {
            values[key] = properties[i + 1];
        }

        exists[key] = GJBaseGameLayer::get();
    }

    return Ok(std::make_pair(std::move(values), std::move(exists)));
}

CustomObject& CustomObject::operator=(CustomObject&& other) = default;

CustomObject::CustomObject(CustomObject&& other) = default;

CustomObject::CustomObject(GameObjectType objectType): m_impl(std::make_unique<Impl>()) {
    m_objectType = objectType;
    m_classType = m_objectType == GameObjectType::Modifier ? GameObjectClassType::Effect : GameObjectClassType::Enhanced;
}

CustomObject::~CustomObject() = default;

std::vector<std::string> CustomObject::getObjectDetails() { return {}; }

CustomProperties CustomObject::getCustomProperties() { return {}; }

void CustomObject::initWithCustomProperties(const CustomProperties& properties) { }

void CustomObject::postInit() { }

void CustomObject::internalPostInit() {
    m_dontIgnoreDuration = m_isTrigger = this->isTrigger();

    if (m_isTrigger) {
        m_baseColor->m_defaultColorID = 0;
        m_isInvisible = PlayLayer::get();
    }
}

void CustomObject::onAction() { }

void CustomObject::setHitbox(const CCSize& sizeUnits, const CCPoint& offsetUnits) {
    constexpr float TILE_SIZE = 30;

    m_width = sizeUnits.width * TILE_SIZE;
    m_height = sizeUnits.height * TILE_SIZE;
    m_customBoxOffset = offsetUnits * TILE_SIZE;
}

void CustomObject::setRoundHitbox(const float radiusUnits) {
    this->setHitbox({ radiusUnits * 2, radiusUnits * 2 });

    m_objectRadius = m_width / 2;
}

void CustomObject::setRawHitbox(const CCSize& size, const CCPoint& offset) {
    m_width = size.width;
    m_height = size.height;
    m_customBoxOffset = offset;
}

void CustomObject::setRawRoundHitbox(const float radius) {
    this->setHitbox({ radius * 2, radius * 2 });

    m_objectRadius = radius;
}

void CustomObject::scheduleUpdate() {
    m_impl->m_isUpdating = true;
}

void CustomObject::unscheduleUpdate() {
    m_impl->m_isUpdating = false;
}

void CustomObject::visit() {
    if (m_impl->m_isUpdating) this->update(CCDirector::get()->getDeltaTime());

    EffectGameObject::visit();
}

void CustomObject::activateObject() {
    EffectGameObject::activateObject();

    if (!this->isTrigger()) this->onAction();
}

void CustomObject::triggerObject(GJBaseGameLayer* layer, int uniqueID, const gd::vector<int>* remapKeys) {
    EffectGameObject::triggerObject(layer, uniqueID, remapKeys);

    if (this->isTrigger()) this->onAction();
}

void CustomObject::customObjectSetup(gd::vector<gd::string>& values, gd::vector<void*>& exists) {
    CustomProperties properties;

    EffectGameObject::customObjectSetup(values, exists);

    for (size_t i = 0; i < values.size() && i < exists.size(); i++) {
        if (exists[i]) {
            std::string value = values[i];

            std::ranges::replace(value, 0x1, ',');
            std::ranges::replace(value, 0x2, ';');

            properties.emplace(i, std::move(value));
        }
    }

    this->initWithCustomProperties(std::move(properties));
}

gd::string CustomObject::getSaveString(GJBaseGameLayer* layer) {
    CustomProperties properties = this->getCustomProperties();
    StringBuffer buffer;

    buffer.append(EffectGameObject::getSaveString(layer));

    for (auto& [key, value] : properties) {
        std::ranges::replace(value, ',', 0x1);
        std::ranges::replace(value, ';', 0x2);

        buffer.append(',');
        buffer.append(key);
        buffer.append(',');
        buffer.append(value);
    }

    return buffer.str();
}