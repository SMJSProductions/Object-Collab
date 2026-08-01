#include <EditorPopupConfig.hpp>

using namespace geode::prelude;
using namespace object_collab::editor_popup;

#define __BUILDER_PARAM__(name, type, variable, method) \
    name::Builder&& name::Builder::variable(type variable) && { \
        m_impl->variable = method(variable); \
        return std::move(*this); \
    }

#define BUILDER_PARAM(name, type, variable) __BUILDER_PARAM__(name, type, variable, std::move)
#define PRIMITIVE_BUILDER_PARAM(name, type, variable) __BUILDER_PARAM__(name, type, variable,)
#define VECTOR_PARAM(name, type, variable) \
    name::Builder&& name::Builder::variable(type variable) && { \
        m_impl->variable##s.emplace_back(std::move(variable)); \
        return std::move(*this); \
    }

#define CONFIG_GETTER(name, type, variable, title) type name::get##title() const { return m_impl->variable; }
#define VALUE_RELEASE(name, type, variable, title, default) type name::release##title() { \
        auto variable = std::move(m_impl->variable); \
        m_impl->variable = default; \
        return std::move(variable); \
    }

#define CONFIG_VALUE(name, inType, outType, variable, title) \
    BUILDER_PARAM(name, inType, variable) \
    CONFIG_GETTER(name, outType, variable, title)
#define PRIMITIVE_CONFIG_VALUE(name, type, variable, title) \
    PRIMITIVE_BUILDER_PARAM(name, type, variable) \
    CONFIG_GETTER(name, type, variable, title)
#define VECTOR_VALUE(name, type, variable, title) \
    VECTOR_PARAM(name, type, variable) \
    CONFIG_GETTER(name, std::span<type>, variable##s, title) \
    BUILDER_PARAM(name, std::vector<type>, variable##s) \
    VALUE_RELEASE(name, std::vector<type>, variable##s, title, std::vector<type>())

#define IMPL_VALUES() \
    std::string id; \
    std::string title
#define VALUE_IMPL_VALUES(type, ...) \
    IMPL_VALUES(); \
    ValueUpdateCallback<type __VA_ARGS__> onValue = [](type __VA_ARGS__ value, const Selected& selected, Popup* popup) { }; \
    CurrentValueCallback<type> currentValue = [](const Selected& selected, Popup* popup) { return type(); }

#define BUILD_IMPL(name) name name::Builder::build() && { return std::move(m_impl); }
#define PTR_BUILD_IMPL(name) std::unique_ptr<name> name::Builder::build() && { return std::make_unique<name>(name(std::move(m_impl))); }

#define CONFIG_IMPL(name) \
    name& name::operator=(name&& other) noexcept = default; \
    name::name(name&& other) noexcept = default; \
    name::name(std::unique_ptr<Impl> impl): m_impl(std::move(impl)) { } \
    name::~name() = default; \
    name::Builder name::builder() { return {}; } \
    name::Builder::Builder(): m_impl(std::make_unique<Impl>()) { } \
    name::Builder::~Builder() = default; \
    CONFIG_VALUE(name, std::string, ZStringView, id, ID) \
    CONFIG_VALUE(name, std::string, ZStringView, title, Title)
#define VALUE_IMPL(name, type, ...) \
    CONFIG_IMPL(name) \
    PTR_BUILD_IMPL(name) \
    BUILDER_PARAM(name, ValueUpdateCallback<type __VA_ARGS__>, onValue) \
    BUILDER_PARAM(name, CurrentValueCallback<type>, currentValue) \
    VALUE_RELEASE(name, ValueUpdateCallback<type __VA_ARGS__>, onValue, OnValue, nullptr) \
    VALUE_RELEASE(name, CurrentValueCallback<type>, currentValue, CurrentValue, nullptr)

struct InfoPopup::Impl {
    IMPL_VALUES();
    std::string description;
};

CONFIG_IMPL(InfoPopup);
BUILD_IMPL(InfoPopup);
CONFIG_VALUE(InfoPopup, std::string, ZStringView, description, Description);

struct ToggleMenu::Impl {
    VALUE_IMPL_VALUES(bool);
};

VALUE_IMPL(ToggleMenu, bool);

struct NumericMenu::Impl {
    VALUE_IMPL_VALUES(float);
    std::string placeholder = "Num";
    NumericMenu::InputType inputType = NumericMenu::InputType::TextBox;
    size_t precision = 2;
    float stepSize = 1;
    std::optional<float> min = std::nullopt;
    std::optional<float> max = std::nullopt;
};

VALUE_IMPL(NumericMenu, float);
CONFIG_VALUE(NumericMenu, std::string, ZStringView, placeholder, Placeholder);
PRIMITIVE_CONFIG_VALUE(NumericMenu, NumericMenu::InputType, inputType, InputType);
PRIMITIVE_CONFIG_VALUE(NumericMenu, size_t, precision, Precision);
PRIMITIVE_CONFIG_VALUE(NumericMenu, float, stepSize, StepSize);
CONFIG_VALUE(NumericMenu, std::optional<float>, const std::optional<float>&, min, Min);
CONFIG_VALUE(NumericMenu, std::optional<float>, const std::optional<float>&, max, Max);

struct InputMenu::Impl {
    VALUE_IMPL_VALUES(std::string, const&);
    std::string placeholder = "Text";
    std::string allowedChars;
    size_t maxSize = std::string::npos;
};

VALUE_IMPL(InputMenu, std::string, const&);
CONFIG_VALUE(InputMenu, std::string, ZStringView, placeholder, Placeholder);
CONFIG_VALUE(InputMenu, std::string, ZStringView, allowedChars, AllowedChars);
PRIMITIVE_CONFIG_VALUE(InputMenu, size_t, maxSize, MaxSize);

struct EnumMenu::Impl {
    VALUE_IMPL_VALUES(std::string, const&);
    EnumValues values = std::vector<std::string>();
};

VALUE_IMPL(EnumMenu, std::string, const&);

EnumMenu::Builder&& EnumMenu::Builder::value(std::string value) && {
    if (std::holds_alternative<std::vector<EnumMenu::AliasedValue>>(m_impl->values)) {
        m_impl->values = std::vector<std::string>();
    }

    std::get<std::vector<std::string>>(m_impl->values).emplace_back(std::move(value));

    return std::move(*this);
}

EnumMenu::Builder&& EnumMenu::Builder::values(EnumValues values) && {
    m_impl->values = std::move(values);

    return std::move(*this);
}

const EnumMenu::EnumValues& EnumMenu::getValues() const {
    return m_impl->values;
}

EnumMenu::EnumValues EnumMenu::releaseValues() {
    EnumValues values = std::move(m_impl->values);

    m_impl->values = std::vector<std::string>();

    return std::move(values);
}

struct CustomValueMenu::Impl {
    IMPL_VALUES();
    CustomMenuFactory factory;
};

CONFIG_IMPL(CustomValueMenu);
PTR_BUILD_IMPL(CustomValueMenu);
BUILDER_PARAM(CustomValueMenu, CustomValueMenu::CustomMenuFactory, factory);
VALUE_RELEASE(CustomValueMenu, CustomValueMenu::CustomMenuFactory, factory, Factory, nullptr);

struct AxisLayoutMenu::Impl {
    IMPL_VALUES();
    std::vector<std::unique_ptr<ValueMenu>> menus;
    Axis axis = Axis::Column;
    float gap = 10;
    AxisDirection mainAxisDirection = AxisDirection::LeftToRight;
    MainAxisAlignment mainAxisAlignment = MainAxisAlignment::Start;
    AxisDirection crossAxisDirection = AxisDirection::LeftToRight;
    CrossAxisAlignment crossAxisAlignment = CrossAxisAlignment::Center;
    bool ignoreInvisibleChildren = true;
};

CONFIG_IMPL(AxisLayoutMenu);
PTR_BUILD_IMPL(AxisLayoutMenu);
VECTOR_VALUE(AxisLayoutMenu, std::unique_ptr<ValueMenu>, menu, Menus);
PRIMITIVE_CONFIG_VALUE(AxisLayoutMenu, Axis, axis, Axis);
PRIMITIVE_CONFIG_VALUE(AxisLayoutMenu, float, gap, Gap);
PRIMITIVE_CONFIG_VALUE(AxisLayoutMenu, AxisDirection, mainAxisDirection, MainAxisDirection);
PRIMITIVE_CONFIG_VALUE(AxisLayoutMenu, MainAxisAlignment, mainAxisAlignment, MainAxisAlignment);
PRIMITIVE_CONFIG_VALUE(AxisLayoutMenu, AxisDirection, crossAxisDirection, CrossAxisDirection);
PRIMITIVE_CONFIG_VALUE(AxisLayoutMenu, CrossAxisAlignment, crossAxisAlignment, CrossAxisAlignment);
PRIMITIVE_CONFIG_VALUE(AxisLayoutMenu, bool, ignoreInvisibleChildren, IgnoreInvisibleChildren);

struct PopupConfig::Impl {
    IMPL_VALUES();
    float width = 300;
    float height = 200;
    float gapX = 10;
    float gapY = 10;
    InfoPopup info = InfoPopup::builder().build();
    std::vector<std::unique_ptr<ValueMenu>> menus;
    bool triggerToggles = false;
    bool noMultiActivateToggle = false;
    std::vector<std::unique_ptr<ToggleMenu>> toggles;
};

CONFIG_IMPL(PopupConfig);
BUILD_IMPL(PopupConfig);
PRIMITIVE_CONFIG_VALUE(PopupConfig, float, width, Width);
PRIMITIVE_CONFIG_VALUE(PopupConfig, float, height, Height);
PRIMITIVE_CONFIG_VALUE(PopupConfig, float, gapX, GapX);
PRIMITIVE_CONFIG_VALUE(PopupConfig, float, gapY, GapY);
CONFIG_VALUE(PopupConfig, InfoPopup, const InfoPopup&, info, Info);
VECTOR_VALUE(PopupConfig, std::unique_ptr<ValueMenu>, menu, Menus);
PRIMITIVE_CONFIG_VALUE(PopupConfig, bool, triggerToggles, TriggerToggles);
PRIMITIVE_CONFIG_VALUE(PopupConfig, bool, noMultiActivateToggle, NoMultiActivateToggle);
VECTOR_VALUE(PopupConfig, std::unique_ptr<ToggleMenu>, toggle, Toggles);