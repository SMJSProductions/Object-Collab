#pragma once

#include <Geode/Geode.hpp>
#include "dll.hpp"

namespace object_collab {
    class CustomObjectInterface;

    template<typename T, typename V> requires std::derived_from<T, GameObject>
    class Property;

    class OBJC_API_DLL PropertyInterface {
    public:
        template<typename T, typename V, typename D> requires std::is_convertible_v<D, V> && std::derived_from<T, GameObject>
        [[nodiscard]] static std::pair<size_t, std::unique_ptr<PropertyInterface>> from(size_t key, V T::* member, D&& defaultValue) {
            return { key, std::make_unique<Property<T, V>>(key, member, std::forward<D>(defaultValue)) };
        }

        /// Stringifies the value to a minimal format using either the GD format or matjson.
        /// @param value The value to stringify.
        template<typename V>
        static inline std::string stringifyValue(const V& value) {
            if constexpr (std::is_convertible_v<V, std::string>) {
                return value;
            } else if constexpr (std::is_same_v<V, bool>) {
                return value ? "1" : "0";
            } else if constexpr (std::is_arithmetic_v<V>) {
                return geode::utils::numToString(value);
            } else {
                return matjson::Value(value).dump();
            }
        }

        PropertyInterface& operator=(PropertyInterface&& other) noexcept = default;
        PropertyInterface& operator=(const PropertyInterface& other) noexcept = delete;

        PropertyInterface(PropertyInterface&& other) noexcept = default;
        PropertyInterface(const PropertyInterface& other) noexcept = delete;
        PropertyInterface() = default;
        virtual ~PropertyInterface() = default;

        virtual bool isDefault(CustomObjectInterface* object) const = 0;
        virtual std::string getStringValue(CustomObjectInterface* object) const = 0;
        virtual void applyDefault(CustomObjectInterface* object) = 0;
        virtual void applyFromString(CustomObjectInterface* object, std::string_view value) = 0;
    };

    template<typename T, typename V> requires std::derived_from<T, GameObject>
    class Property : public PropertyInterface {
        size_t m_key;
        V T::* m_member;
        V m_defaultValue;
    public:
        Property& operator=(Property&& other) noexcept = default;
        Property& operator=(const Property& other) noexcept = delete;

        Property(Property&& other) noexcept = default;
        Property(const Property& other) noexcept = delete;
        template<typename D> requires std::is_convertible_v<D, V>
        Property(size_t key, V T::* member, D&& defaultValue): m_key(key), m_member(member), m_defaultValue(std::forward<D>(defaultValue)) { }

        [[nodiscard]] inline size_t getKey() const {
            return m_key;
        }

        /// Gets the target member
        [[nodiscard]] inline V T::* getMember() const {
            return m_member;
        }

        /// Gets the currently assigned value.
        [[nodiscard]] inline V& getValue(CustomObjectInterface* object) {
            return geode::cast::typeinfo_cast<T*>(object)->*m_member;
        }

        /// Gets the currently assigned value.
        [[nodiscard]] inline const V& getValue(CustomObjectInterface* object) const {
            return geode::cast::typeinfo_cast<T*>(object)->*m_member;
        }

        /// If the value is defaulted.
        [[nodiscard]] inline bool isDefault(CustomObjectInterface* object) const override {
            return this->getValue(object) == m_defaultValue;
        }

        /// Gets the stringified version of the value.
        [[nodiscard]] inline std::string getStringValue(CustomObjectInterface* object) const override {
            return PropertyInterface::stringifyValue(this->getValue(object));
        }

        /// Gets the default value.
        [[nodiscard]] inline const V& getDefaultValue() const {
            return m_defaultValue;
        }

        /// Sets the property value to default.
        inline void applyDefault(CustomObjectInterface* object) override {
            this->getValue(object) = m_defaultValue;
        }

        /// Sets the property value.
        inline void apply(CustomObjectInterface* object, V value) {
            this->getValue(object) = std::move(value);
        }

        /// Converts the value to a the parsed value using either the GD format or matjson and assigns it to the property.
        /// @param value The value to assign.
        inline void applyFromString(CustomObjectInterface* object, std::string_view value) override {
            if constexpr (std::is_convertible_v<V, std::string>) {
                this->getValue(object) = value;
            } else if constexpr (std::is_same_v<V, bool>) {
                // This is required. matjson converts 0 to true since it looks for the constant and not the accurate conversion.
                this->getValue(object) = value != "0" && value != "" && value != "false";
            } else if constexpr (std::is_arithmetic_v<V>) {
                if (geode::Result<V> result = geode::utils::numFromString<V>(value)) {
                    this->getValue(object) = std::move(result).unwrap();
                } else {
                    this->applyDefault(object);
                }
            } else {
                if (geode::Result<V> result = matjson::parseAs<V>(value)) {
                    this->getValue(object) = std::move(result).unwrap();
                } else {
                    this->applyDefault(object);
                }
            }
        }
    };

    using CustomProperties = std::unordered_map<size_t, std::unique_ptr<PropertyInterface>>;

    class CustomPropertiesList {
        CustomProperties m_map;
    public:
        CustomPropertiesList& operator=(CustomPropertiesList&& other) noexcept = default;
        CustomPropertiesList& operator=(const CustomPropertiesList& other) noexcept = delete;

        CustomPropertiesList(CustomPropertiesList&& other) noexcept = default;
        CustomPropertiesList(const CustomPropertiesList& other) noexcept = delete;
        template<class ...Pairs>
        CustomPropertiesList(Pairs&& ...pairs) {
            (m_map.emplace(std::forward<Pairs>(pairs)), ...);
        }

        [[nodiscard]] inline CustomProperties&& releaseMap() && {
            return std::move(m_map);
        }
    };
}