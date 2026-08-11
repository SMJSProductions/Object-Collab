#define GEODE_DEFINE_EVENT_EXPORTS
#include <object_collab_optional.hpp>

using namespace object_collab;
using namespace geode::prelude;

namespace object_collab {
    Result<OptionalRegister> getOptionalRegister() {
        OptionalRegister objectRegister;

        for (auto& [id, info] : ObjectAPI::getRegister()) {
            const ObjectConstruction& construction = info.getConstruction();
            const bool isComplex = std::holds_alternative<ComplexObject>(construction);
            std::vector<size_t> properties;

            if (isComplex) {
                for (auto& [property, _] : std::get<ComplexObject>(construction).getCustomProperties()) {
                    properties.emplace_back(property);
                }
            }

            objectRegister.emplace(id, OptionalObjectInfo{
                .id = std::string(info.getID()),
                .sprite = info.getSprite(),
                .hasEditSpecial = info.hasEditSpecial(),
                .hasEditObject = info.hasEditObject(),
                .isComplex = isComplex,
                .customProperties = std::move(properties)
            });
        }

        return Ok(objectRegister);
    }
}