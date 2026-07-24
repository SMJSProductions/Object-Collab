#pragma once

#include <CustomObject.hpp>
#include <EditorPopupConfig.hpp>

namespace object_collab {
    using PopupOptions = std::variant<geode::Popup*, editor_popup::PopupConfig>;
    using ObjectFactory = geode::Function<CustomObjectInterface*()>;
    using ObjectPopupFactory = geode::Function<PopupOptions(const Selected& selected)>;

    enum class EditorTab {
        SOLIDS = 0,
        TRANSPARENT_SOLIDS = 1,
        SLOPES = 2,
        HAZARDS = 3,
        THREE_DIMENSIONALS = 4,
        MODIFIERS = 5,
        EFFECTS = 6,
        PIXELS = 7,
        COLLLECTABLES = 8,
        PARTICLES = 9,
        DECORATIONS = 10,
        SAWS = 11,
        TRIGGERS = 12
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
            Builder&& id(std::string id) &&;
            Builder&& sprite(std::string sprite) &&;
            Builder&& editorTab(EditorTab editorTab) &&;
            Builder&& factory(ObjectFactory factory) &&;
            Builder&& editObject(ObjectPopupFactory editObject) &&;
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
        EditorTab getEditorTab() const;
        bool hasFactory() const;
        CustomObjectInterface* factory() const;
        bool hasEditObject() const;
        PopupOptions editObject(const Selected& selected) const;
        bool hasEditSpecial() const;
        PopupOptions editSpecial(const Selected& selected) const;
    };
}