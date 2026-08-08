#include "CollisionHandling.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

void CollisionModPlayLayer::destroyPlayer(PlayerObject* player, GameObject* object) {
    PlayLayer::destroyPlayer(player, object);

    CUSTOM_OBJECT_IMPLEMENT(object, CustomObjectInterface, custom, custom->collidedByPlayer(player));
}

void CollisionModLevelEditorLayer::playerTookDamage(PlayerObject* player) {
    LevelEditorLayer::playerTookDamage(player);

    for (GameObject* object : m_hazardCollisionObjects) {
        CUSTOM_OBJECT_IMPLEMENT(object, CustomObjectInterface, custom, custom->collidedByPlayer(player));
    }
}

bool CollisionModPlayerObject::collidedWithObjectInternal(const float dt, GameObject* object, CCRect rect, const bool skipCheck) {
    if (PlayerObject::collidedWithObjectInternal(dt, object, std::move(rect), skipCheck)) {
        CUSTOM_OBJECT_IMPLEMENT(object, CustomObjectInterface, custom, custom->collidedByPlayer(this));

        return true;
    }

    return false;
}

void CollisionModPlayerObject::collidedWithSlopeInternal(const float dt, GameObject* object, const bool forced) {
    PlayerObject::collidedWithSlopeInternal(dt, object, forced);

    CUSTOM_OBJECT_IMPLEMENT(object, CustomObjectInterface, custom, custom->collidedByPlayer(this));
}