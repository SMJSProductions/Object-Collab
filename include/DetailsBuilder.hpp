#pragma once

#include <Geode/Geode.hpp>
#include "dll.hpp"

namespace object_collab {
    class OBJC_API_DLL DetailsBuilder {
    public:
        [[nodiscard]] inline static DetailsBuilder builder() {
            return DetailsBuilder();
        }
    private:
        std::vector<std::string> m_fields;

        DetailsBuilder() = default;
    public:
        /// Reserves a size in the internal list.
        /// @param capacity The capacity.
        [[nodiscard]] inline DetailsBuilder&& reserve(size_t capacity) && {
            m_fields.reserve(capacity);

            return std::move(*this);
        }

        /// Adds a field to the internal list in a standardized format.
        /// @param name The property name.
        /// @param value The property value.
        template<typename T>
        [[nodiscard]] inline DetailsBuilder&& field(std::string_view name, T value) && {
            geode::utils::StringBuffer buffer;

            buffer.append(name);
            buffer.append(": ");

            if constexpr (std::is_same_v<T, bool>) {
                buffer.append(value ? "Yes" : "No");
            } else if constexpr (std::is_floating_point_v<T>) {
                buffer.append(geode::utils::numToString(value, 2));
            } else {
                buffer.append(value);
            }
            
            m_fields.emplace_back(buffer.str());

            return std::move(*this);
        }

        /// Adds a raw formatted field to the internal list.
        /// @param fmt The FMT string.
        /// @param args The list of templated args to add to the string.
        template<typename ...T>
        [[nodiscard]] inline DetailsBuilder&& rawField(fmt::format_string<T...> fmt, T&& ...args) && {
            m_fields.emplace_back(fmt::format(fmt, std::forward<T>(args)...));

            return std::move(*this);
        }

        [[nodiscard]] inline std::vector<std::string> build() && {
            return std::move(m_fields);
        }
    };
}