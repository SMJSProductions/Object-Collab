#include <CustomLevelData.hpp>

using namespace alpha;
using namespace object_collab;
using namespace geode::prelude;

constexpr const char* SAVE_KEY = "custom_level_data";

struct CustomLevelData::Impl {
    bool defaulted;
    std::vector<std::string> mods;
    std::vector<std::string> missingMods;
    AllocationsRegister allocations;
    MissingRegister missingObjects;
    std::vector<std::string> objects;
};

template <>
struct matjson::Serialize<CustomLevelData> {
    static Result<CustomLevelData> fromJson(const Value& value) {
        const Value& objects = value["objects"];
        const Value& allocations = value["allocations"];
        Loader* loader = Loader::get();
        CustomLevelData levelData(false);

        if (!objects.isArray()) {
            return Err("Not an array");
        } else if (!allocations.isObject()) {
            return Err("Not an object");
        }

        for (const Value& object : objects) {
            levelData.m_impl->objects.emplace_back(GEODE_UNWRAP(object.asString()));
        }

        for (const auto& [rawIDString, idValue] : allocations) {
            GEODE_UNWRAP_INTO(std::string objectID, idValue.asString());
            GEODE_UNWRAP_INTO(const uint32_t rawID, utils::numFromString<uint32_t>(rawIDString));
            const size_t slashPos = objectID.find('/');
            const std::string_view modID = std::string_view(objectID).substr(0, slashPos);

            if (slashPos == std::string::npos) {
                return Err("Invalid object ID in the allocation info");
            } else if (!loader->isModLoaded(modID)) {
                levelData.m_impl->missingObjects.emplace(rawID, std::move(objectID));

                if (std::find(levelData.m_impl->missingMods.begin(), levelData.m_impl->missingMods.end(), modID) == levelData.m_impl->missingMods.end()) {
                    levelData.m_impl->missingMods.emplace_back(modID);
                }
            } else if (std::find(levelData.m_impl->mods.begin(), levelData.m_impl->mods.end(), modID) == levelData.m_impl->mods.end()) {
                levelData.m_impl->mods.emplace_back(modID);
            }

            if (ObjectInfo* info = ObjectAPI::getCustomObject(objectID)) {
                levelData.m_impl->allocations.emplace(rawID, info);
            } else {
                levelData.m_impl->missingObjects.emplace(rawID, std::move(objectID));
            }
        }

        return Ok(std::move(levelData));
    }

    static Value toJson(const CustomLevelData& customLevelData) {
        Value saveData = Value::object();

        saveData.set("objects", Value::array());
        saveData.set("allocations", Value::object());

        Value& objects = saveData["objects"];
        Value& allocations = saveData["allocations"];

        for (const auto& object : customLevelData.m_impl->objects) {
            objects.push(object);
        }

        for (const auto& [rawID, info] : customLevelData.m_impl->allocations) {
            allocations.set(utils::numToString(rawID), info->getID());
        }

        return saveData;
    }
};

CustomLevelData CustomLevelData::ACTIVE;

matjson::Value CustomLevelData::raw(CCLayer* baseGameLayer) {
    if (!baseGameLayer) return {};

    return level_storage::getSavedValue<matjson::Value>(baseGameLayer, SAVE_KEY);
}

CustomLevelData CustomLevelData::load(CCLayer* baseGameLayer) {
    if (!baseGameLayer) return true;

    return level_storage::getSavedValue<CustomLevelData>(baseGameLayer, SAVE_KEY);
}

void CustomLevelData::save(LevelEditorLayer* editorLayer, std::span<CustomObjectnterface*> customObjects) {
    if (!editorLayer) return;

    CCArray* objects = LevelEditorLayer::get()->getAllObjects();
    StringMap<uint32_t> tempLookup;
    CustomLevelData levelData(false);
    size_t offset = ObjectAPI::getBaseCustomObjectID();

    for (CustomObjectnterface* object : customObjects) {
        GameObject* gameObject = object->getGameObject();
        const int originalObjectID = gameObject->m_objectID;
        ObjectInfo* objectInfo = ObjectAPI::getCustomObject(gameObject->m_objectID);
        const std::string_view id = objectInfo->getID();

        // Yes I am temp overwriting the existing object ID to save it... Sure do hope no one assumes this thing is static.
        if (const auto assignedID = tempLookup.find(id); assignedID == tempLookup.end()) {
            gameObject->m_objectID = offset;
            tempLookup.emplace(id, offset);
            levelData.m_impl->allocations.emplace(offset, objectInfo);

            offset++;
        } else {
            gameObject->m_objectID = assignedID->second;
        }

        // No need to populate the mods field, it doesn't get used for saving
        levelData.m_impl->objects.emplace_back(gameObject->getSaveString(editorLayer));

        gameObject->m_objectID = originalObjectID;
    }

    level_storage::setSavedValue(editorLayer, SAVE_KEY, levelData);
}

CustomLevelData& CustomLevelData::operator=(CustomLevelData&& other) = default;

CustomLevelData::CustomLevelData(CustomLevelData&& other) = default;

CustomLevelData::CustomLevelData(bool defaulted): m_impl(std::make_unique<CustomLevelData::Impl>()) {
    m_impl->defaulted = defaulted;
}

CustomLevelData::~CustomLevelData() { }

bool CustomLevelData::isDefaulted() const {
    return m_impl->defaulted;
}

const std::vector<std::string>& CustomLevelData::getMods() const {
    return m_impl->mods;
}

const std::vector<std::string>& CustomLevelData::getMissingMods() const {
    return m_impl->missingMods;
}

const AllocationsRegister& CustomLevelData::getAllocations() const {
    return m_impl->allocations;
}

const MissingRegister& CustomLevelData::getMissingObjects() const {
    return m_impl->missingObjects;
}

const std::vector<std::string>& CustomLevelData::getObjects() const {
    return m_impl->objects;
}