#pragma once

#include <CustomObject.hpp>

namespace object_collab {
    using Selected = std::vector<CustomObjectInterface*>;
}

namespace object_collab::editor_popup {
    using namespace object_collab;

    /// @param value The value to digest (ownership is moved to the callback).
    /// @param selected The currently selected objects.
    /// @param popup The editor popup instance.
    template<typename T>
    using ValueUpdateCallback = geode::Function<void(T value, const Selected& selected, geode::Popup* popup)>;

    /// @param selected The currently selected objects.
    /// @param popup The editor popup instance.
    /// @returns The current agreed upon value.
    template<typename T>
    using CurrentValueCallback = geode::Function<T(const Selected& selected, geode::Popup* popup)>;

    /// @param popup The editor popup instance.
    /// @returns The custom menu to add.
    using CustomMenuFactory = geode::Function<cocos2d::CCMenu*(const Selected& selected, geode::Popup* popup)>;

    class OBJC_API_DLL ValueMenu {
    public:
        virtual ~ValueMenu() = default;
    };

    class OBJC_API_DLL InfoPopup {
        struct Impl;

        std::unique_ptr<Impl> m_impl;

        InfoPopup(std::unique_ptr<Impl> impl);
    public:
        class OBJC_API_DLL Builder {
            friend class InfoPopup;

            std::unique_ptr<Impl> m_impl;

            Builder();
        public:
            ~Builder();
            /// @param id The ID assigned to the menu.
            Builder&& id(std::string id) &&;
            /// @param title The title of the popup section.
            Builder&& title(std::string title) &&;
            /// @param description The description of the info popup which supports color tags (e.g. `<cy></c>`).
            Builder&& description(std::string description) &&;
            InfoPopup build() &&;
        };

        static Builder builder();

        InfoPopup& operator=(InfoPopup&& other) noexcept;
        InfoPopup& operator=(const InfoPopup& other) noexcept = delete;

        InfoPopup(InfoPopup&& other) noexcept;
        InfoPopup(const InfoPopup& other) noexcept = delete;
        ~InfoPopup();
        geode::ZStringView getID() const;
        geode::ZStringView getTitle() const;
        geode::ZStringView getDescription() const;
    };

    class OBJC_API_DLL ToggleMenu : public ValueMenu {
        struct Impl;

        std::unique_ptr<Impl> m_impl;

        ToggleMenu(std::unique_ptr<Impl> impl);
    public:
        class OBJC_API_DLL Builder {
            friend class ToggleMenu;

            std::unique_ptr<Impl> m_impl;

            Builder();
        public:
            ~Builder();
            /// @param id The ID assigned to the menu.
            Builder&& id(std::string id) &&;
            /// @param title The title of the popup section.
            Builder&& title(std::string title) &&;
            /// @param onValue A callback to digest any value changes.
            Builder&& onValue(ValueUpdateCallback<bool> onValue) &&;
            /// @param currentValue The current value getter callback.
            Builder&& currentValue(CurrentValueCallback<bool> currentValue) &&;
            std::unique_ptr<ToggleMenu> build() &&;
        };

        static Builder builder();

        ToggleMenu& operator=(ToggleMenu&& other) noexcept;
        ToggleMenu& operator=(const ToggleMenu& other) noexcept = delete;

        ToggleMenu(ToggleMenu&& other) noexcept;
        ToggleMenu(const ToggleMenu& other) noexcept = delete;
        ~ToggleMenu();
        geode::ZStringView getID() const;
        geode::ZStringView getTitle() const;
        ValueUpdateCallback<bool> releaseOnValue();
        CurrentValueCallback<bool> releaseCurrentValue();
    };

    class OBJC_API_DLL NumericMenu : public ValueMenu {
        struct Impl;

        std::unique_ptr<Impl> m_impl;

        NumericMenu(std::unique_ptr<Impl> impl);
    public:
        enum class InputType {
            /// Renders the menu as a single text box.
            TextBox,
            /// Renders the menu as a text box with arrows.
            Arrows,
            /// Renders the menu as a text bpx with a slider.
            Slider
        };

        class OBJC_API_DLL Builder {
            friend class NumericMenu;

            std::unique_ptr<Impl> m_impl;

            Builder();
        public:
            ~Builder();
            /// @param id The ID assigned to the menu.
            Builder&& id(std::string id) &&;
            /// @param title The title of the popup section.
            Builder&& title(std::string title) &&;
            /// @param onValue A callback to digest any value changes.
            Builder&& onValue(ValueUpdateCallback<float> onValue) &&;
            /// @param currentValue The current value getter callback.
            Builder&& currentValue(CurrentValueCallback<float> currentValue) &&;
            /// @note Default is "Num".
            /// @param placeholder A placeholder label set when no value is present.
            Builder&& placeholder(std::string placeholder) &&;
            /// @note Default is NumericMenu::InputType::TextBox.
            /// @param inputType The type of input style.
            Builder&& inputType(InputType inputType) &&;
            /// @note Default is 2.
            /// @param precision The precision of the numeric value.
            Builder&& precision(size_t precision) &&;
            /// @note Default is 1.
            /// @param stepSize The step size of the input.
            Builder&& stepSize(float stepSize) &&;
            /// @note Default is std::nullopt.
            /// @note std::nullopt = No min value if range is false, 0 if true.
            /// @param min The min numeric value.
            Builder&& min(std::optional<float> min) &&;
            /// @note Default is std::nullopt.
            /// @note std::nullopt = No max value if range is false, 100 if true.
            /// @param max The max numeric value.
            Builder&& max(std::optional<float> max) &&;
            std::unique_ptr<NumericMenu> build() &&;
        };

        static Builder builder();

        NumericMenu& operator=(NumericMenu&& other) noexcept;
        NumericMenu& operator=(const NumericMenu& other) noexcept = delete;

        NumericMenu(NumericMenu&& other) noexcept;
        NumericMenu(const NumericMenu& other) noexcept = delete;
        ~NumericMenu();
        geode::ZStringView getID() const;
        geode::ZStringView getTitle() const;
        ValueUpdateCallback<float> releaseOnValue();
        CurrentValueCallback<float> releaseCurrentValue();
        geode::ZStringView getPlaceholder() const;
        InputType getInputType() const;
        size_t getPrecision() const;
        float getStepSize() const;
        const std::optional<float>& getMin() const;
        const std::optional<float>& getMax() const;
    };

    class OBJC_API_DLL InputMenu : public ValueMenu {
        struct Impl;

        std::unique_ptr<Impl> m_impl;

        InputMenu(std::unique_ptr<Impl> impl);
    public:
        class OBJC_API_DLL Builder {
            friend class InputMenu;

            std::unique_ptr<Impl> m_impl;

            Builder();
        public:
            ~Builder();
            /// @param id The ID assigned to the menu.
            Builder&& id(std::string id) &&;
            /// @param title The title of the popup section.
            Builder&& title(std::string title) &&;
            /// @param onValue A callback to digest any value changes.
            Builder&& onValue(ValueUpdateCallback<const std::string&> onValue) &&;
            /// @param currentValue The current value getter callback.
            Builder&& currentValue(CurrentValueCallback<std::string> currentValue) &&;
            /// @note Default is "Text".
            /// @param placeholder A placeholder label set when no text is present.
            Builder&& placeholder(std::string placeholder) &&;
            /// @note Empty = No filter.
            /// @param allowedChars The characters allowed in the input (due to limitations in Cocos it can't support any characters Cocos doesn't natively support).
            Builder&& allowedChars(std::string allowedChars) &&;
            /// @note Default is std::string::npos.
            /// @note std::string::npos = No limit.
            /// @param maxSize The max amount of characters in the input.
            Builder&& maxSize(size_t maxSize) &&;
            std::unique_ptr<InputMenu> build() &&;
        };

        static Builder builder();

        InputMenu& operator=(InputMenu&& other) noexcept;
        InputMenu& operator=(const InputMenu& other) noexcept = delete;

        InputMenu(InputMenu&& other) noexcept;
        InputMenu(const InputMenu& other) noexcept = delete;
        ~InputMenu();
        geode::ZStringView getID() const;
        geode::ZStringView getTitle() const;
        ValueUpdateCallback<const std::string&> releaseOnValue();
        CurrentValueCallback<std::string> releaseCurrentValue();
        geode::ZStringView getPlaceholder() const;
        geode::ZStringView getAllowedChars() const;
        size_t getMaxSize() const;
    };

    class OBJC_API_DLL EnumMenu : public ValueMenu {
        struct Impl;

        std::unique_ptr<Impl> m_impl;

        EnumMenu(std::unique_ptr<Impl> impl);
    public:
        class OBJC_API_DLL Builder {
            friend class EnumMenu;

            std::unique_ptr<Impl> m_impl;

            Builder();
        public:
            ~Builder();
            /// @param id The ID assigned to the menu.
            Builder&& id(std::string id) &&;
            /// @param title The title of the popup section.
            Builder&& title(std::string title) &&;
            /// @param onValue A callback to digest any value changes.
            Builder&& onValue(ValueUpdateCallback<const std::string&> onValue) &&;
            /// @param currentValue The current value getter callback.
            Builder&& currentValue(CurrentValueCallback<std::string> currentValue) &&;
            /// @param value An enum value.
            Builder&& value(std::string value) &&;
            /// @note An empty vector is considered invalid and will skip this node in the list.
            /// @param values The list of enum values.
            Builder&& values(std::vector<std::string> values) &&;
            std::unique_ptr<EnumMenu> build() &&;
        };

        static Builder builder();

        EnumMenu& operator=(EnumMenu&& other) noexcept;
        EnumMenu& operator=(const EnumMenu& other) noexcept = delete;

        EnumMenu(EnumMenu&& other) noexcept;
        EnumMenu(const EnumMenu& other) noexcept = delete;
        ~EnumMenu();
        geode::ZStringView getID() const;
        geode::ZStringView getTitle() const;
        ValueUpdateCallback<const std::string&> releaseOnValue();
        CurrentValueCallback<std::string> releaseCurrentValue();
        std::span<std::string> getValues() const;
        std::vector<std::string> releaseValues();
    };

    class OBJC_API_DLL CustomValueMenu : public ValueMenu {
        struct Impl;

        std::unique_ptr<Impl> m_impl;

        CustomValueMenu(std::unique_ptr<Impl> impl);
    public:
        class OBJC_API_DLL Builder {
            friend class CustomValueMenu;

            std::unique_ptr<Impl> m_impl;

            Builder();
        public:
            ~Builder();
            /// @param id The ID assigned to the menu.
            Builder&& id(std::string id) &&;
            /// @param title The title of the popup section.
            Builder&& title(std::string title) &&;
            /// @param factory The node factory for a custom value menu.
            Builder&& factory(CustomMenuFactory factory) &&;
            std::unique_ptr<CustomValueMenu> build() &&;
        };

        static Builder builder();

        CustomValueMenu& operator=(CustomValueMenu&& other) noexcept;
        CustomValueMenu& operator=(const CustomValueMenu& other) noexcept = delete;

        CustomValueMenu(CustomValueMenu&& other) noexcept;
        CustomValueMenu(const CustomValueMenu& other) noexcept = delete;
        ~CustomValueMenu();
        geode::ZStringView getID() const;
        geode::ZStringView getTitle() const;
        CustomMenuFactory releaseFactory();
    };

    class OBJC_API_DLL AxisLayoutMenu : public ValueMenu {
        struct Impl;

        std::unique_ptr<Impl> m_impl;

        AxisLayoutMenu(std::unique_ptr<Impl> impl);
    public:
        class OBJC_API_DLL Builder {
            friend class AxisLayoutMenu;

            std::unique_ptr<Impl> m_impl;

            Builder();
        public:
            ~Builder();
            /// @param id The ID assigned to the menu.
            Builder&& id(std::string id) &&;
            /// @param title The title of the popup section.
            Builder&& title(std::string title) &&;
            /// @note Default is geode::Axis::Column.
            /// @see geode::SimpleAxisLayout::setAxis
            /// @param axis The layout axis.
            Builder&& axis(geode::Axis axis) &&;
            /// @note Default is 10.
            /// @see geode::SimpleAxisLayout::setGap
            /// @param gap The layout gap.
            Builder&& gap(float gap) &&;
            /// @note Default is geode::AxisDirection::LeftToRight.
            /// @see geode::SimpleAxisLayout::setMainAxisDirection
            /// @param direction The layout main axis direction.
            Builder&& mainAxisDirection(geode::AxisDirection direction) &&;
            /// @note Default is geode::MainAxisAlignment::Start.
            /// @see geode::SimpleAxisLayout::setMainAxisAlignment
            /// @param direction The layout main axis alignment.
            Builder&& mainAxisAlignment(geode::MainAxisAlignment alignment) &&;
            /// @note Default is geode::AxisDirection::LeftToRight.
            /// @see geode::SimpleAxisLayout::setCrossAxisDirection
            /// @param direction The layout cross axis direction.
            Builder&& crossAxisDirection(geode::AxisDirection direction) &&;
            /// @note Default is geode::CrossAxisAlignment::Center.
            /// @see geode::SimpleAxisLayout::setCrossAxisAlignment
            /// @param direction The layout cross axis alignment.
            Builder&& crossAxisAlignment(geode::CrossAxisAlignment alignment) &&;
            /// @note Default is true.
            /// @see geode::SimpleAxisLayout::ignoreInvisibleChildren
            /// @param enabled If invisible children should be ignored in the layout.
            Builder&& ignoreInvisibleChildren(bool enabled) &&;
            /// @param menu A value menu shown in the layout.
            Builder&& menu(std::unique_ptr<ValueMenu> menu) &&;
            /// @param menus A list of value menus shown in the layout.
            Builder&& menus(std::vector<std::unique_ptr<ValueMenu>> menus) &&;
            std::unique_ptr<AxisLayoutMenu> build() &&;
        };

        static Builder builder();

        AxisLayoutMenu& operator=(AxisLayoutMenu&& other) noexcept;
        AxisLayoutMenu& operator=(const AxisLayoutMenu& other) noexcept = delete;

        AxisLayoutMenu(AxisLayoutMenu&& other) noexcept;
        AxisLayoutMenu(const AxisLayoutMenu& other) noexcept = delete;
        ~AxisLayoutMenu();
        geode::ZStringView getID() const;
        geode::ZStringView getTitle() const;
        geode::Axis getAxis() const;
        float getGap() const;
        geode::AxisDirection getMainAxisDirection() const;
        geode::MainAxisAlignment getMainAxisAlignment() const;
        geode::AxisDirection getCrossAxisDirection() const;
        geode::CrossAxisAlignment getCrossAxisAlignment() const;
        bool getIgnoreInvisibleChildren() const;
        std::span<std::unique_ptr<ValueMenu>> getMenus() const;
        std::vector<std::unique_ptr<ValueMenu>> releaseMenus();
    };

    class OBJC_API_DLL PopupConfig {
        struct Impl;

        std::unique_ptr<Impl> m_impl;

        PopupConfig(std::unique_ptr<Impl> impl);
    public:
        class OBJC_API_DLL Builder {
            friend class PopupConfig;

            std::unique_ptr<Impl> m_impl;

            Builder();
        public:
            ~Builder();
            /// @param id The ID assigned to the menu.
            Builder&& id(std::string id) &&;
            /// @param title The title of the popup section.
            Builder&& title(std::string title) &&;
            /// @note Default is 300.
            /// @param width The popup width.
            Builder&& width(float width) &&;
            /// @note Default is 200.
            /// @param height The popup height.
            Builder&& height(float height) &&;
            /// @note Default is 10.
            /// @param gap The gap on the X axis.
            Builder&& gapX(float gap) &&;
            /// @note Default is 10.
            /// @param gap The gap on the Y axis.
            Builder&& gapY(float gap) &&;
            /// @note Empty = No info.
            /// @param info The info button details of the popup.
            Builder&& info(InfoPopup info) &&;
            /// @param menu A value menu shown in the popup.
            Builder&& menu(std::unique_ptr<ValueMenu> menu) &&;
            /// @param menus A list of value menus shown in the popup.
            Builder&& menus(std::vector<std::unique_ptr<ValueMenu>> menus) &&;
            /// @note Default is false.
            /// @param enabled If true, adds the default trigger toggles to the toggle list.
            Builder&& triggerToggles(bool enabled) &&;
            /// @note Default is false.
            /// @param enabled If true, adds the default no multi activate toggle to the toggle list.
            Builder&& noMultiActivateToggle(bool enabled) &&;
            /// @param toggle A toggle in the bottom left corner of the popup.
            Builder&& toggle(std::unique_ptr<ToggleMenu> toggle) &&;
            /// @param toggles A list of toggles in the bottom left corner of the popup.
            Builder&& toggles(std::vector<std::unique_ptr<ToggleMenu>> toggles) &&;
            PopupConfig build() &&;
        };

        static Builder builder();

        PopupConfig& operator=(PopupConfig&& other) noexcept;
        PopupConfig& operator=(const PopupConfig& other) noexcept = delete;

        PopupConfig(PopupConfig&& other) noexcept;
        PopupConfig(const PopupConfig& other) noexcept = delete;
        ~PopupConfig();
        geode::ZStringView getID() const;
        geode::ZStringView getTitle() const;
        float getWidth() const;
        float getHeight() const;
        float getGapX() const;
        float getGapY() const;
        const InfoPopup& getInfo() const;
        std::span<std::unique_ptr<ValueMenu>> getMenus() const;
        std::vector<std::unique_ptr<ValueMenu>> releaseMenus();
        bool getTriggerToggles() const;
        bool getNoMultiActivateToggle() const;
        std::span<std::unique_ptr<ToggleMenu>> getToggles() const;
        std::vector<std::unique_ptr<ToggleMenu>> releaseToggles();
    };

    template<typename V, typename T>
    inline void applyValueToSelected(const Selected& selected, V T::* member, const V& value) {
        for (CustomObjectInterface* object : selected) {
            T* castedObject = geode::cast::typeinfo_cast<T*>(object);

            castedObject->*member = value;
        }
    }

    template<typename V, typename T>
    inline void applyValueToSelectedAndReport(const Selected& selected, V T::* member, const V& value, void (T::* reportMember)()) {
        for (CustomObjectInterface* object : selected) {
            T* castedObject = geode::cast::typeinfo_cast<T*>(object);

            castedObject->*member = value;
            (castedObject->*reportMember)();
        }
    }

    template<typename V>
    inline void applyValueToSelectedProperty(const Selected& selected, size_t property, const V& value) {
        const std::string stringValue = PropertyInterface::stringifyValue(value);

        for (CustomObjectInterface* object : selected) {
            object->updateProperty(property, stringValue);
        }
    }

    template<typename V, typename T>
    inline void applyValueToSelectedPropertyAndReport(const Selected& selected, size_t property, const V& value, void (T::* reportMember)()) {
        const std::string stringValue = PropertyInterface::stringifyValue(value);

        for (CustomObjectInterface* object : selected) {
            if (object->updateProperty(property, stringValue)) {
                (geode::cast::typeinfo_cast<T*>(object)->*reportMember)();
            }
        }
    }

    template<typename V, typename T>
    inline V getCommonValueOrDefault(const Selected& selected, V T::* member, auto defaultValue) {
        if (selected.empty()) return defaultValue;

        const V& firstValue = geode::cast::typeinfo_cast<T*>(selected[0])->*member;

        for (size_t i = 1; i < selected.size(); i++) {
            if (firstValue != geode::cast::typeinfo_cast<T*>(selected[i])->*member) return defaultValue;
        }

        return firstValue;
    }

    template<typename V>
    inline V getCommonPropertyValueOrDefault(const Selected& selected, size_t property) {
        if (selected.empty()) return V();

        const CustomProperties& firstCustomProperties = selected[0]->getCustomProperties();
        auto firstEntry = firstCustomProperties.find(property);

        if (firstEntry == firstCustomProperties.end()) return V();

        const Property<V>* firstProperty = geode::cast::typeinfo_cast<const Property<V>*>(firstEntry->second.get());
        V firstValue = firstProperty->getValue();

        for (size_t i = 1; i < selected.size(); i++) {
            const CustomProperties& customProperties = selected[i]->getCustomProperties();
            auto entry = customProperties.find(property);

            if (entry == customProperties.end()) {
                return firstProperty->getDefaultValue();
            } else if (const Property<V>* property = geode::cast::typeinfo_cast<const Property<V>*>(entry->second.get()); !property || property->getValue() != firstValue) {
                return firstProperty->getDefaultValue();
            }
        }

        return firstValue;
    }
}