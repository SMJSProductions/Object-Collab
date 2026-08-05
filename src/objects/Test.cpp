#include <object_collab.hpp>

using namespace object_collab::prelude;

class $object(AutoJumpUfoPortal, EnhancedGameObject) {
public:
    static AutoJumpUfoPortal* create(ObjectInfo* info) {
        // The mod will internally handle auto releasing and calling init.
        return new AutoJumpUfoPortal(info);
    }

    bool m_active;

    AutoJumpUfoPortal(ObjectInfo* info): CustomObject(info, GameObjectType::Solid) { }

    void collidedByPlayer(PlayerObject* player) override {
        geode::log::warn("Collision spotted");
    }
};

$on_mod(Loaded) {
    ObjectAPI::registerObject(ObjectInfo::builder()
        .id("id"_spr)
        .sprite("square_01_001.png")
        .construction(ComplexObject::builder()
            .factory(AutoJumpUfoPortal::create)
            .build())
        .build());
}