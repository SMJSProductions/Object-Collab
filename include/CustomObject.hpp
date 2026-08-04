#pragma once

#include "dll.hpp"
#include "Property.hpp"

template<typename T>
concept GameObjectHasBasicInit = requires(T& object) {
    { object.init() } -> std::same_as<bool>;
};

template<typename T>
concept GameObjectHasFrameInit = requires(T& object, const char* name) {
    { object.init(name) } -> std::same_as<bool>;
};

namespace object_collab {
    class ObjectInfo;

    /// The pair of vectors GD uses to configure the properties. The void* vector will be nullptr to present not present and has a pointer when present.
    using ObjectVectors = std::pair<gd::vector<gd::string>, gd::vector<void*>>;

    class OBJC_API_DLL CustomObjectInterface {
        template<typename T> requires std::derived_from<T, GameObject>
        friend class CustomObject;

        struct Impl;

        std::unique_ptr<Impl> m_impl;

        [[nodiscard]] static std::vector<std::string_view> split(const std::string_view string, const char delimiter);
    public:
        /// @param objectString The object string with the delimited data.
        /// @returns The vector pair GD uses to initialize objects.
        [[nodiscard]] static geode::Result<ObjectVectors> createObjectVectorsFromString(std::string_view objectString);

        CustomObjectInterface& operator=(CustomObjectInterface&& other) noexcept;
        CustomObjectInterface& operator=(const CustomObjectInterface& other) noexcept = delete;

        CustomObjectInterface(CustomObjectInterface&& other) noexcept;
        CustomObjectInterface(const CustomObjectInterface& other) noexcept = delete;
    protected:
        CustomObjectInterface(ObjectInfo* info);
    public:
        virtual ~CustomObjectInterface();
        [[nodiscard]] virtual bool init(const char* frame) = 0;
        virtual void postInit() = 0;
        virtual void postEditorInit() = 0;
        virtual void collidedByPlayer(PlayerObject* player) = 0;
        virtual bool updateProperty(size_t property, std::string_view value) = 0;
        [[nodiscard]] virtual std::vector<std::string> getObjectDetails() = 0;
        [[nodiscard]] virtual GameObject* getGameObject() = 0;
        [[nodiscard]] virtual bool isColorTrigger() = 0;
        [[nodiscard]] virtual bool isSettingsObject() = 0;
        [[nodiscard]] virtual bool isSpawnableTrigger() = 0;
        [[nodiscard]] virtual bool isSpecialObject() = 0;
        [[nodiscard]] virtual bool isTrigger() = 0;
        [[nodiscard]] virtual bool shouldLockX() = 0;
        [[nodiscard]] virtual bool shouldNotHideAnimFreeze() = 0;
        [[nodiscard]] virtual bool usesFreezeAnimation() = 0;
        [[nodiscard]] virtual bool usesSpecialAnimation() = 0;
        [[nodiscard]] const CustomProperties& getCustomProperties();
    private:
        std::optional<size_t> getTriggerTextProperty();
        void setTriggerTextProperty(std::optional<size_t> property);
        const cocos2d::CCPoint& getTriggerTextPropertyOffset();
        void setTriggerTextPropertyOffset(cocos2d::CCPoint offset);
        float getTriggerTextPropertyScale();
        void setTriggerTextPropertyScale(float scale);
    };

    template<typename T> requires std::derived_from<T, GameObject>
    class CustomObject : public CustomObjectInterface, public T {
    public:
        static constexpr float TILE_SIZE = 30;

        CustomObject& operator=(CustomObject&& other) noexcept = default;
        CustomObject& operator=(const CustomObject& other) noexcept = delete;

        CustomObject(CustomObject&& other) noexcept = default;
        CustomObject(const CustomObject& other) noexcept = delete;
        /// @note Make sure to check https://flowvix.github.io/gd-info-explorer/props to prevent custom property overlaps!
        /// @param info The object info of this object to populate properties.
        /// @param objectType The type of object, this copies some standard properties of the specified type.
        /// @param defaultZLayer The default z layer given when the object is created.
        /// @param defaultZOrder The default z order given when the object is created.
        CustomObject(ObjectInfo* info, GameObjectType objectType = GameObjectType::Solid, ZLayer defaultZLayer = ZLayer::Default, int defaultZOrder = 2): CustomObjectInterface(info) {
            if (objectType == GameObjectType::SecretCoin || objectType == GameObjectType::UserCoin) {
                geode::log::error("Force swapped the object type to prevent anti cheat triggering.");

                this->m_objectType = GameObjectType::Collectible;
            } else {
                this->m_objectType = objectType;
            }

            this->m_defaultZLayer = defaultZLayer;
            this->m_defaultZOrder = defaultZOrder;
        }

        /// @see GameObject::init
        [[nodiscard]] virtual bool init(const char* frame) override {
            static_assert(GameObjectHasBasicInit<T> || GameObjectHasFrameInit<T>, "Must have either ::init(const char* frame) or ::init()");

            const int defaultZOrder = this->m_defaultZOrder;
            this->m_addToNodeContainer = true;

            if constexpr (GameObjectHasBasicInit<T>) {
                if (!T::init() || !cocos2d::CCSpriteExtra::initWithSpriteFrameName(frame)) return false;
            } else {
                if (!T::init(frame)) return false;
            }

            this->m_defaultZOrder = defaultZOrder;

            return true;
        }

        /// @see GameObject::firstSetup
        virtual void firstSetup() override {
            for (auto& [_, property] : this->getCustomProperties()) {
                property->applyDefault(this);
            }
        }

        /// @see GameObject::customSetup
        virtual void customSetup() override {
            T::customSetup();

            GameObject::setDefaultMainColorMode(this->getDefaultMainColorID());

            this->m_dontIgnoreDuration = !this->ignoreEditorDuration();
            this->m_activateTriggerInEditor = this->isEditorSpawnableTrigger();
            this->m_canBeControlled = this->isStoppableTrigger();
            this->m_isInvisible = this->isTrigger() && !this->m_editorEnabled;
        }

        /// Runs after the object has fully generated.
        /// @note It's highly recommended to use this if you want to alter default GameObject properties.
        virtual void postInit() override { }

        /// Runs after the object has fully generated in the editor.
        /// @note It's highly recommended to use this if you want to alter default GameObject properties in the editor.
        virtual void postEditorInit() override { }

        /// Provides any custom details shown when the object is selected.
        /// @returns A list of custom lines shown, the default implement is none.
        [[nodiscard]] virtual std::vector<std::string> getObjectDetails() override { return {}; }

        /// Gets the detail sprite of the object.
        [[nodiscard]] cocos2d::CCSprite* getColorSprite() {
            return this->m_colorSprite;
        }

        /// Gets the glow sprite of the object.
        [[nodiscard]] cocos2d::CCSprite* getGlowSprite() {
            return this->m_glowSprite;
        }

        /// Gets the text shown on a trigger.
        /// @see GameObject::getObjectLabel
        [[nodiscard]] cocos2d::CCLabelBMFont* getTriggerText() requires std::derived_from<T, EffectGameObject> {
            return T::getObjectLabel();
        }

        /// Sets the detail sprite of the object.
        /// @param frame The sprite frame name to use.
        /// @param defaultColorID The default color ID given to the detail sprite.
        void setDetailSprite(geode::ZStringView frame, int defaultColorID = 1) {
            this->addCustomColorChild(frame.c_str());
            this->setDefaultSecondaryColorMode(defaultColorID);
        }

        /// Sets the glow sprite of the object.
        /// @param frame The sprite frame name to use.
        /// @param color The optional color, if set it will make the glow considered custom.
        void setGlowSprite(geode::ZStringView frame, const std::optional<cocos2d::ccColor4B>& color = std::nullopt) {
            this->createGlow(std::move(frame));
            this->addCustomColorChild(frame);

            if (color) {
                this->m_customGlowColor = true;
                this->m_cantColorGlow = false;

                this->setGlowOpacity(color->a);
                this->setGlowColor({ color->r, color->g, color->b });
            } else {
                this->m_customGlowColor = false;
                this->m_cantColorGlow = true;
            }
        }

        /// Sets the shown on triggers based on the value of a property.
        /// @warning This method is only functional when the custom object is templated with EffectGameObject or an inheritor of!
        /// @param property The property ID to use.
        /// @param offset The offset of the label.
        /// @param scale The scale of the label.
        void setTriggerTextProperty(size_t property, cocos2d::CCPoint offset = { 0, 0 }, float scale = 0.5f) requires std::derived_from<T, EffectGameObject> {
            CustomObjectInterface::setTriggerTextProperty(property);
            this->setTriggerTextPropertyOffset(std::move(offset));
            this->setTriggerTextPropertyScale(scale);
            this->updateTriggerText();
        }

        /// Removes the detail sprite.
        /// @see GameObject::removeColorSprite
        void removeDetailSprite() {
            this->removeColorSprite();
        }

        /// Removes the glow sprite.
        /// @see GameObject::removeGlow
        void removeGlowSprite() {
            this->removeGlow();
        }

        /// Removes the text shown on a trigger.
        void removeTriggerTextProperty() requires std::derived_from<T, EffectGameObject> {
            CustomObjectInterface::setTriggerTextProperty(std::nullopt);
            this->updateTriggerText();
        }

        /// Sets the hitbox of the object based on the size and offset.
        /// @param sizeUnits The amount of units (1 in-game tile) the hitbox size is on both axis.
        /// @param offsetUnits The amount of units (1 in-game tile) the hitbox is offset on both axis.
        void setHitbox(const cocos2d::CCSize& sizeUnits, const cocos2d::CCPoint& offsetUnits = { 0, 0 }) {
            this->m_width = sizeUnits.width * CustomObject::TILE_SIZE;
            this->m_height = sizeUnits.height * CustomObject::TILE_SIZE;
            this->m_customBoxOffset = offsetUnits * CustomObject::TILE_SIZE;
        }

        /// Sets the round hitbox of the object based on the radius.
        /// @param radiusUnits The amount of units (1 in-game tile) the hitbox radius is.
        void setRoundHitbox(float radiusUnits) {
            this->setHitbox({ radiusUnits * 2, radiusUnits * 2 });

            this->m_objectRadius = this->m_width / 2;
        }

        /// Sets the hitbox of the object based on the size and offset using the 30 steps per grid system.
        /// @param size The size of the hitbox on both axis.
        /// @param offset The offset of the hitbox on both axis.
        void setRawHitbox(const cocos2d::CCSize& size, const cocos2d::CCPoint& offset = { 0, 0 }) {
            this->m_width = size.width;
            this->m_height = size.height;
            this->m_customBoxOffset = offset;
        }

        /// Sets the round hitbox of the object based on the radius using the 30 steps per grid system.
        /// @param radius The radius of the hitbox.
        void setRawRoundHitbox(float radius) {
            this->setHitbox({ radius * 2, radius * 2 });

            this->m_objectRadius = radius;
        }

        /// Called when the object was collided by the given player.
        /// @warning This gets called every step the player is in contact with the object.
        /// @param player The player who collided with the object.
        virtual void collidedByPlayer(PlayerObject* player) override { }

        /// Called when the object was activated by the given player.
        /// @note This method is not called by InverseMirrorPortal, NormalMirrorPortal, Modifier, EnterEffectObject, DualPortal, SoloPortal, SecretCoin, Collectible & UserCoin.
        /// @warning This method is only called when the custom object is templated with EffectGameObject or an inheritor of!
        /// @see GameObject::activatedByPlayer
        /// @param player The player who triggered the object.
        virtual void activatedByPlayer(PlayerObject* player) override {
            T::activatedByPlayer(player);
        }

        /// Called when the object was collided by a player.
        /// @note This method is called on InverseMirrorPortal, NormalMirrorPortal, Modifier, EnterEffectObject, DualPortal, SoloPortal, SecretCoin, Collectible & UserCoin.
        /// @note This method ignores if the object has already been triggered. It will always be called on collision.
        /// @warning This method is only called when the custom object is templated with EffectGameObject or an inheritor of!
        /// @see GameObject::triggerActivated
        /// @param spawnXPosition From where the object was triggered. If by a player it will be 0.
        virtual void triggerActivated(float spawnXPosition) override {
            T::triggerActivated(spawnXPosition);
        }

        /// Called when any trigger condition is met for a trigger object.
        /// @note This method is called on Modifier, EnterEffectObject, SecretCoin, Collectible & UserCoin.
        /// @warning This method is only called when the custom object is templated with EffectGameObject or an inheritor of!
        /// @see GameObject::triggerObject
        /// @param layer The base game layer this object belongs to.
        /// @param uniqueID The unique ID of the trigger which spawned this trigger or this->m_uniqueID if triggered by the player.
        /// @param remapKeys The target group IDs which have to be swapped upon triggering. This is a nullptr if triggered by the player.
        virtual void triggerObject(GJBaseGameLayer* layer, int uniqueID, const gd::vector<int>* remapKeys) override {
            T::triggerObject(layer, uniqueID, remapKeys);
        }

        /// A replacement for CCNode::update().
        /// @see GameObject::activateObject
        virtual void activateObject() override {
            T::activateObject();
        }

        /// Gets the default main color ID given when the object is created.
        /// @note Returning 0 will deactivate colors.
        [[nodiscard]] virtual int getDefaultMainColorID() {
            return this->isTrigger() ? 0 : 1004;
        }

        /// If the object can be rotated without 90deg snapping.
        /// @warning This feature is currently unimplemented due to too much inlining!
        /// @see GameObject::canRotateFree
        [[nodiscard]] virtual bool canRotateFree() {
            return T::canRotateFree();
        }

        /// If the trigger duration handling should be removed.
        /// @see GameObject::ignoreEditorDuration
        [[nodiscard]] virtual bool ignoreEditorDuration() {
            return !this->isTrigger();
        }

        /// If the trigger can affect color channels.
        /// @see GameObject::isColorTrigger
        [[nodiscard]] virtual bool isColorTrigger() override {
            return false;
        }

        /// If the trigger should be simulated in the editor.
        /// @see GameObject::isEditorSpawnableTrigger
        [[nodiscard]] virtual bool isEditorSpawnableTrigger() {
            return this->isTrigger();
        }

        /// If the play layer should ignore this object as its reserved for the editor.
        /// @see GameObject::isSettingsObject
        [[nodiscard]] virtual bool isSettingsObject() override {
            return false;
        }

        /// If the trigger can be spawned.
        /// @see GameObject::isSpawnableTrigger
        [[nodiscard]] virtual bool isSpawnableTrigger() override {
            return this->isTrigger();
        }

        /// If the object should be omitted from rendering in the custom delete & delete all buttons.
        /// @see GameObject::isSpecialObject
        [[nodiscard]] virtual bool isSpecialObject() override {
            return false;
        }

        /// If the object can change the gameplay speed.
        /// @warning This feature is currently unimplemented due to too much inlining!
        /// @see GameObject::isSpeedObject
        [[nodiscard]] virtual bool isSpeedObject() {
            return false;
        }

        /// If the trigger can be manipulated by a stop trigger.
        /// @see GameObject::isStoppableTrigger
        [[nodiscard]] virtual bool isStoppableTrigger() {
            return this->isTrigger();
        }

        /// If the object should be considered a trigger.
        /// @see GameObject::isTrigger
        [[nodiscard]] bool isTrigger() override {
            return this->m_classType == GameObjectClassType::Effect && this->m_objectType == GameObjectType::Modifier;
        }

        /// If the object can be affected by move triggers on the X axis.
        /// @see GameObject::shouldLockX
        [[nodiscard]] virtual bool shouldLockX() override {
            return false;
        }

        /// If the object should end the animation dirty.
        /// @see GameObject::shouldNotHideAnimFreeze
        [[nodiscard]] virtual bool shouldNotHideAnimFreeze() override {
            return false;
        }

        /// If the object uses an animation with a delayed start.
        /// @see GameObject::usesFreezeAnimation
        [[nodiscard]] virtual bool usesFreezeAnimation() override {
            return false;
        }

        /// If the object is animated.
        /// @see GameObject::usesSpecialAnimation
        [[nodiscard]] virtual bool usesSpecialAnimation() override {
            return false;
        }

        /// Initializes the object with the custom variables inside a (really strange) vector/map structure.
        /// @see GameObject::customObjectSetup
        /// @param values The values part of the properties map.
        /// @param exists The key existence part of the properties map.
        virtual void customObjectSetup(gd::vector<gd::string>& values, gd::vector<void*>& exists) override {
            const CustomProperties& properties = this->getCustomProperties();
            std::unordered_map<size_t, std::string> foundProperties;

            T::customObjectSetup(values, exists);

            for (size_t i = 0; i < values.size() && i < exists.size(); i++) {
                if (exists[i] && properties.contains(i)) {
                    std::string value = values[i];

                    std::ranges::replace(value, 0x1, ',');
                    std::ranges::replace(value, 0x2, ';');

                    foundProperties.emplace(i, std::move(value));
                }
            }

            for (auto& [key, property] : properties) {
                if (auto entry = foundProperties.find(key); entry == foundProperties.end()) {
                    property->applyDefault(this);
                } else {
                    property->applyFromString(this, entry->second);
                }
            }
        }

        /// Gets the raw save string of the object.
        /// @see GameObject::getSaveString
        [[nodiscard]] virtual gd::string getSaveString(GJBaseGameLayer* layer) override {
            const CustomProperties& customProperties = this->getCustomProperties();
            const gd::string saveString = T::getSaveString(layer);
            const std::vector<std::string_view> properties = CustomObjectInterface::split(saveString, ',');
            geode::utils::StringBuffer buffer;

            for (size_t i = 0; i < properties.size(); i += 2) {
                if (geode::Result<size_t> key = geode::utils::numFromString<size_t>(properties[i]); key && !customProperties.contains(std::move(key).unwrap())) {
                    buffer.append(properties[i]);
                    buffer.append(',');
                    buffer.append(properties[i + 1]);
                    buffer.append(',');
                }
            }

            for (auto& [key, property] : this->getCustomProperties()) {
                if (!property->isDefault(this)) {
                    std::string value = property->getStringValue(this);

                    std::ranges::replace(value, ',', 0x1);
                    std::ranges::replace(value, ';', 0x2);

                    buffer.append(key);
                    buffer.append(',');
                    buffer.append(value);
                    buffer.append(',');
                }
            }

            std::string result = buffer.str();

            result.pop_back();

            return result;
        }

        /// Updates a property with a stringified value & updates the trigger text if applicable.
        /// @param property The property ID to use.
        /// @param value The stringified value to assign.
        bool updateProperty(size_t property, std::string_view value) override {
            const CustomProperties& customProperties = this->getCustomProperties();

            if (auto entry = customProperties.find(property); entry == customProperties.end()) {
                return false;
            } else {
                entry->second->applyFromString(this, value);

                if constexpr (std::derived_from<T, EffectGameObject>) {
                    if (property == this->getTriggerTextProperty()) this->updateTriggerText();
                }

                return true;
            }
        }

        /// Updates the trigger text with the new property value.
        /// @warning This method is only functional when the custom object is templated with EffectGameObject or an inheritor of!
        void updateTriggerText() requires std::derived_from<T, EffectGameObject> {
            const CustomProperties& properties = this->getCustomProperties();
            std::optional<size_t> property = this->getTriggerTextProperty();
            cocos2d::CCLabelBMFont* label = this->getObjectLabel();

            if (!property || !properties.contains(*property)) {
                label->removeFromParent();
                this->setObjectLabel(nullptr);

                return;
            }

            if (label) {
                label->setString(properties.at(*property)->getStringValue(this).c_str());
            } else {
                label = cocos2d::CCLabelBMFont::create(properties.at(*property)->getStringValue(this).c_str(), "bigFont.fnt");

                this->addChild(label, 1);
                this->setObjectLabel(label);
            }

            label->setScale(this->getTriggerTextPropertyScale());
            label->setPosition(this->getContentSize() * 0.5f + this->getTriggerTextPropertyOffset());
            label->limitLabelWidth(30, 0.5f, 0);
        }

        /// @note This exists for simplified use of the templated class internally.
        /// @returns The instance as a game object.
        [[nodiscard]] inline GameObject* getGameObject() override {
            return this;
        }
    };

    #define $object(name, inheritance) name : public object_collab::CustomObject<inheritance>
}