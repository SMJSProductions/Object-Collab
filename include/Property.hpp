#pragma once

#include <Geode/Geode.hpp>
#include "dll.hpp"

namespace object_collab {
    class OBJC_API_DLL PropertyInterface {
    public:
        template<typename T>
        static inline std::string stringifyValue(const T& value) {
            if constexpr (std::is_convertible_v<T, std::string>) {
                return value;
            } else if constexpr (std::is_same_v<T, bool>) {
                return value ? "1" : "0";
            } else if constexpr (std::is_arithmetic_v<T>) {
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

        virtual bool isDefault() const = 0;
        virtual std::string getStringValue() const = 0;
        virtual void applyDefault() = 0;
        virtual void applyFromString(std::string_view value) = 0;
    };

    template<typename T>
    class Property : public PropertyInterface {
        T& m_member;
        T m_defaultValue;
    public:
        Property<T>& operator=(Property<T>&& other) noexcept = default;
        Property<T>& operator=(const Property<T>& other) noexcept = delete;

        Property<T>(Property<T>&& other) noexcept = default;
        Property<T>(const Property<T>& other) noexcept = delete;
        template<typename D> requires std::is_convertible_v<D, T>
        Property(T& member, D&& defaultValue): m_member(member), m_defaultValue(std::forward<D>(defaultValue)) { }

        [[nodiscard]] inline bool isDefault() const override {
            return m_member == m_defaultValue;
        }

        [[nodiscard]] inline const T& getValue() const {
            return m_member;
        }

        [[nodiscard]] inline std::string getStringValue() const override {
            return PropertyInterface::stringifyValue(m_member);
        }

        [[nodiscard]] inline const T& getDefaultValue() const {
            return m_defaultValue;
        }

        inline void applyDefault() override {
            m_member = m_defaultValue;
        }

        inline void applyFromString(std::string_view value) override {
            if constexpr (std::is_convertible_v<T, std::string>) {
                m_member = value;
            } else if constexpr (std::is_same_v<T, bool>) {
                // This is required. matjson converts 0 to true since it looks for the constant and not the accurate conversion.
                m_member = value != "0" && value != "" && value != "false";
            } else if constexpr (std::is_arithmetic_v<T>) {
                if (geode::Result<T> result = geode::utils::numFromString<T>(value)) {
                    m_member = std::move(result).unwrap();
                } else {
                    this->applyDefault();
                }
            } else {
                if (geode::Result<T> result = matjson::parseAs<T>(value)) {
                    m_member = std::move(result).unwrap();
                } else {
                    this->applyDefault();
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