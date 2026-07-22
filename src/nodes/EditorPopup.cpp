#include "EditorPopup.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

EditorPopup* EditorPopup::create(Selected selected, PopupConfig& config) {
    EditorPopup* popup = new EditorPopup(std::move(selected));

    if (popup && popup->init(config)) {
        popup->autorelease();

        return popup;
    } else {
        delete popup;

        return nullptr;
    }
}

EditorPopup::EditorPopup(Selected selected): PopupExtra(PopupExtra::CloseSetup::NO_BUTTON), m_selected(std::move(selected)) { }

bool EditorPopup::init(PopupConfig& config) {
    if (!PopupExtra::init(config.getTitle(), {
        config.getWidth(),
        config.getHeight()
    }, makeVector<PopupExtra::ButtonConfig>(
        PopupExtra::ButtonConfig{ "OK", "GJ_button_01.png", [this](CCMenuItemSpriteExtra*) { this->onClose(nullptr); } }
    ))) return false;

    m_noElasticity = true;

    this->setID(config.getID());
    this->addInfo(config.getInfo());
    this->addToggles(config);
    this->addMenus(config.releaseMenus());

    return true;
}

void EditorPopup::addInfo(const InfoPopup& info) {
    const ZStringView title = info.getTitle();
    const ZStringView description = info.getDescription();

    if (title.empty() && description.empty()) return;

    InfoAlertButton* infoBtn = InfoAlertButton::create(title, description, 1);

    infoBtn->setID(info.getID().empty() ? "info"_spr : info.getID());
    infoBtn->setPosition((ccp(6.5f, 6.5f) + infoBtn->getContentSize() / 2) * ccp(1, -1) + ccp(0, m_size.height));
    m_buttonMenu->addChild(infoBtn);
}

void EditorPopup::addMenus(std::vector<std::unique_ptr<ValueMenu>> menus) {
    CCMenu* menuContainer = CCMenu::create();

    for (const std::unique_ptr<ValueMenu>& menu : menus) {
        if (ToggleMenu* toggleMenu = typeinfo_cast<ToggleMenu*>(menu.get())) {
            menuContainer->addChild(ToggleMenuNode::create(m_selected, this, *toggleMenu));
        } else if (NumericMenu* numericMenu = typeinfo_cast<NumericMenu*>(menu.get())) {
            menuContainer->addChild(NumericMenuNode::create(m_selected, this, *numericMenu));
        } else if (InputMenu* inputMenu = typeinfo_cast<InputMenu*>(menu.get())) {
            menuContainer->addChild(InputMenuNode::create(m_selected, this, *inputMenu));
        } else if (EnumMenu* enumMenu = typeinfo_cast<EnumMenu*>(menu.get())) {
            menuContainer->addChild(EnumMenuNode::create(m_selected, this, *enumMenu));
        } else if (CustomValueMenu* customValueMenu = typeinfo_cast<CustomValueMenu*>(menu.get())) {
            menuContainer->addChild(CustomValueMenuNode::create(m_selected, this, *customValueMenu));
        } else if (AxisLayoutMenu* axisLayoutMenu = typeinfo_cast<AxisLayoutMenu*>(menu.get())) {
            menuContainer->addChild(AxisLayoutMenuNode::create(m_selected, this, *axisLayoutMenu));
        }
    }

    menuContainer->setContentSize(m_contentLayer->getContentSize());
    menuContainer->setLayout(TableLayout::create(Axis::Row)
        ->setGap(10)
        ->setMinScale(0.4f)
        ->setMaxScale(0.8f)
        ->inverseCrossAxis(true));
    m_contentLayer->addChild(menuContainer);
    m_contentLayer->updateLayout();
}

void EditorPopup::addToggles(object_collab::editor_popup::PopupConfig& config) {
    CCNode* container = CCNode::create();
    bool shouldAddMultiTrigger = false;

    if (config.getTriggerToggles()) {
        shouldAddMultiTrigger = this->addTriggerToggles(container);
    }

    if (config.getNoMultiActivateToggle()) {
        this->addNoMultiActivateToggle(container);
    }

    for (const std::unique_ptr<ToggleMenu>& toggleMenu : config.releaseToggles()) {
        container->addChild(ToggleMenuNode::create(m_selected, this, *toggleMenu.get()));
    }

    if (shouldAddMultiTrigger) {
        this->addMultiTriggerToggle(container);
    }

    container->setContentSize({
        m_extraButtons->getPositionX() - m_extraButtons->getContentWidth() * m_extraButtons->getAnchorPoint().x - 20,
        40
    });
    container->setLayout(TableLayout::create(Axis::Column)
        ->setGap(10)
        ->setMinScale(0.4f)
        ->setMaxScale(1)
        ->inverseMainAxis(true));
    m_buttonMenu->addChildAtPosition(container, Anchor::BottomLeft, { 10, 10 });
}

bool EditorPopup::addTriggerToggles(CCNode* container) {
    ToggleMenuNode* touchTrigger = ToggleMenuNode::create(m_selected, this, *ToggleMenu::builder()
        .id("touch-trigger"_spr)
        .title("Touch\nTrigger")
        .currentValue([](const Selected& selected, Popup* popup) {
            return getCommonValueOrDefault(selected, &CustomObject::m_isTouchTriggered, false);
        })
        .onValue([this, container](const bool value, const Selected& selected, Popup* popup) {
            

            if (value) {
                CCMenuItemToggler* spawnTrigger = reinterpret_cast<ToggleMenuNode*>(container->getChildByID("spawn-trigger"_spr))
                    ->getValueNode<CCMenuItemToggler>();

                if (spawnTrigger->isToggled()) spawnTrigger->toggleWithCallback(false);

                this->addMultiTriggerToggle(container);
            } else {
                container->getChildByID("multi-trigger"_spr)->removeFromParent();
                container->updateLayout();
            }

            applyValueToSelected(selected, &CustomObject::m_isTouchTriggered, value);
        })
        .build()
        .get());
    ToggleMenuNode* spawnTrigger = ToggleMenuNode::create(m_selected, this, *ToggleMenu::builder()
        .id("spawn-trigger"_spr)
        .title("Spawn\nTrigger")
        .currentValue([](const Selected& selected, Popup* popup) {
            return getCommonValueOrDefault(selected, &CustomObject::m_isSpawnTriggered, false);
        })
        .onValue([this, container](const bool value, const Selected& selected, Popup* popup) {
            if (value) {
                CCMenuItemToggler* touchTrigger = reinterpret_cast<ToggleMenuNode*>(container->getChildByID("touch-trigger"_spr))
                    ->getValueNode<CCMenuItemToggler>();

                if (touchTrigger->isToggled()) touchTrigger->toggleWithCallback(false);

                this->addMultiTriggerToggle(container);
            } else {
                container->getChildByID("multi-trigger"_spr)->removeFromParent();
                container->updateLayout();
            }

            applyValueToSelected(selected, &CustomObject::m_isSpawnTriggered, value);
        })
        .build()
        .get());

    container->addChild(touchTrigger);
    container->addChild(spawnTrigger);

    return touchTrigger->getValueNode<CCMenuItemToggler>()->isToggled() || spawnTrigger->getValueNode<CCMenuItemToggler>()->isToggled();
}

void EditorPopup::addNoMultiActivateToggle(cocos2d::CCNode* container) {
    container->addChild(ToggleMenuNode::create(m_selected, this, *ToggleMenu::builder()
        .id("no-multi-activate"_spr)
        .title("No Multi\nActivate")
        .currentValue([](const Selected& selected, Popup* popup) {
            return getCommonValueOrDefault(selected, &CustomObject::m_isNoMultiActivate, false);
        })
        .onValue([](const bool value, const Selected& selected, Popup* popup) {
            applyValueToSelected(selected, &CustomObject::m_isNoMultiActivate, value);
        })
        .build()
        .get()));
}

void EditorPopup::addMultiTriggerToggle(CCNode* container) {
    if (container->getChildByID("multi-trigger"_spr)) return;

    container->addChild(ToggleMenuNode::create(m_selected, this, *ToggleMenu::builder()
        .id("multi-trigger"_spr)
        .title("Multi\nTrigger")
        .currentValue([](const Selected& selected, Popup* popup) {
            return getCommonValueOrDefault(selected, &CustomObject::m_isMultiTriggered, false);
        })
        .onValue([](const bool value, const Selected& selected, Popup* popup) {
            applyValueToSelected(selected, &CustomObject::m_isMultiTriggered, value);
        })
        .build()
        .get()));
    container->updateLayout();
}