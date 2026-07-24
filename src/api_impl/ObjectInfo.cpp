#include <ObjectInfo.hpp>

using namespace object_collab;
using namespace geode::prelude;

struct ObjectInfo::Impl {
    std::string id;
    std::string sprite;
    EditorTab editorTab = EditorTab::SOLIDS;
    ObjectFactory factory = nullptr;
    ObjectPopupFactory editObject = nullptr;
    ObjectPopupFactory editSpecial = nullptr;
};

ObjectInfo::Builder ObjectInfo::builder() {
    return ObjectInfo::Builder();
}

ObjectInfo& ObjectInfo::operator=(ObjectInfo&& other) = default;

ObjectInfo::ObjectInfo(ObjectInfo&& other) = default;

ObjectInfo::ObjectInfo(): m_impl(std::make_unique<Impl>()) { }

ObjectInfo::~ObjectInfo() { }

std::string_view ObjectInfo::getID() const {
    return m_impl->id;
}

ZStringView ObjectInfo::getSprite() const {
    return m_impl->sprite;
}

EditorTab ObjectInfo::getEditorTab() const {
    return m_impl->editorTab;
}

bool ObjectInfo::hasFactory() const {
    return m_impl->factory != nullptr;
}

CustomObjectInterface* ObjectInfo::factory() const {
    return m_impl->factory();
}

bool ObjectInfo::hasEditObject() const {
    return m_impl->editObject != nullptr;
}

PopupOptions ObjectInfo::editObject(const Selected& selected) const {
    return m_impl->editObject(selected);
}

bool ObjectInfo::hasEditSpecial() const {
    return m_impl->editSpecial != nullptr;
}

PopupOptions ObjectInfo::editSpecial(const Selected& selected) const {
    return m_impl->editSpecial(selected);
}

ObjectInfo::Builder::Builder(): m_config(std::unique_ptr<ObjectInfo>(new ObjectInfo())) { }

ObjectInfo::Builder::~Builder() { }

ObjectInfo::Builder&& ObjectInfo::Builder::id(std::string id) && {
    m_config->m_impl->id = std::move(id);

    return std::forward<ObjectInfo::Builder>(*this);
}

ObjectInfo::Builder&& ObjectInfo::Builder::sprite(std::string sprite) && {
    m_config->m_impl->sprite = std::move(sprite);

    return std::forward<ObjectInfo::Builder>(*this);
}

ObjectInfo::Builder&& ObjectInfo::Builder::editorTab(EditorTab editorTab) && {
    m_config->m_impl->editorTab = editorTab;

    return std::forward<ObjectInfo::Builder>(*this);
}

ObjectInfo::Builder&& ObjectInfo::Builder::factory(ObjectFactory factory) && {
    m_config->m_impl->factory = std::move(factory);

    return std::forward<ObjectInfo::Builder>(*this);
}

ObjectInfo::Builder&& ObjectInfo::Builder::editObject(ObjectPopupFactory editObject) && {
    m_config->m_impl->editObject = std::move(editObject);

    return std::forward<ObjectInfo::Builder>(*this);
}

ObjectInfo::Builder&& ObjectInfo::Builder::editSpecial(ObjectPopupFactory editSpecial) && {
    m_config->m_impl->editSpecial = std::move(editSpecial);

    return std::forward<ObjectInfo::Builder>(*this);
}

ObjectInfo ObjectInfo::Builder::build() && {
    return std::move(*m_config);
}