#pragma once

#include <CustomObject.hpp>
#include <EditorPopupConfig.hpp>

namespace object_collab {
    using PopupOptions = std::variant<geode::Popup*, editor_popup::PopupConfig>;
    using ObjectFactory = geode::Function<CustomObjectInterface*()>;
    using ObjectPopupFactory = geode::Function<PopupOptions(const Selected& selected)>;

    enum class EditorTab {
        Solids = 0,
        TransparentSolids = 1,
        Slopes = 2,
        Hazards = 3,
        ThreeDimensionals = 4,
        Modifiers = 5,
        Effects = 6,
        Pixels = 7,
        Collectables = 8,
        Particles = 9,
        Decorations = 10,
        Saws = 11,
        Triggers = 12
    };

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
            /// @param id The id of the object.
            Builder&& id(std::string id) &&;
            /// @note REQUIRED!
            /// @param sprite The sprite of the object.
            Builder&& sprite(std::string sprite) &&;
            /// @note Default is GameObjectType::Solid.
            /// @warning This only works for objects with no associated factory!
            /// @param objectType The object type which determines the vanilla behavior it inherits.
            Builder&& objectType(GameObjectType objectType) &&;
            /// @note Default is EditorTab::Solids.
            /// @param editorTab The editor tab the object will be shown in.
            Builder&& editorTab(EditorTab editorTab) &&;
            /// @note Without it will fallback to a simple GameObject implement.
            /// @param factory The factory method to generate a custom object.
            Builder&& factory(ObjectFactory factory) &&;
            /// @note Without it will default the edit object button.
            /// @param editObject The popup factory for the edit object button.
            Builder&& editObject(ObjectPopupFactory editObject) &&;
            /// @note Without it will disable the edit special button.
            /// @param editSpecial The popup factory for the edit special button.
            Builder&& editSpecial(ObjectPopupFactory editSpecial) &&;
            ObjectInfo build() &&;
        };

        static Builder builder();

        ObjectInfo& operator=(ObjectInfo&& other);
        ObjectInfo& operator=(const ObjectInfo& other) = delete;

        ObjectInfo(ObjectInfo&& other);
        ObjectInfo(const ObjectInfo& other) = delete;
    private:
        ObjectInfo();
    public:
        ~ObjectInfo();
        std::string_view getID() const;
        geode::ZStringView getSprite() const;
        GameObjectType getObjectType() const;
        EditorTab getEditorTab() const;
        bool hasFactory() const;
        CustomObjectInterface* factory() const;
        bool hasEditObject() const;
        PopupOptions editObject(const Selected& selected) const;
        bool hasEditSpecial() const;
        PopupOptions editSpecial(const Selected& selected) const;
    };
}