#pragma once

#include "CustomObject.hpp"
#include "EditorPopupConfig.hpp"

namespace object_collab {
    class CustomObjectInterface;

    /// The variant of either the raw popup or the builder based configuration.
    using PopupOptions = std::variant<geode::Popup*, editor_popup::PopupConfig>;
    /// The method which returns the configuration for the edit popup.
    /// @param selected The selected objects.
    using ObjectPopupFactory = geode::Function<PopupOptions(const Selected& selected)>;
    /// The method which produces the custom object.
    using ObjectFactory = geode::Function<CustomObjectInterface*(ObjectInfo* info)>;

    enum class EditorTab {
        Solids = 0,
        TransparentSolids = 1,
        Slopes = 2,
        Hazards = 3,
        ThreeDimensionals = 4,
        Modifiers = 5,
        Animated = 6,
        Pixels = 7,
        Collectables = 8,
        Particles = 9,
        Decorations = 10,
        Saws = 11,
        Triggers = 12
    };

    class OBJC_API_DLL QuickObject {
        struct Impl;

        std::unique_ptr<Impl> m_impl;
    public:
        class OBJC_API_DLL Builder {
            friend class QuickObject;

            std::unique_ptr<QuickObject> m_config;

            Builder();
        public:
            ~Builder();
            /// @note Default is GameObjectType::Solid.
            /// @param objectType The object type which determines the vanilla behavior it inherits.
            [[nodiscard]] Builder&& objectType(GameObjectType objectType) &&;
            /// @note Default is ZLayer::Default.
            /// @param defaultZLayer The default z layer assigned when the object is created.
            [[nodiscard]] Builder&& defaultZLayer(ZLayer defaultZLayer) &&;
            /// @note Default is 2.
            /// @param defaultZOrder The default z order assigned when the object is created.
            [[nodiscard]] Builder&& defaultZOrder(int defaultZOrder) &&;
            [[nodiscard]] QuickObject build() &&;
        };

        [[nodiscard]] static Builder builder();

        QuickObject& operator=(QuickObject&& other) noexcept;
        QuickObject& operator=(const QuickObject& other) noexcept = delete;

        QuickObject(QuickObject&& other) noexcept;
        QuickObject(const QuickObject& other) noexcept = delete;
    private:
        QuickObject();
    public:
        ~QuickObject();
        [[nodiscard]] GameObjectType getObjectType() const;
        [[nodiscard]] ZLayer getDefaultZLayer() const;
        [[nodiscard]] int getDefaultZOrder() const;
    };

    class OBJC_API_DLL ComplexObject {
        struct Impl;

        std::unique_ptr<Impl> m_impl;
    public:
        class OBJC_API_DLL Builder {
            friend class ComplexObject;

            std::unique_ptr<ComplexObject> m_config;

            Builder();
        public:
            ~Builder();
            /// @note REQUIRED!
            /// @param factory The factory method to generate a custom object.
            [[nodiscard]] Builder&& factory(ObjectFactory factory) &&;
            /// @note Default is an empty map.
            /// @param customProperties The custom properties list this object uses. This will automate saving, loading & updating data.
            [[nodiscard]] Builder&& customProperties(CustomPropertiesList customProperties) &&;
            [[nodiscard]] ComplexObject build() &&;
        };

        [[nodiscard]] static Builder builder();

        ComplexObject& operator=(ComplexObject&& other) noexcept;
        ComplexObject& operator=(const ComplexObject& other) noexcept = delete;

        ComplexObject(ComplexObject&& other) noexcept;
        ComplexObject(const ComplexObject& other) noexcept = delete;
    private:
        ComplexObject();
    public:
        ~ComplexObject();
        [[nodiscard]] bool hasFactory() const;
        [[nodiscard]] CustomObjectInterface* factory(ObjectInfo* info) const;
        [[nodiscard]] const CustomProperties& getCustomProperties() const;
    };

    /// A selection of what construction method is used for the custom object.
    using ObjectConstruction = std::variant<QuickObject, ComplexObject>;

    class OBJC_API_DLL ObjectInfo {
        struct Impl;

        std::unique_ptr<Impl> m_impl;
    public:
        class OBJC_API_DLL Builder {
            friend class ObjectInfo;

            std::unique_ptr<ObjectInfo> m_config;

            Builder();
        public:
            ~Builder();
            /// @note REQUIRED!
            /// @warning This should never be changed! Changing the ID will make all saved objects with this ID be considered missing.
            /// @param id The id of the object which must be suffixed by _spr.
            [[nodiscard]] Builder&& id(std::string id) &&;
            /// @note REQUIRED!
            /// @param sprite The sprite of the object.
            [[nodiscard]] Builder&& sprite(std::string sprite) &&;
            /// @note Default is the default QuickObject setup.
            /// @param construction The object construction instructions.
            [[nodiscard]] Builder&& construction(ObjectConstruction construction) &&;
            /// @note Default is EditorTab::Solids.
            /// @param editorTab The editor tab the object will be shown in.
            [[nodiscard]] Builder&& editorTab(EditorTab editorTab) &&;
            /// @note Without it will default the edit object button.
            /// @param editObject The popup factory for the edit object button.
            [[nodiscard]] Builder&& editObject(ObjectPopupFactory editObject) &&;
            /// @note Without it will disable the edit special button.
            /// @param editSpecial The popup factory for the edit special button.
            [[nodiscard]] Builder&& editSpecial(ObjectPopupFactory editSpecial) &&;
            [[nodiscard]] ObjectInfo build() &&;
        };

        [[nodiscard]] static Builder builder();

        ObjectInfo& operator=(ObjectInfo&& other) noexcept;
        ObjectInfo& operator=(const ObjectInfo& other) noexcept = delete;

        ObjectInfo(ObjectInfo&& other) noexcept;
        ObjectInfo(const ObjectInfo& other) noexcept = delete;
    private:
        ObjectInfo();
    public:
        ~ObjectInfo();
        [[nodiscard]] std::string_view getID() const;
        [[nodiscard]] geode::ZStringView getSprite() const;
        [[nodiscard]] const ObjectConstruction& getConstruction() const;
        [[nodiscard]] EditorTab getEditorTab() const;
        [[nodiscard]] bool hasEditObject() const;
        [[nodiscard]] PopupOptions editObject(const Selected& selected) const;
        [[nodiscard]] bool hasEditSpecial() const;
        [[nodiscard]] PopupOptions editSpecial(const Selected& selected) const;
    };
}