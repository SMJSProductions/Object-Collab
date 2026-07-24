#pragma once

#include <dll.hpp>
#include <Geode/Geode.hpp>

template<typename T>
concept GameObjectHasBasicInit = requires(T& object) {
    { object.init() } -> std::same_as<bool>;
};

template<typename T>
concept GameObjectHasFrameInit = requires(T& object, const char* name) {
    { object.init(name) } -> std::same_as<bool>;
};

namespace object_collab {
    using CustomProperties = std::unordered_map<uint32_t, std::string>;
    using ObjectVectors = std::pair<gd::vector<gd::string>, gd::vector<void*>>;

    class OBJC_API_DLL CustomObjectnterface {
        template<typename T> requires std::derived_from<T, GameObject>
        friend class CustomObject;

        struct Impl;

        std::unique_ptr<Impl> m_impl;
    public:
        /// @param objectString The object string with the delimited data.
        /// @returns The vector pair GD uses to initialize objects.
        static geode::Result<ObjectVectors> createObjectVectorsFromString(std::string_view objectString);
    private:
        CustomObjectnterface();
    public:
        virtual ~CustomObjectnterface();
        virtual bool init(const char* frame) = 0;
        virtual void postInit() = 0;
        virtual void internalPostInit() = 0;
        virtual std::vector<std::string> getObjectDetails() = 0;
        virtual GameObject* getGameObject() = 0;
    protected:
        bool isUpdating();
        void toggleUpdating(bool enabled);
    };

    template<typename T> requires std::derived_from<T, GameObject>
    class CustomObject : public CustomObjectnterface, public T {
    public:
        /// Converts a deserialized property to a matjson serialized property and makes a key value pair.
        /// @param key The property key.
        /// @param property The property to serialize.
        /// @returns The key value pair to save.
        template<typename V>
        static std::pair<uint32_t, std::string> toProperty(uint32_t key, const V& property) {
            return { key, matjson::Value(property).dump() };
        }

        /// Converts a matjson serialized property to a deserialized property.
        /// @param key The key with the property to deserialize.
        /// @param properties The properties container.
        /// @returns The deserialized property.
        template<typename V>
        static geode::Result<V> fromProperty(uint32_t key, const CustomProperties& properties) {
            if (const auto& property = properties.find(key); property != properties.end()) {
                return matjson::parseAs<V>(property->second);
            } else {
                return geode::Err("Property not found");
            }
        }

        /// Converts a matjson serialized property to a deserialized property and puts it into the target if present.
        /// @param target The target to put the deserialized property into.
        /// @param key The key with the property to deserialize.
        /// @param properties The properties container.
        template<typename V>
        static void propertyInto(V& target, uint32_t key, const CustomProperties& properties) {
            if (const auto& property = properties.find(key); property != properties.end()) {
                GEODE_UNWRAP_INTO_IF_OK(target, matjson::parseAs<V>(property->second));
            }
        }

        CustomObject& operator=(CustomObject&& other) = default;
        CustomObject& operator=(const CustomObject& other) = delete;

        CustomObject(CustomObject&& other) = default;
        CustomObject(const CustomObject& other) = delete;

        /// @param objectType The type of object, this copies some standard properties of the specified type. Default is Solid.
        CustomObject(GameObjectType objectType = GameObjectType::Solid) {
            this->m_objectType = objectType;
        }

        /// @see GameObject::init
        virtual bool init(const char* frame) override {
            static_assert(GameObjectHasBasicInit<T> || GameObjectHasFrameInit<T>, "Must have either ::init(const char* frame) or ::init()");

            this->m_addToNodeContainer = true;

            if constexpr (GameObjectHasBasicInit<T>) {
                return T::init() && cocos2d::CCSpriteExtra::initWithSpriteFrameName(frame);
            } else {
                return T::init(frame);
            }
        }

        /// Runs after the object has fully generated.
        /// @note It's highly recommended to use this if you want to alter default GameObject properties.
        virtual void postInit() override { }

        /// An internal post init which runs before postInit to setup basic object properties
        void internalPostInit() override {
            this->m_dontIgnoreDuration = this->m_isTrigger = this->isTrigger();

            if (this->m_isTrigger) {
                this->m_baseColor->m_defaultColorID = 0;
                this->m_isInvisible = !this->m_editorEnabled;
            }
        }

        /// Creates a map of custom properties which will be saved in the level string together with the standard properties.
        /// @note Make sure to check https://boomlings.dev/resources/client/level-components/level-string#level-string-data to prevent overlaps!
        /// @returns The map of all custom properties which will be stored in the level string as 'key1,value1,key2,value2;'.
        virtual CustomProperties getCustomProperties() { return {}; }

        /// Initializes the object with its custom properties.
        /// @param properties The map of properties associated with the object.
        virtual void initWithCustomProperties(const CustomProperties& properties) { }

        /// Provides any custom details shown when the object is selected.
        /// @returns A list of custom lines shown, the default implement is none.
        virtual std::vector<std::string> getObjectDetails() override { return {}; }

        /// This will trigger either when activateObject is called for a normal object or triggerObject for a trigger.
        virtual void onAction(GJBaseGameLayer* layer, int uniqueID, const gd::vector<int>* remapKeys) { }

        /// Sets the hitbox of the object based on the size and offset.
        /// @param sizeUnits The amount of units (1 in-game tile) the hitbox size is on both axis.
        /// @param offsetUnits The amount of units (1 in-game tile) the hitbox is offset on both axis.
        virtual void setHitbox(const cocos2d::CCSize& sizeUnits, const cocos2d::CCPoint& offsetUnits = { 0, 0 }) {
            constexpr float TILE_SIZE = 30;

            this->m_width = sizeUnits.width * TILE_SIZE;
            this->m_height = sizeUnits.height * TILE_SIZE;
            this->m_customBoxOffset = offsetUnits * TILE_SIZE;
        }

        /// Sets the round hitbox of the object based on the radius.
        /// @param radiusUnits The amount of units (1 in-game tile) the hitbox radius is.
        virtual void setRoundHitbox(const float radiusUnits) {
            this->setHitbox({ radiusUnits * 2, radiusUnits * 2 });

            this->m_objectRadius = this->m_width / 2;
        }

        /// Sets the hitbox of the object based on the size and offset using the 30 steps per grid system.
        /// @param size The size of the hitbox on both axis.
        /// @param offset The offset of the hitbox on both axis.
        virtual void setRawHitbox(const cocos2d::CCSize& size, const cocos2d::CCPoint& offset = { 0, 0 }) {
            this->m_width = size.width;
            this->m_height = size.height;
            this->m_customBoxOffset = offset;
        }

        /// Sets the round hitbox of the object based on the radius using the 30 steps per grid system.
        /// @param radius The radius of the hitbox.
        virtual void setRawRoundHitbox(const float radius) {
            this->setHitbox({ radius * 2, radius * 2 });

            this->m_objectRadius = radius;
        }

        /// @see CCNode::scheduleUpdate
        virtual void scheduleUpdate() {
            this->toggleUpdating(true);
        }

        /// @see CCNode::unscheduleUpdate
        virtual void unscheduleUpdate() {
            this->toggleUpdating(false);
        }

        /// @see CCNode::visit
        virtual void visit() override {
            if (this->isUpdating()) this->update(cocos2d::CCDirector::get()->getDeltaTime());

            T::visit();
        }

        /// @see GameObject::activateObject
        virtual void activateObject() override {
            if (!this->isTrigger()) this->onAction(GJBaseGameLayer::get(), this->m_uniqueID, {});

            GameObject::activateObject();
        }

        /// @see GameObject::triggerObject
        virtual void triggerObject(GJBaseGameLayer* layer, int uniqueID, const gd::vector<int>* remapKeys) override {
            if (this->isTrigger()) this->onAction(layer, uniqueID, remapKeys);

            GameObject::triggerObject(layer, uniqueID, remapKeys);
        }

        /// @see GameObject::customObjectSetup
        virtual void customObjectSetup(gd::vector<gd::string>& values, gd::vector<void*>& exists) override {
            CustomProperties properties;

            T::customObjectSetup(values, exists);

            for (size_t i = 0; i < values.size() && i < exists.size(); i++) {
                if (exists[i]) {
                    std::string value = values[i];

                    std::ranges::replace(value, 0x1, ',');
                    std::ranges::replace(value, 0x2, ';');

                    properties.emplace(i, std::move(value));
                }
            }

            this->initWithCustomProperties(std::move(properties));
        }

        /// @see GameObject::getSaveString
        virtual gd::string getSaveString(GJBaseGameLayer* layer) override {
            CustomProperties properties = this->getCustomProperties();
            geode::utils::StringBuffer buffer;

            buffer.append(T::getSaveString(layer));

            for (auto& [key, value] : properties) {
                std::ranges::replace(value, ',', 0x1);
                std::ranges::replace(value, ';', 0x2);

                buffer.append(',');
                buffer.append(key);
                buffer.append(',');
                buffer.append(value);
            }

            return buffer.str();
        }

        /// @note This mostly exists for simplified use of the templated class internally
        /// @returns The instance as a game object.
        GameObject* getGameObject() override {
            return this;
        }
    };
}