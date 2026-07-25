#include <CustomObject.hpp>
#include <CustomLevelData.hpp>

using namespace object_collab;
using namespace geode::prelude;

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

struct CustomObjectInterface::Impl {
    // Reserved for future updates
};

Result<ObjectVectors> CustomObjectInterface::createObjectVectorsFromString(std::string_view object) {
    const std::vector<std::string_view> properties = split(object, ',');
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

        if (key == 1 && !CustomLevelData::ACTIVE.isDefaulted()) {
            GEODE_UNWRAP_INTO(const int objectID, utils::numFromString<int>(properties[i + 1]));
            const AllocationsRegister& allocations = CustomLevelData::ACTIVE.getAllocations();
            const auto& entry = allocations.find(objectID);

            if (entry == allocations.end()) {
                return Err(fmt::format("Unknown object allocation ID {}", objectID));
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

CustomObjectInterface::CustomObjectInterface(): m_impl(std::make_unique<Impl>()) { }

CustomObjectInterface::~CustomObjectInterface() = default;