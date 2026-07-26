#include <ObjectInfo.hpp>

using namespace object_collab;
using namespace geode::prelude;

struct ObjectInfo::Impl {
    std::string id;
    std::string sprite;
    GameObjectType objectType = GameObjectType::Solid;
    ZLayer defaultZLayer = ZLayer::Default;
    int defaultZOrder = 2;
    EditorTab editorTab = EditorTab::Solids;
    ObjectFactory factory = nullptr;
    ObjectPopupFactory editObject = nullptr;
    ObjectPopupFactory editSpecial = nullptr;
};

ObjectInfo::Builder ObjectInfo::builder() {
    return ObjectInfo::Builder();
}

ObjectInfo& ObjectInfo::operator=(ObjectInfo&& other) noexcept = default;

ObjectInfo::ObjectInfo(ObjectInfo&& other) noexcept = default;

ObjectInfo::ObjectInfo(): m_impl(std::make_unique<Impl>()) { }

ObjectInfo::~ObjectInfo() = default;

std::string_view ObjectInfo::getID() const {
    return m_impl->id;
}

ZStringView ObjectInfo::getSprite() const {
    return m_impl->sprite;
}

GameObjectType ObjectInfo::getObjectType() const {
    return m_impl->objectType;
}

ZLayer ObjectInfo::getDefaultZLayer() const {
    return m_impl->defaultZLayer;
}

int ObjectInfo::getDefaultZOrder() const {
    return m_impl->defaultZOrder;
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

ObjectInfo::Builder::~Builder() = default;

ObjectInfo::Builder&& ObjectInfo::Builder::id(std::string id) && {
    m_config->m_impl->id = std::move(id);

    return std::forward<ObjectInfo::Builder>(*this);
}

ObjectInfo::Builder&& ObjectInfo::Builder::sprite(std::string sprite) && {
    m_config->m_impl->sprite = std::move(sprite);

    return std::forward<ObjectInfo::Builder>(*this);
}

ObjectInfo::Builder&& ObjectInfo::Builder::objectType(GameObjectType objectType) && {
    m_config->m_impl->objectType = objectType;

    return std::forward<ObjectInfo::Builder>(*this);
}

ObjectInfo::Builder&& ObjectInfo::Builder::defaultZLayer(ZLayer defaultZLayer) && {
    m_config->m_impl->defaultZLayer = defaultZLayer;

    return std::forward<ObjectInfo::Builder>(*this);
}

ObjectInfo::Builder&& ObjectInfo::Builder::defaultZOrder(int defaultZOrder) && {
    m_config->m_impl->defaultZOrder = defaultZOrder;

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