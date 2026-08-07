#pragma once

#include <Geode/Geode.hpp>
#include "dll.hpp"

namespace object_collab {
    /// @param original A callback to the original method.
    using PlayShineEffect = geode::Function<void(geode::Function<void()> original)>;

    class OBJC_API_DLL ObjectTraits {
        struct Impl;

        std::unique_ptr<Impl> m_impl;
    public:
        class OBJC_API_DLL Builder {
            friend class ObjectTraits;

            std::unique_ptr<ObjectTraits> m_config;

            Builder();
        public:
            ~Builder();
            /// @note Default is GameObjectType::Solid.
            /// @param gameObjectType The type of object, this copies some standard properties of the specified object type.
            [[nodiscard]] Builder&& gameObjectType(GameObjectType objectType) &&;
            /// @note Default is 1004.
            /// @note Returning 0 will deactivate colors.
            /// @param defaultMainColorID Gets the default main color ID given when the object is created.
            [[nodiscard]] Builder&& defaultMainColorID(int defaultMainColorID) &&;
            /// @note Default is ZLayer::Default.
            /// @param defaultZLayer The default z layer given when the object is created.
            [[nodiscard]] Builder&& defaultZLayer(ZLayer defaultZLayer) &&;
            /// @note Default is 2.
            /// @param defaultZOrder The default z order given when the object is created.
            [[nodiscard]] Builder&& defaultZOrder(int defaultZOrder) &&;
            /// @note Default is 0.
            /// @warning This only works when the custom object is templated with EffectGameObject or an inheritor of, is a object type of Modifier and returns isSpeedObject to true.
            /// @warning This feature is currently unimplemented due to too much inlining.
            /// @see EffectGameObject::updateSpeedModType
            /// @param speedMod Gets the speed mod of the object to apply to the gameplay.
            [[nodiscard]] Builder&& speedMod(short speedMod) &&;
            /// @note Default is false.
            /// @warning This feature is currently unimplemented due to too much inlining.
            /// @see GameObject::canRotateFree
            /// @param toggle If the object can be rotated without 90deg snapping.
            [[nodiscard]] Builder&& canRotateFree(bool toggle) &&;
            /// @note Default is false.
            /// @warning This only works when the custom object is templated with EffectGameObject or an inheritor of and is a object type of Modifier.
            /// @see GameObject::ignoreEditorDuration
            /// @param toggle If the trigger duration handling should be removed.
            [[nodiscard]] Builder&& ignoreEditorDuration(bool toggle) &&;
            /// @note Default is false.
            /// @warning This only works when the custom object is templated with EffectGameObject or an inheritor of and is a object type of Modifier.
            /// @see GameObject::isColorTrigger
            /// @param toggle If the trigger can affect color channels.
            [[nodiscard]] Builder&& isColorTrigger(bool toggle) &&;
            /// @note Default is true.
            /// @warning This only works when the custom object is templated with EffectGameObject or an inheritor of and is a object type of Modifier.
            /// @see GameObject::isSpawnableTrigger
            /// @param toggle If the trigger can be spawned.
            [[nodiscard]] Builder&& isSpawnableTrigger(bool toggle) &&;
            /// @note Default is true.
            /// @warning This only works when the custom object is templated with EffectGameObject or an inheritor of and is a object type of Modifier.
            /// @see GameObject::isStoppableTrigger
            /// @param toggle If the trigger can be manipulated by a stop trigger.
            [[nodiscard]] Builder&& isStoppableTrigger(bool toggle) &&;
            /// @note Default is true.
            /// @warning This only works when the custom object is templated with EffectGameObject or an inheritor of and is a object type of Modifier.
            /// @see GameObject::isEditorSpawnableTrigger
            /// @param toggle If the trigger should be simulated in the editor.
            [[nodiscard]] Builder&& isEditorSpawnableTrigger(bool toggle) &&;
            /// @note Default is false.
            /// @warning This only works when the custom object is templated with EffectGameObject or an inheritor of and is a object type of Modifier.
            /// @warning This feature is currently unimplemented due to too much inlining.
            /// @see GameObject::isSpeedObject
            //// @param toggle If the object can change the gameplay speed.
            [[nodiscard]] Builder&& isSpeedObject(bool toggle) &&;
            /// @note Default is false.
            /// @see GameObject::isSettingsObject
            /// @param toggle If the play layer should ignore this object as its reserved for the editor.
            [[nodiscard]] Builder&& isEditorReserved(bool toggle) &&;
            /// @note Default is false.
            /// @see GameObject::isSpecialObject
            /// @param toggle If the object should be omitted from rendering in the custom delete & delete all buttons.
            [[nodiscard]] Builder&& omitTrashTexture(bool toggle) &&;
            /// @note Default is false.
            /// @see GameObject::shouldLockX
            /// @param toggle If the object can be affected by move triggers on the X axis.
            [[nodiscard]] Builder&& shouldLockX(bool toggle) &&;
            /// @note Default is false.
            /// @warning This only works when the custom object is templated with EnhancedGameObject or an inheritor of.
            /// @see GameObject::shouldNotHideAnimFreeze
            /// @param toggle If the object should disable once the animation freezes.
            [[nodiscard]] Builder&& shouldNotHideAnimFreeze(bool toggle) &&;
            /// @note Default is false.
            /// @warning This only works when the custom object is templated with EnhancedGameObject or an inheritor of.
            /// @see GameObject::usesFreezeAnimation
            /// @param toggle If the object uses an animation with a delayed start.
            [[nodiscard]] Builder&& usesFreezeAnimation(bool toggle) &&;
            /// @note Default is false.
            /// @warning This only works when the custom object is templated with EnhancedGameObject or an inheritor of.
            /// @see GameObject::usesSpecialAnimation
            /// @param toggle If the object is animated.
            [[nodiscard]] Builder&& usesSpecialAnimation(bool toggle) &&;
            /// @see GameObject::playShineEffect
            /// @param onPlayShineEffect Plays an effect on activation of the object if the GameObjectType supports it.
            [[nodiscard]] Builder&& onPlayShineEffect(PlayShineEffect onPlayShineEffect) &&;
            [[nodiscard]] ObjectTraits build() &&;

        };

        [[nodiscard]] static Builder builder();

        ObjectTraits& operator=(ObjectTraits&& other) noexcept;
        ObjectTraits& operator=(const ObjectTraits& other) noexcept = delete;

        ObjectTraits(ObjectTraits&& other) noexcept;
        ObjectTraits(const ObjectTraits& other) noexcept = delete;
    private:
        ObjectTraits();
    public:
        ~ObjectTraits();
        [[nodiscard]] GameObjectType getGameObjectType() const;
        [[nodiscard]] int getDefaultMainColorID() const;
        [[nodiscard]] ZLayer getDefaultZLayer() const;
        [[nodiscard]] int getDefaultZOrder() const;
        [[nodiscard]] short getSpeedMod() const;
        [[nodiscard]] bool canRotateFree() const;
        [[nodiscard]] bool ignoreEditorDuration() const;
        [[nodiscard]] bool isColorTrigger() const;
        [[nodiscard]] bool isSpawnableTrigger() const;
        [[nodiscard]] bool isStoppableTrigger() const;
        [[nodiscard]] bool isEditorSpawnableTrigger() const;
        [[nodiscard]] bool isSpeedObject() const;
        [[nodiscard]] bool isEditorReserved() const;
        [[nodiscard]] bool omitTrashTexture() const;
        [[nodiscard]] bool shouldLockX() const;
        [[nodiscard]] bool shouldNotHideAnimFreeze() const;
        [[nodiscard]] bool usesFreezeAnimation() const;
        [[nodiscard]] bool usesSpecialAnimation() const;
        void playShineEffect(geode::Function<void()> original) const;
    };

    #define OBJECT_CUSTOM_IMPLEMENT(source, custom, ...) \
        if (source && source->m_objectID >= object_collab::ObjectAPI::getBaseCustomObjectID()) { \
            if (object_collab::CustomObjectInterface* custom = geode::cast::typeinfo_cast<object_collab::CustomObjectInterface*>(source)) __VA_ARGS__; \
        }
}