#include "EditorUI.hpp"

using namespace std::string_view_literals;
using namespace object_collab::prelude;
using namespace geode::prelude;

EditorUI* ModEditorUI::INSTANCE = nullptr;

EditorUI* ModEditorUI::getEarly() {
    return ModEditorUI::INSTANCE;
}

ModEditorUI::~ModEditorUI() {
    ModEditorUI::INSTANCE = nullptr;
}

bool ModEditorUI::init(LevelEditorLayer* editorLayer) {
    ModEditorUI::INSTANCE = this;

    if (!EditorUI::init(editorLayer)) {
        return false;
    } else if (Mod::get()->getSettingValue<bool>("link-buttons-reposition") && !Loader::get()->isModLoaded("alphalaneous.tinker")) {
        CCNode* linkMenu = this->getChildByID("link-menu");

        linkMenu->setScale(0.6f);
        linkMenu->setAnchorPoint({ 0.5f, 1 });
        linkMenu->setLayout(SimpleRowLayout::create()
            ->setMainAxisAlignment(MainAxisAlignment::Start)
            ->setMainAxisScaling(AxisScaling::Fit)
            ->setCrossAxisScaling(AxisScaling::Fit)
            ->setGap(3));
        linkMenu->setPosition(this->getChildByID("position-slider")->getPosition() - ccp(0, 17));
    }

    return true;
}

void ModEditorUI::toggleEditObjectButton() {
    const std::optional<int> key = this->currentSelectedObjectID();

    if (key) {
        if (ObjectInfo* info = ObjectAPI::getCustomObject(key.value())) {
            this->toggleButton(m_editObjectBtn, info->hasEditObject() || this->editButtonUsable());
            this->toggleButton(m_editSpecialBtn, info->hasEditSpecial());
            this->updateEditColorButton();
        } else {
            EditorUI::toggleEditObjectButton();
        }
    } else {
        EditorUI::toggleEditObjectButton();
    }

    if (Mod::get()->getSettingValue<bool>("special-button-fix")) {
        this->fixEditSpecial(key);
    }
}

void ModEditorUI::editObject(CCObject* sender) {
    if (std::vector<GameObject*> selected = this->currentSelectedObjects(); selected.size()) {
        ObjectInfo* info = ObjectAPI::getCustomObject(selected[0]->m_objectID);

        if (!info || !info->hasEditObject()) return EditorUI::editObject(sender);

        Selected customSelected;

        customSelected.reserve(selected.size());

        for (GameObject* object : selected) {
            customSelected.emplace_back(typeinfo_cast<CustomObjectInterface*>(object));
        }

        Popup* popup = std::visit<Popup*>(makeVisitor{
            [](Popup* popup) {
                return popup;
            },
            [&customSelected](PopupConfig config) {
                return EditorPopup::create(std::move(customSelected), config);
            }
        }, info->editObject(customSelected));

        popup->show();
        popup->addEventListener(Popup::CloseEvent(popup), [this]() {
            if (m_selectedObject) {
                EditorUI::updateObjectInfoLabel();
            }
        });
    } else {
        EditorUI::editObject(sender);
    }
}

void ModEditorUI::editObjectSpecial(const int type) {
    if (std::vector<GameObject*> selected = this->currentSelectedObjects(); selected.size()) {
        ObjectInfo* info = ObjectAPI::getCustomObject(selected[0]->m_objectID);

        if (!info || !info->hasEditSpecial()) return EditorUI::editObjectSpecial(type);

        Selected customSelected;

        customSelected.reserve(selected.size());

        for (GameObject* object : selected) {
            customSelected.emplace_back(typeinfo_cast<CustomObjectInterface*>(object));
        }

        Popup* popup = std::visit<Popup*>(makeVisitor{
            [](Popup* popup) {
                return popup;
            },
            [&customSelected](PopupConfig config) {
                return EditorPopup::create(std::move(customSelected), config);
            }
        }, info->editSpecial(customSelected));

        popup->show();
        popup->addEventListener(Popup::CloseEvent(popup), [this]() {
            if (m_selectedObject) {
                EditorUI::updateObjectInfoLabel();
            }
        });
    } else {
        EditorUI::editObjectSpecial(type);
    }
}

void ModEditorUI::onCreateObject(const int id) {
    EditorUI::onCreateObject(id);

    if (m_objectInfoLabel->getString() == " "sv && Mod::get()->getSettingValue<bool>("object-info-label-fix")) {
        EditorUI::updateObjectInfoLabel();
    }
}

void ModEditorUI::updateObjectInfoLabel() {
    EditorUI::updateObjectInfoLabel();

    if (CustomObjectInterface* object = typeinfo_cast<CustomObjectInterface*>(m_selectedObject)) {
        const std::string_view objectID = ObjectAPI::getCustomObject(object->getGameObject()->m_objectID)->getID();
        std::vector<std::string> customInfo = object->getObjectDetails();
        utils::StringBuffer info;

        info.append(m_objectInfoLabel->getString());

        if (info.view().back() != '\n') {
            info.append('\n');
        }

        for (const std::string_view customInfoEntry : customInfo) {
            info.append("{}\n", customInfoEntry);
        }

        info.append("Object Mod: {}", objectID.substr(0, objectID.find('/')));
        info.append('\n');
        m_objectInfoLabel->setString(info.c_str());
    }
}

void ModEditorUI::toggleButton(CCMenuItemSpriteExtra* button, const bool enabled) {
    button->m_animationEnabled = enabled;

    if (enabled) {
        button->setColor(ccWHITE);
        button->setOpacity(0xFF);
    } else {
        button->setColor(ccGRAY);
        button->setOpacity(0xAF);
    }
}

void ModEditorUI::fixEditSpecial(const std::optional<int>& selectedKey) {
    // Fix a vanilla bug where edit special is highlighted when different objects are selected
    if (selectedKey == std::nullopt && m_selectedObjects) {
        bool enabled = false;

        for (size_t i = 0; i < m_selectedObjects->count(); i++) {
            if (reinterpret_cast<GameObject*>(m_selectedObjects->objectAtIndex(i))->m_classType == GameObjectClassType::Smart) {
                enabled = true;

                break;
            }
        }

        this->toggleButton(m_editSpecialBtn, enabled);
    }

    // Fix a vanilla bug where setEnabled is never properly set on edit special
    m_editSpecialBtn->setEnabled(m_editSpecialBtn->m_animationEnabled);
}

std::optional<int> ModEditorUI::currentSelectedObjectID() {
    if (m_selectedObject) {
        return m_selectedObject->m_objectID;
    } else if (const size_t size = m_selectedObjects ? m_selectedObjects->count() : 0) {
        const int key = reinterpret_cast<GameObject*>(m_selectedObjects->objectAtIndex(0))->m_objectID;

        for (size_t i = 1; i < size; i++) {
            if (key != reinterpret_cast<GameObject*>(m_selectedObjects->objectAtIndex(i))->m_objectID) {
                return std::nullopt;
            }
        }

        return key;
    }

    return std::nullopt;
}

std::vector<GameObject*> ModEditorUI::currentSelectedObjects() {
    if (m_selectedObject) {
        return { m_selectedObject };
    } else if (const size_t size = m_selectedObjects ? m_selectedObjects->count() : 0) {
        std::vector<GameObject*> objects = { reinterpret_cast<GameObject*>(m_selectedObjects->objectAtIndex(0)) };

        for (size_t i = 1; i < size; i++) {
            objects.emplace_back(reinterpret_cast<GameObject*>(m_selectedObjects->objectAtIndex(i)));

            if (objects.back()->m_objectID != objects[0]->m_objectID) {
                return {};
            }
        }

        return objects;
    }

    return {};
}