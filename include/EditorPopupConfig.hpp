#pragma once

#include <CustomObject.hpp>

namespace object_collab {
    using Selected = std::vector<CustomObjectnterface*>;
}

namespace object_collab::editor_popup {
    using namespace object_collab;

    /// @param value The value to digest (ownership is moved to the callback)
    /// @param selected The currently selected objects
    /// @param popup The editor popup instance
    template<typename T>
    using ValueUpdateCallback = geode::Function<void(T value, const Selected& selected, geode::Popup* popup)>;

    /// @param selected The currently selected objects
    /// @param popup The editor popup instance
    /// @returns The current agreed upon value
    template<typename T>
    using CurrentValueCallback = geode::Function<T(const Selected& selected, geode::Popup* popup)>;

    /// @param popup The editor popup instance
    /// @returns The menu
    using MenuFactory = geode::Function<cocos2d::CCMenu*(const Selected& selected, geode::Popup* popup)>;

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
            /// @param id The ID assigned to the menu
            Builder&& id(std::string id) &&;
            /// @param title The title of the popup section
            Builder&& title(std::string title) &&;
            /// @param description The description of the info popup which supports color tags (e.g. `<cy></c>`)
            Builder&& description(std::string description) &&;
            InfoPopup build() &&;
        };

        static Builder builder();

        InfoPopup& operator=(InfoPopup&& other);
        InfoPopup& operator=(const InfoPopup& other) = delete;

        InfoPopup(InfoPopup&& other);
        InfoPopup(const InfoPopup& other) = delete;
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
            /// @param id The ID assigned to the menu
            Builder&& id(std::string id) &&;
            /// @param title The title of the popup section
            Builder&& title(std::string title) &&;
            /// @param onValue A callback to digest any value changes
            Builder&& onValue(ValueUpdateCallback<bool> onValue) &&;
            /// @param currentValue The current value getter callback
            Builder&& currentValue(CurrentValueCallback<bool> currentValue) &&;
            std::unique_ptr<ToggleMenu> build() &&;
        };

        static Builder builder();

        ToggleMenu& operator=(ToggleMenu&& other);
        ToggleMenu& operator=(const ToggleMenu& other) = delete;

        ToggleMenu(ToggleMenu&& other);
        ToggleMenu(const ToggleMenu& other) = delete;
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
            TEXT_BOX,
            ARROWS,
            SLIDER
        };

        class OBJC_API_DLL Builder {
            friend class NumericMenu;

            std::unique_ptr<Impl> m_impl;

            Builder();
        public:
            ~Builder();
            /// @param id The ID assigned to the menu
            Builder&& id(std::string id) &&;
            /// @param title The title of the popup section
            Builder&& title(std::string title) &&;
            /// @param onValue A callback to digest any value changes
            Builder&& onValue(ValueUpdateCallback<float> onValue) &&;
            /// @param currentValue The current value getter callback
            Builder&& currentValue(CurrentValueCallback<float> currentValue) &&;
            /// @param placeholder A placeholder label set when no value is present
            Builder&& placeholder(std::string placeholder) &&;
            /// @param inputType The type of input style
            Builder&& inputType(InputType inputType) &&;
            /// @param precision The precision of the numeric value
            Builder&& precision(size_t precision) &&;
            /// @param min The min numeric value
            /// @note std::nullopt = No min value if range is false, 0 if true
            Builder&& min(std::optional<float> min) &&;
            /// @param max The max numeric value
            /// @note std::nullopt = No max value if range is false, 100 if true
            Builder&& max(std::optional<float> max) &&;
            std::unique_ptr<NumericMenu> build() &&;
        };

        static Builder builder();

        NumericMenu& operator=(NumericMenu&& other);
        NumericMenu& operator=(const NumericMenu& other) = delete;

        NumericMenu(NumericMenu&& other);
        NumericMenu(const NumericMenu& other) = delete;
        ~NumericMenu();
        geode::ZStringView getID() const;
        geode::ZStringView getTitle() const;
        ValueUpdateCallback<float> releaseOnValue();
        CurrentValueCallback<float> releaseCurrentValue();
        geode::ZStringView getPlaceholder() const;
        InputType getInputType() const;
        size_t getPrecision() const;
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
            /// @param id The ID assigned to the menu
            Builder&& id(std::string id) &&;
            /// @param title The title of the popup section
            Builder&& title(std::string title) &&;
            /// @param onValue A callback to digest any value changes
            Builder&& onValue(ValueUpdateCallback<std::string> onValue) &&;
            /// @param currentValue The current value getter callback
            Builder&& currentValue(CurrentValueCallback<std::string> currentValue) &&;
            /// @param placeholder A placeholder label set when no text is present
            Builder&& placeholder(std::string placeholder) &&;
            /// @param allowedChars The characters allowed in the input (due to limitations in Cocos it can't support any characters Cocos doesn't natively support)
            /// @note Empty = No filter
            Builder&& allowedChars(std::string allowedChars) &&;
            /// @param maxSize The max amount of characters in the input
            /// @note std::string::npos = No limit
            Builder&& maxSize(size_t maxSize) &&;
            std::unique_ptr<InputMenu> build() &&;
        };

        static Builder builder();

        InputMenu& operator=(InputMenu&& other);
        InputMenu& operator=(const InputMenu& other) = delete;

        InputMenu(InputMenu&& other);
        InputMenu(const InputMenu& other) = delete;
        ~InputMenu();
        geode::ZStringView getID() const;
        geode::ZStringView getTitle() const;
        ValueUpdateCallback<std::string> releaseOnValue();
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
            /// @param id The ID assigned to the menu
            Builder&& id(std::string id) &&;
            /// @param title The title of the popup section
            Builder&& title(std::string title) &&;
            /// @param onValue A callback to digest any value changes
            Builder&& onValue(ValueUpdateCallback<std::string> onValue) &&;
            /// @param currentValue The current value getter callback
            Builder&& currentValue(CurrentValueCallback<std::string> currentValue) &&;
            /// @param value An enum value
            Builder&& value(std::string value) &&;
            /// @param values The list of enum values
            /// @note An empty vector is considered invalid and will skip this node in the list
            Builder&& values(std::vector<std::string> values) &&;
            std::unique_ptr<EnumMenu> build() &&;
        };

        static Builder builder();

        EnumMenu& operator=(EnumMenu&& other);
        EnumMenu& operator=(const EnumMenu& other) = delete;

        EnumMenu(EnumMenu&& other);
        EnumMenu(const EnumMenu& other) = delete;
        ~EnumMenu();
        geode::ZStringView getID() const;
        geode::ZStringView getTitle() const;
        ValueUpdateCallback<std::string> releaseOnValue();
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
            /// @param id The ID assigned to the menu
            Builder&& id(std::string id) &&;
            /// @param title The title of the popup section
            Builder&& title(std::string title) &&;
            /// @param factory The node factory for a custom value menu
            Builder&& factory(MenuFactory factory) &&;
            std::unique_ptr<CustomValueMenu> build() &&;
        };

        static Builder builder();

        CustomValueMenu& operator=(CustomValueMenu&& other);
        CustomValueMenu& operator=(const CustomValueMenu& other) = delete;

        CustomValueMenu(CustomValueMenu&& other);
        CustomValueMenu(const CustomValueMenu& other) = delete;
        ~CustomValueMenu();
        geode::ZStringView getID() const;
        geode::ZStringView getTitle() const;
        MenuFactory releaseFactory();
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
            /// @param id The ID assigned to the menu
            Builder&& id(std::string id) &&;
            /// @param title The title of the popup section
            Builder&& title(std::string title) &&;
            /// @param axis The layout axis
            /// @see geode::SimpleAxisLayout::setAxis
            Builder&& axis(geode::Axis axis) &&;
            /// @param gap The layout gap
            /// @see geode::SimpleAxisLayout::setGap
            Builder&& gap(float gap) &&;
            /// @param direction The layout main axis direction
            /// @see geode::SimpleAxisLayout::setMainAxisDirection
            Builder&& mainAxisDirection(geode::AxisDirection direction) &&;
            /// @param direction The layout main axis alignment
            /// @see geode::SimpleAxisLayout::setMainAxisAlignment
            Builder&& mainAxisAlignment(geode::MainAxisAlignment alignment) &&;
            /// @param direction The layout cross axis direction
            /// @see geode::SimpleAxisLayout::setCrossAxisDirection
            Builder&& crossAxisDirection(geode::AxisDirection direction) &&;
            /// @param direction The layout cross axis alignment
            /// @see geode::SimpleAxisLayout::setCrossAxisAlignment
            Builder&& crossAxisAlignment(geode::CrossAxisAlignment alignment) &&;
            /// @param enabled If invisible children should be ignored in the layout
            /// @see geode::SimpleAxisLayout::ignoreInvisibleChildren
            Builder&& ignoreInvisibleChildren(bool enabled) &&;
            /// @param menu A value menu shown in the layout
            Builder&& menu(std::unique_ptr<ValueMenu> menu) &&;
            /// @param menus A list of value menus shown in the layout
            Builder&& menus(std::vector<std::unique_ptr<ValueMenu>> menus) &&;
            std::unique_ptr<AxisLayoutMenu> build() &&;
        };

        static Builder builder();

        AxisLayoutMenu& operator=(AxisLayoutMenu&& other);
        AxisLayoutMenu& operator=(const AxisLayoutMenu& other) = delete;

        AxisLayoutMenu(AxisLayoutMenu&& other);
        AxisLayoutMenu(const AxisLayoutMenu& other) = delete;
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
            /// @param id The ID assigned to the menu
            Builder&& id(std::string id) &&;
            /// @param title The title of the popup section
            Builder&& title(std::string title) &&;
            /// @param width The popup width
            Builder&& width(float width) &&;
            /// @param height The popup height
            Builder&& height(float height) &&;
            /// @param info The info button details of the popup
            /// @note Empty = No info
            Builder&& info(InfoPopup info) &&;
            /// @param menu A value menu shown in the popup
            Builder&& menu(std::unique_ptr<ValueMenu> menu) &&;
            /// @param menus A list of value menus shown in the popup
            Builder&& menus(std::vector<std::unique_ptr<ValueMenu>> menus) &&;
            /// @param enabled If true, adds the default trigger toggles to the toggle list
            Builder&& triggerToggles(bool enabled) &&;
            /// @param enabled If true, adds the default no multi activate toggle to the toggle list
            Builder&& noMultiActivateToggle(bool enabled) &&;
            /// @param toggle A toggle in the bottom left corner of the popup
            Builder&& toggle(std::unique_ptr<ToggleMenu> toggle) &&;
            /// @param toggles A list of toggles in the bottom left corner of the popup
            Builder&& toggles(std::vector<std::unique_ptr<ToggleMenu>> toggles) &&;
            PopupConfig build() &&;
        };

        static Builder builder();

        PopupConfig& operator=(PopupConfig&& other);
        PopupConfig& operator=(const PopupConfig& other) = delete;

        PopupConfig(PopupConfig&& other);
        PopupConfig(const PopupConfig& other) = delete;
        ~PopupConfig();
        geode::ZStringView getID() const;
        geode::ZStringView getTitle() const;
        float getWidth() const;
        float getHeight() const;
        const InfoPopup& getInfo() const;
        std::span<std::unique_ptr<ValueMenu>> getMenus() const;
        std::vector<std::unique_ptr<ValueMenu>> releaseMenus();
        bool getTriggerToggles() const;
        bool getNoMultiActivateToggle() const;
        std::span<std::unique_ptr<ToggleMenu>> getToggles() const;
        std::vector<std::unique_ptr<ToggleMenu>> releaseToggles();
    };

    template<typename T, typename Member, typename Value>
    inline void applyValueToSelected(const Selected& selected, Member T::* member, const Value& value) {
        for (CustomObjectnterface* object : selected) {
            geode::cast::typeinfo_cast<T*>(object)->*member = value;
        }
    }

    template<typename T, typename Member, typename Value, typename F> requires std::invocable<F, T*> && std::is_convertible_v<std::invoke_result_t<F, T*>, bool>
    inline void applyValueToSelectedIf(const Selected& selected, Member T::* member, const Value& value, F&& condition) {
        for (CustomObjectnterface* object : selected) {
            T* castedObject = geode::cast::typeinfo_cast<T*>(object);

            if (condition(castedObject)) castedObject->*member = value;
        }
    }

    template<typename T, typename Member, typename Value>
    inline Value getCommonValueOrDefault(const Selected& selected, Member T::* member, Value defaultValue) {
        if (selected.empty()) return defaultValue;

        const Value& firstValue = geode::cast::typeinfo_cast<T*>(selected[0])->*member;

        for (size_t i = 1; i < selected.size(); i++) {
            if (firstValue != geode::cast::typeinfo_cast<T*>(selected[i])->*member) return defaultValue;
        }

        return firstValue;
    }

    template<typename T, typename Member, typename Value>
    inline Value getOnlyValueOrDefault(const Selected& selected, Member T::* member, Value defaultValue) {
        if (selected.size() == 1) {
            return geode::cast::typeinfo_cast<T*>(selected[0])->*member;
        } else {
            return defaultValue;
        }
    }
}