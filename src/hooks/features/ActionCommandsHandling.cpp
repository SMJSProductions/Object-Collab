#include "ActionCommandsHandling.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

void ActionCommandsModGJEffectManager::controlActionsForControlID(const int id, const GJActionCommand command) {
    GJEffectManager::controlActionsForControlID(id, command);

    for (const GroupCommandObject2& groupCommandObject : m_unkVector560) {
        if (groupCommandObject.m_controlID != id) continue;

        CUSTOM_OBJECT_IMPLEMENT(groupCommandObject.m_gameObject, CustomObjectInterface, custom, {
            custom->getTraits().controlIDCommand(command);
        });
    }
}

void ActionCommandsModGJBaseGameLayer::controlTriggersInGroup(const int group, const GJActionCommand command) {
    GJBaseGameLayer::controlTriggersInGroup(group, command);

    for (GameObject* object : CCArrayExt<GameObject*>(this->getGroup(group))) {
        CUSTOM_OBJECT_IMPLEMENT(object, CustomObjectInterface, custom, {
            custom->getTraits().objectGroupCommand(command);
        });
    }
}