#include <ObjectAPI.hpp>
#include <Geode/utils/terminate.hpp>

using namespace object_collab;
using namespace geode::prelude;

static uint32_t ID_OFFSET = 0;
static Register REGISTER;
static StringMap<uint32_t> LOOKUP_TABLE;

const Register& ObjectAPI::getRegister() {
    return REGISTER;
}

std::optional<uint32_t> ObjectAPI::getCustomObjectNumericID(std::string_view objectID) {
    if (const auto& entry = LOOKUP_TABLE.find(objectID); entry != LOOKUP_TABLE.end()) {
        return entry->second;
    } else {
        return std::nullopt;
    }
}

std::optional<std::string_view> ObjectAPI::getCustomObjectID(int numericID) {
    if (const auto& entry = REGISTER.find(numericID); entry != REGISTER.end()) {
        return entry->second.getID();
    } else {
        return std::nullopt;
    }
}

ObjectInfo* ObjectAPI::getCustomObject(std::string_view objectID) {
    if (const std::optional<uint32_t> numericID = ObjectAPI::getCustomObjectNumericID(objectID)) {
        return &REGISTER.find(numericID.value())->second;
    } else {
        return nullptr;
    }
}

ObjectInfo* ObjectAPI::getCustomObject(int numericID) {
    if (const auto entry = REGISTER.find(numericID); entry != REGISTER.end()) {
        return &entry->second;
    } else {
        return nullptr;
    }
}

void ObjectAPI::registerObject(ObjectInfo info, Mod* mod) {
    const std::string_view id = info.getID();

    if (!id.starts_with(mod->getID())) {
        utils::terminate("Object ID must use the \"\"_spr operator", mod);
    }

    const uint32_t numericID = ObjectAPI::BASE_CUSTOM_OBJECT_ID + ID_OFFSET++;

    LOOKUP_TABLE.emplace(id, numericID);
    CCTextureCache::get()->addImage(info.getSprite().c_str(), false);
    ObjectToolbox::sharedState()->m_allKeys.emplace(numericID, info.getSprite().c_str());
    REGISTER.emplace(numericID, std::move(info));
}

bool ObjectAPI::unregisterObject(std::string_view objectID) {
    if (const std::optional<uint32_t> numericID = ObjectAPI::getCustomObjectNumericID(objectID)) {
        ObjectToolbox* objToolbox = ObjectToolbox::sharedState();

        objToolbox->m_allKeys.erase(objToolbox->m_allKeys.find(numericID.value()));

        return LOOKUP_TABLE.erase(objectID) && REGISTER.erase(numericID.value());
    } else {
        return false;
    }
}

bool ObjectAPI::unregisterObject(int numericID) {
    if (REGISTER.contains(numericID)) {
        return LOOKUP_TABLE.erase(REGISTER.at(numericID).getID()) && REGISTER.erase(numericID);
    } else {
        return false;
    }
}