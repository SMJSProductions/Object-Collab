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
    bool isColorTrigger = false;
    bool isSpawnableTrigger = true;
    bool isStoppableTrigger = true;
    bool isEditorSpawnableTrigger = true;
    bool isSpeedObject = false;
    bool isEditorReserved = false;
    bool omitTrashTexture = false;
    bool shouldLockX = false;
    bool shouldNotHideAnimFreeze = false;
    bool usesFreezeAnimation = false;
    bool usesSpecialAnimation = false;
    OnPlayShineEffect onPlayShineEffect = nullptr;
    OnActionCommand onControlIDCommand = nullptr;
    OnActionCommand onObjectGroupCommand = nullptr;
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

ObjectTraits::Builder&& ObjectTraits::Builder::isColorTrigger(bool toggle) && {
    m_config->m_impl->isColorTrigger = toggle;

    return std::forward<ObjectTraits::Builder>(*this);
}

ObjectTraits::Builder&& ObjectTraits::Builder::isSpawnableTrigger(bool toggle) && {
    m_config->m_impl->isSpawnableTrigger = toggle;

    return std::forward<ObjectTraits::Builder>(*this);
}

ObjectTraits::Builder&& ObjectTraits::Builder::isStoppableTrigger(bool toggle) && {
    m_config->m_impl->isStoppableTrigger = toggle;

    return std::forward<ObjectTraits::Builder>(*this);
}

ObjectTraits::Builder&& ObjectTraits::Builder::isEditorSpawnableTrigger(bool toggle) && {
    m_config->m_impl->isEditorSpawnableTrigger = toggle;

    return std::forward<ObjectTraits::Builder>(*this);
}

ObjectTraits::Builder&& ObjectTraits::Builder::isSpeedObject(bool toggle) && {
    m_config->m_impl->isSpeedObject = toggle;

    return std::forward<ObjectTraits::Builder>(*this);
}

ObjectTraits::Builder&& ObjectTraits::Builder::isEditorReserved(bool toggle) && {
    m_config->m_impl->isEditorReserved = toggle;

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

ObjectTraits::Builder&& ObjectTraits::Builder::onPlayShineEffect(OnPlayShineEffect onPlayShineEffect) && {
    m_config->m_impl->onPlayShineEffect = std::move(onPlayShineEffect);

    return std::forward<ObjectTraits::Builder>(*this);
}

ObjectTraits::Builder&& ObjectTraits::Builder::onControlIDCommand(OnActionCommand onControlIDCommand) && {
    m_config->m_impl->onControlIDCommand = std::move(onControlIDCommand);

    return std::forward<ObjectTraits::Builder>(*this);
}

ObjectTraits::Builder&& ObjectTraits::Builder::onObjectGroupCommand(OnActionCommand onObjectGroupCommand) && {
    m_config->m_impl->onObjectGroupCommand = std::move(onObjectGroupCommand);

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

bool ObjectTraits::ignoreEditorDuration() const {
    return m_impl->ignoreEditorDuration;
}

bool ObjectTraits::isColorTrigger() const {
    return m_impl->isColorTrigger;
}

bool ObjectTraits::isSpawnableTrigger() const {
    return m_impl->isSpawnableTrigger;
}

bool ObjectTraits::isStoppableTrigger() const {
    return m_impl->isStoppableTrigger;
}

bool ObjectTraits::isEditorSpawnableTrigger() const {
    return m_impl->isEditorSpawnableTrigger;
}

bool ObjectTraits::isSpeedObject() const {
    return false && m_impl->isSpeedObject;
}

bool ObjectTraits::isEditorReserved() const {
    return m_impl->isEditorReserved;
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

void ObjectTraits::controlIDCommand(GJActionCommand command) const {
    if (m_impl->onControlIDCommand) {
        m_impl->onControlIDCommand(command);
    }
}

void ObjectTraits::objectGroupCommand(GJActionCommand command) const {
    if (m_impl->onObjectGroupCommand) {
        m_impl->onObjectGroupCommand(command);
    }
}