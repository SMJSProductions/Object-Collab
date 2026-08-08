#include <ObjectInfo.hpp>
#include "../hooks/editor/EditorUI.hpp"

using namespace object_collab;
using namespace geode::prelude;

struct QuickObject::Impl {
    GameObjectType objectType = GameObjectType::Solid;
    ZLayer defaultZLayer = ZLayer::Default;
    int defaultZOrder = 2;
};

struct ComplexObject::Impl {
    ObjectFactory factory = nullptr;
    CustomProperties customProperties;
};

struct ObjectInfo::Impl {
    std::string id;
    std::string sprite;
    ObjectConstruction construction = QuickObject::builder().build();
    EditorTab editorTab = EditorTab::Solids;
    EditorButtonColor editorButtonColor = EditorButtonColor::Aqua;
    ObjectPopupFactory editObject = nullptr;
    ObjectPopupFactory editSpecial = nullptr;
};

QuickObject::Builder::Builder(): m_config(std::unique_ptr<QuickObject>(new QuickObject())) { }

QuickObject::Builder::~Builder() = default;

QuickObject::Builder QuickObject::builder() {
    return QuickObject::Builder();
}

QuickObject::Builder&& QuickObject::Builder::objectType(GameObjectType objectType) && {
    m_config->m_impl->objectType = objectType;

    return std::forward<QuickObject::Builder>(*this);
}

QuickObject::Builder&& QuickObject::Builder::defaultZLayer(ZLayer defaultZLayer) && {
    m_config->m_impl->defaultZLayer = defaultZLayer;

    return std::forward<QuickObject::Builder>(*this);
}

QuickObject::Builder&& QuickObject::Builder::defaultZOrder(int defaultZOrder) && {
    m_config->m_impl->defaultZOrder = defaultZOrder;

    return std::forward<QuickObject::Builder>(*this);
}

QuickObject QuickObject::Builder::build() && {
    return std::move(*m_config);
}

QuickObject& QuickObject::operator=(QuickObject&& other) noexcept = default;

QuickObject::QuickObject(QuickObject&& other) noexcept = default;

QuickObject::QuickObject(): m_impl(std::make_unique<Impl>()) { }

QuickObject::~QuickObject() = default;

GameObjectType QuickObject::getObjectType() const {
    return m_impl->objectType;
}

ZLayer QuickObject::getDefaultZLayer() const {
    return m_impl->defaultZLayer;
}

int QuickObject::getDefaultZOrder() const {
    return m_impl->defaultZOrder;
}

ComplexObject::Builder::Builder(): m_config(std::unique_ptr<ComplexObject>(new ComplexObject())) { }

ComplexObject::Builder::~Builder() = default;

ComplexObject::Builder&& ComplexObject::Builder::factory(ObjectFactory factory) && {
    m_config->m_impl->factory = std::move(factory);

    return std::forward<ComplexObject::Builder>(*this);
}

ComplexObject::Builder&& ComplexObject::Builder::customProperties(CustomPropertiesList customProperties) && {
    m_config->m_impl->customProperties = std::forward<CustomPropertiesList>(customProperties).releaseMap();

    return std::forward<ComplexObject::Builder>(*this);
}

ComplexObject ComplexObject::Builder::build() && {
    return std::move(*m_config);
}

ComplexObject::Builder ComplexObject::builder() {
    return ComplexObject::Builder();
}

ComplexObject& ComplexObject::operator=(ComplexObject&& other) noexcept = default;

ComplexObject::ComplexObject(ComplexObject&& other) noexcept = default;

ComplexObject::ComplexObject(): m_impl(std::make_unique<Impl>()) { }

ComplexObject::~ComplexObject() = default;

bool ComplexObject::hasFactory() const {
    return m_impl->factory != nullptr;
}

CustomObjectInterface* ComplexObject::factory(ObjectInfo* info) const {
    return m_impl->factory(info);
}

const CustomProperties& ComplexObject::getCustomProperties() const {
    return m_impl->customProperties;
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

ObjectInfo::Builder&& ObjectInfo::Builder::construction(ObjectConstruction construction) && {
    m_config->m_impl->construction = std::move(construction);

    return std::forward<ObjectInfo::Builder>(*this);
}

ObjectInfo::Builder&& ObjectInfo::Builder::editorTab(EditorTab editorTab) && {
    m_config->m_impl->editorTab = editorTab;

    return std::forward<ObjectInfo::Builder>(*this);
}

ObjectInfo::Builder&& ObjectInfo::Builder::editorButtonColor(EditorButtonColor color) && {
    m_config->m_impl->editorButtonColor = color;

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

const ObjectConstruction& ObjectInfo::getConstruction() const {
    return m_impl->construction;
}

EditorTab ObjectInfo::getEditorTab() const {
    return m_impl->editorTab;
}

EditorButtonColor ObjectInfo::getEditorButtonColor() const {
    return m_impl->editorButtonColor;
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

CreateMenuItem* ObjectInfo::setupCreateMenuItem() const {
    EditorUI* editorUI = ModEditorUI::getEarly();

    if (!editorUI) return nullptr;

    if (std::optional<uint32_t> id = ObjectAPI::getCustomObjectNumericID(m_impl->id)) {
        return editorUI->getCreateBtn(*id, static_cast<int>(m_impl->editorButtonColor));
    } else {
        return nullptr;
    }
}