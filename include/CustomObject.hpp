#pragma once

#include <dll.hpp>
#include <Geode/Geode.hpp>

namespace object_collab {
    using CustomProperties = std::unordered_map<uint32_t, std::string>;
    using ObjectVectors = std::pair<gd::vector<gd::string>, gd::vector<void*>>;

    class OBJC_API_DLL CustomObject : public EffectGameObject {
        struct Impl;

        std::unique_ptr<Impl> m_impl;
    public:
        /// @param objectString The object string with the delimited data.
        /// @returns The vector pair GD uses to initialize objects.
        static geode::Result<ObjectVectors> createObjectVectorsFromString(std::string_view objectString);

        /// Converts a deserialized property to a matjson serialized property and makes a key value pair.
        /// @param key The property key.
        /// @param property The property to serialize.
        /// @returns The key value pair to save.
        template<typename T>
        static std::pair<uint32_t, std::string> toProperty(uint32_t key, const T& property) {
            return { key, matjson::Value(property).dump() };
        }

        /// Converts a matjson serialized property to a deserialized property.
        /// @param key The key with the property to deserialize.
        /// @param properties The properties container.
        /// @returns The deserialized property.
        template<typename T>
        static geode::Result<T> fromProperty(uint32_t key, const CustomProperties& properties) {
            if (const auto& property = properties.find(key); property != properties.end()) {
                return matjson::parseAs<T>(property->second);
            } else {
                return geode::Err("Property not found");
            }
        }

        /// Converts a matjson serialized property to a deserialized property and puts it into the target if present.
        /// @param target The target to put the deserialized property into.
        /// @param key The key with the property to deserialize.
        /// @param properties The properties container.
        template<typename T>
        static void propertyInto(T& target, uint32_t key, const CustomProperties& properties) {
            if (const auto& property = properties.find(key); property != properties.end()) {
                GEODE_UNWRAP_INTO_IF_OK(target, matjson::parseAs<T>(property->second));
            }
        }

        CustomObject& operator=(CustomObject&& other);
        CustomObject& operator=(const CustomObject& other) = delete;

        CustomObject(CustomObject&& other);
        CustomObject(const CustomObject& other) = delete;
        /// @param objectType The type of object, this copies some standard properties of the specified type. Default is Solid.
        CustomObject(GameObjectType objectType = GameObjectType::Solid);
        ~CustomObject();

        /// Provides any custom details shown when the object is selected.
        /// @returns A list of custom lines shown, the default implement is none.
        virtual std::vector<std::string> getObjectDetails();
        /// Creates a map of custom properties which will be saved in the level string together with the standard properties.
        /// @note Make sure to check https://boomlings.dev/resources/client/level-components/level-string#level-string-data to prevent overlaps!
        /// @returns The map of all custom properties which will be stored in the level string as 'key1,value1,key2,value2;'.
        virtual CustomProperties getCustomProperties();
        /// Initializes the object with its custom properties.
        /// @param properties The map of properties associated with the object.
        virtual void initWithCustomProperties(const CustomProperties& properties);
        /// Runs after the object has fully generated.
        /// @note It's highly recommended to use this if you want to alter default GameObject properties.
        virtual void postInit();
        /// An internal post init which runs before postInit to setup basic object properties
        void internalPostInit();
        /// This will trigger either when activateObject is called for a normal object or triggerObject for a trigger.
        virtual void onAction();
        /// Sets the hitbox of the object based on the size and offset.
        /// @param sizeUnits The amount of units (1 in-game tile) the hitbox size is on both axis.
        /// @param offsetUnits The amount of units (1 in-game tile) the hitbox is offset on both axis.
        virtual void setHitbox(const cocos2d::CCSize& sizeUnits, const cocos2d::CCPoint& offsetUnits = { 0, 0 });
        /// Sets the round hitbox of the object based on the radius.
        /// @param radiusUnits The amount of units (1 in-game tile) the hitbox radius is.
        virtual void setRoundHitbox(const float radiusUnits);
        /// Sets the hitbox of the object based on the size and offset using the 30 steps per grid system.
        /// @param size The size of the hitbox on both axis.
        /// @param offset The offset of the hitbox on both axis.
        virtual void setRawHitbox(const cocos2d::CCSize& size, const cocos2d::CCPoint& offset = { 0, 0 });
        /// Sets the round hitbox of the object based on the radius using the 30 steps per grid system.
        /// @param radius The radius of the hitbox.
        virtual void setRawRoundHitbox(const float radius);

        virtual void scheduleUpdate();
        virtual void unscheduleUpdate();
        virtual void visit() override;
        virtual void activateObject() override;
        virtual void triggerObject(GJBaseGameLayer* layer, int uniqueID, const gd::vector<int>* remapKeys) override;
        virtual void customObjectSetup(gd::vector<gd::string>& values, gd::vector<void*>& exists) override;
        virtual gd::string getSaveString(GJBaseGameLayer* layer) override;
    };
}