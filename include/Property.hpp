#pragma once

#include <dll.hpp>
#include <Geode/Geode.hpp>

namespace object_collab {
    class OBJC_API_DLL PropertyInterface {
    public:
        PropertyInterface& operator=(PropertyInterface&& other) noexcept = default;
        PropertyInterface& operator=(const PropertyInterface& other) noexcept = delete;
        
        PropertyInterface(PropertyInterface&& other) noexcept = default;
        PropertyInterface(const PropertyInterface& other) noexcept = delete;
        PropertyInterface() = default;
        virtual ~PropertyInterface() = default;

        virtual std::string getStringValue() const = 0;
        virtual void applyDefault() = 0;
        virtual void applyFromString(std::string_view value) = 0;
    };

    template<typename V>
    class Property : public PropertyInterface {
        V& member;
        V defaultValue;
    public:
        Property<V>& operator=(Property<V>&& other) noexcept = default;
        Property<V>& operator=(const Property<V>& other) noexcept = delete;
        
        Property<V>(Property<V>&& other) noexcept = default;
        Property<V>(const Property<V>& other) noexcept = delete;
        template<typename D> requires std::is_convertible_v<D, V>
        Property(V& member, D&& defaultValue): member(member), defaultValue(std::forward<D>(defaultValue)) { }

        inline std::string getStringValue() const override {
            return matjson::Value(member).dump();
        }

        inline void applyDefault() override {
            member = defaultValue;
        }

        inline void applyFromString(std::string_view value) override {
            if (geode::Result<V> result = matjson::parseAs<V>(value)) {
                member = std::move(result).unwrap();
            } else {
                this->applyDefault();
            }
        }
    };

    using CustomProperties = std::unordered_map<int, std::unique_ptr<PropertyInterface>>;

    class CustomPropertiesList {
        CustomProperties m_map;
    public:
        CustomPropertiesList& operator=(CustomPropertiesList&& other) noexcept = default;
        CustomPropertiesList& operator=(const CustomPropertiesList& other) noexcept = delete;
        
        CustomPropertiesList(CustomPropertiesList&& other) noexcept = default;
        CustomPropertiesList(const CustomPropertiesList& other) noexcept = delete;
        template<class... Pairs>
        CustomPropertiesList(Pairs&&... pairs) {
            (m_map.emplace(std::forward<Pairs>(pairs)), ...);
        }

        inline CustomProperties&& releaseMap() && {
            return std::move(m_map);
        }
    };
}