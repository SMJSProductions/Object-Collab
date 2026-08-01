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

        /// Adds a formatted field to the internal list.
        /// @param fmt The FMT string.
        /// @param args The list of templated args to add to the string.
        template<typename ...T>
        [[nodiscard]] inline DetailsBuilder&& field(fmt::format_string<T...> fmt, T&& ...args) && {
            m_fields.emplace_back(fmt::format(fmt, std::forward<T>(args)...));

            return std::move(*this);
        }

        [[nodiscard]] inline std::vector<std::string> build() && {
            return std::move(m_fields);
        }
    };
}