#include <ObjectTraits.hpp>

using namespace object_collab;

struct ObjectTraits::Impl {
    GameObjectType objectType = GameObjectType::Solid;
    int defaultMainColorID = 1004;
    ZLayer defaultZLayer = ZLayer::Default;
    int defaultZOrder = 2;
    short speedMod = 0;
    bool canRotateFree = false;
    bool ignoreEditorDuration = false;
    bool colorTrigger = false;
    bool spawnableTrigger = true;
    bool stoppableTrigger = true;
    bool editorSpawnableTrigger = true;
    bool speedObject = false;
    bool editorReserved = false;
    bool omitTrashTexture = false;
    bool shouldLockX = false;
    bool shouldNotHideAnimFreeze = false;
    bool usesFreezeAnimation = false;
    bool usesSpecialAnimation = false;
    PlayShineEffect onPlayShineEffect = nullptr;
};

ObjectTraits::Builder::Builder(): m_config(std::unique_ptr<ObjectTraits>(new ObjectTraits())) { }

ObjectTraits::Builder::~Builder() = default;

ObjectTraits::Builder ObjectTraits::builder() {
    return ObjectTraits::Builder();
}

ObjectTraits::Builder&& ObjectTraits::Builder::gameObjectType(GameObjectType objectType) && {
    m_config->m_impl->objectType = objectType;

    return std::forward<ObjectTraits::Builder>(*this);
}

ObjectTraits::Builder&& ObjectTraits::Builder::defaultMainColorID(int defaultMainColorID) && {
    m_config->m_impl->defaultMainColorID = defaultMainColorID;

    return std::forward<ObjectTraits::Builder>(*this);
}

ObjectTraits::Builder&& ObjectTraits::Builder::defaultZLayer(ZLayer defaultZLayer) && {
    m_config->m_impl->defaultZLayer = defaultZLayer;

    return std::forward<ObjectTraits::Builder>(*this);
}

ObjectTraits::Builder&& ObjectTraits::Builder::defaultZOrder(int defaultZOrder) && {
    m_config->m_impl->defaultZOrder = defaultZOrder;

    return std::forward<ObjectTraits::Builder>(*this);
}

ObjectTraits::Builder&& ObjectTraits::Builder::speedMod(short speedMod) && {
    m_config->m_impl->speedMod = speedMod;

    return std::forward<ObjectTraits::Builder>(*this);
}

ObjectTraits::Builder&& ObjectTraits::Builder::ignoreEditorDuration(bool toggle) && {
    m_config->m_impl->ignoreEditorDuration = toggle;

    return std::forward<ObjectTraits::Builder>(*this);
}

ObjectTraits::Builder&& ObjectTraits::Builder::colorTrigger(bool toggle) && {
    m_config->m_impl->colorTrigger = toggle;

    return std::forward<ObjectTraits::Builder>(*this);
}

ObjectTraits::Builder&& ObjectTraits::Builder::spawnableTrigger(bool toggle) && {
    m_config->m_impl->spawnableTrigger = toggle;

    return std::forward<ObjectTraits::Builder>(*this);
}

ObjectTraits::Builder&& ObjectTraits::Builder::stoppableTrigger(bool toggle) && {
    m_config->m_impl->stoppableTrigger = toggle;

    return std::forward<ObjectTraits::Builder>(*this);
}

ObjectTraits::Builder&& ObjectTraits::Builder::editorSpawnableTrigger(bool toggle) && {
    m_config->m_impl->editorSpawnableTrigger = toggle;

    return std::forward<ObjectTraits::Builder>(*this);
}

ObjectTraits::Builder&& ObjectTraits::Builder::speedObject(bool toggle) && {
    m_config->m_impl->speedObject = toggle;

    return std::forward<ObjectTraits::Builder>(*this);
}

ObjectTraits::Builder&& ObjectTraits::Builder::editorReserved(bool toggle) && {
    m_config->m_impl->editorReserved = toggle;

    return std::forward<ObjectTraits::Builder>(*this);
}

ObjectTraits::Builder&& ObjectTraits::Builder::omitTrashTexture(bool toggle) && {
    m_config->m_impl->omitTrashTexture = toggle;

    return std::forward<ObjectTraits::Builder>(*this);
}

ObjectTraits::Builder&& ObjectTraits::Builder::shouldLockX(bool toggle) && {
    m_config->m_impl->shouldLockX = toggle;

    return std::forward<ObjectTraits::Builder>(*this);
}

ObjectTraits::Builder&& ObjectTraits::Builder::shouldNotHideAnimFreeze(bool toggle) && {
    m_config->m_impl->shouldNotHideAnimFreeze = toggle;

    return std::forward<ObjectTraits::Builder>(*this);
}

ObjectTraits::Builder&& ObjectTraits::Builder::usesFreezeAnimation(bool toggle) && {
    m_config->m_impl->usesFreezeAnimation = toggle;

    return std::forward<ObjectTraits::Builder>(*this);
}

ObjectTraits::Builder&& ObjectTraits::Builder::usesSpecialAnimation(bool toggle) && {
    m_config->m_impl->usesSpecialAnimation = toggle;

    return std::forward<ObjectTraits::Builder>(*this);
}

ObjectTraits ObjectTraits::Builder::build() && {
    return std::move(*m_config);
}

ObjectTraits& ObjectTraits::operator=(ObjectTraits&& other) noexcept = default;

ObjectTraits::ObjectTraits(ObjectTraits&& other) noexcept = default;

ObjectTraits::ObjectTraits(): m_impl(std::make_unique<Impl>()) { }

ObjectTraits::~ObjectTraits() = default;

GameObjectType ObjectTraits::getGameObjectType() const {
    return m_impl->objectType;
}

int ObjectTraits::getDefaultMainColorID() const {
    return m_impl->defaultMainColorID;
}

ZLayer ObjectTraits::getDefaultZLayer() const {
    return m_impl->defaultZLayer;
}

int ObjectTraits::getDefaultZOrder() const {
    return m_impl->defaultZOrder;
}

short ObjectTraits::getSpeedMod() const {
    return m_impl->speedMod;
}

bool ObjectTraits::canRotateFree() const {
    return m_impl->canRotateFree;
}

bool ObjectTraits::isIgnoreEditorDuration() const {
    return m_impl->ignoreEditorDuration;
}

bool ObjectTraits::isColorTrigger() const {
    return m_impl->colorTrigger;
}

bool ObjectTraits::isSpawnableTrigger() const {
    return m_impl->spawnableTrigger;
}

bool ObjectTraits::isStoppableTrigger() const {
    return m_impl->stoppableTrigger;
}

bool ObjectTraits::isEditorSpawnableTrigger() const {
    return m_impl->editorSpawnableTrigger;
}

bool ObjectTraits::isSpeedObject() const {
    return m_impl->speedObject;
}

bool ObjectTraits::isEditorReserved() const {
    return m_impl->editorReserved;
}

bool ObjectTraits::omitTrashTexture() const {
    return m_impl->omitTrashTexture;
}

bool ObjectTraits::shouldLockX() const {
    return m_impl->shouldLockX;
}

bool ObjectTraits::shouldNotHideAnimFreeze() const {
    return m_impl->shouldNotHideAnimFreeze;
}

bool ObjectTraits::usesFreezeAnimation() const {
    return m_impl->usesFreezeAnimation;
}

bool ObjectTraits::usesSpecialAnimation() const {
    return m_impl->usesSpecialAnimation;
}

void ObjectTraits::playShineEffect(geode::Function<void()> original) const {
    if (m_impl->onPlayShineEffect) {
        m_impl->onPlayShineEffect(std::move(original));
    } else {
        original();
    }
}