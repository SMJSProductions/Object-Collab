#include "EditButtonBar.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

EditButtonBar* ModEditButtonBar::create(CCArray* objects, CCPoint position, const int tab, const bool hasCreateItems, const int columns, const int rows) {
    if (!ModEditorUI::getEarly() || !hasCreateItems || tab == 13) return EditButtonBar::create(objects, std::move(position), tab, hasCreateItems, columns, rows);

    const Register& objectRegister = ObjectAPI::getRegister();
    CCObject* test = new CCObject();
    CCArray* newObjects = CCArray::create();

    for (const auto& [key, info] : objectRegister) {
        if (static_cast<int>(info.getEditorTab()) == tab) {
            newObjects->addObject(ModEditorUI::getEarly()->getCreateBtn(key, static_cast<int>(info.getEditorButtonColor())));
        }
    }

    newObjects->addObjectsFromArray(objects);

    return EditButtonBar::create(newObjects, std::move(position), tab, hasCreateItems, columns, rows);
}