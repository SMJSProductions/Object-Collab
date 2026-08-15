#include "EditorPopup.hpp"

using namespace object_collab::prelude;
using namespace geode::prelude;

EditorPopup* EditorPopup::create(Selected selected, PopupConfig& config) {
    EditorPopup* popup = new EditorPopup(std::move(selected), config);

    if (popup && popup->init(config)) {
        popup->autorelease();

        return popup;
    } else {
        delete popup;

        return nullptr;
    }
}

EditorPopup::EditorPopup(Selected selected, PopupConfig& config): PopupExtra(PopupExtra::CloseSetup::NoButton),
m_selected(std::move(selected)),
m_onClose(config.releaseOnClose()) { }

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
    this->addToggles(config, true);
    this->addToggles(config, false);
    this->addMenus(config);

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

void EditorPopup::addMenus(PopupConfig& config) {
    CCMenu* menuContainer = CCMenu::create();

    for (const std::unique_ptr<ValueMenu>& menu : config.getMenus()) {
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

    menuContainer->setContentSize(m_contentLayer->getContentSize() - ccp(0, 10));
    menuContainer->setLayout(TableLayout::create(Axis::Row)
        ->setMainAxisGap(config.getGapX())
        ->setCrossAxisGap(config.getGapY())
        ->setMinScale(config.getMinScale())
        ->setMaxScale(config.getMaxScale())
        ->inverseCrossAxis(true));
    m_contentLayer->addChild(menuContainer);
    m_contentLayer->updateLayout();
}

void EditorPopup::addToggles(PopupConfig& config, const bool left) {
    CCNode* container = CCNode::create();
    bool shouldAddMultiTrigger = false;

    if (left && config.getTriggerToggles()) {
        shouldAddMultiTrigger = this->addTriggerToggles(container);
    }

    if (left && config.getNoMultiActivateToggle()) {
        this->addNoMultiActivateToggle(container);
    }

    for (const std::unique_ptr<ToggleMenu>& toggleMenu : (left ? config.getLeftToggles() : config.getRightToggles())) {
        ToggleMenuNode* toggle = ToggleMenuNode::create(m_selected, this, *toggleMenu.get());

        if (left) {
            reinterpret_cast<SimpleAxisLayout*>(toggle->getLayout())->setMainAxisDirection(AxisDirection::LeftToRight);
        } else {
            reinterpret_cast<SimpleAxisLayout*>(toggle->getLayout())->setMainAxisDirection(AxisDirection::RightToLeft);
        }

        toggle->updateLayout();
        container->addChild(toggle);
    }

    if (shouldAddMultiTrigger) {
        this->addMultiTriggerToggle(container);
    }

    container->setContentSize({
        m_extraButtons->getPositionX() - m_extraButtons->getContentWidth() * m_extraButtons->getAnchorPoint().x - 20,
        40
    });
    container->setAnchorPoint({ static_cast<float>(!left), 0 });
    container->setLayout(TableLayout::create(Axis::Column)
        ->setGap(10)
        ->setMinScale(0.4f)
        ->setMaxScale(1)
        ->inverseMainAxis(true)
        ->inverseCrossAxis(!left));
    m_buttonMenu->addChildAtPosition(container, left ? Anchor::BottomLeft : Anchor::BottomRight, { left ? 10.0f : -10.0f, 10 });
}

bool EditorPopup::addTriggerToggles(CCNode* container) {
    ToggleMenuNode* touchTrigger = ToggleMenuNode::create(m_selected, this, *ToggleMenu::builder()
        .id("touch-trigger"_spr)
        .title("Touch\nTrigger")
        .currentValue([](const Selected& selected, Popup* popup) {
            return getCommonValueOrDefault(selected, &EffectGameObject::m_isTouchTriggered);
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

            applyValueToSelected(selected, &EffectGameObject::m_isTouchTriggered, value);
        })
        .build()
        .get());
    ToggleMenuNode* spawnTrigger = ToggleMenuNode::create(m_selected, this, *ToggleMenu::builder()
        .id("spawn-trigger"_spr)
        .title("Spawn\nTrigger")
        .currentValue([](const Selected& selected, Popup* popup) {
            return getCommonValueOrDefault(selected, &EffectGameObject::m_isSpawnTriggered);
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

            applyValueToSelected(selected, &EffectGameObject::m_isSpawnTriggered, value);
        })
        .build()
        .get());

    container->addChild(touchTrigger);
    container->addChild(spawnTrigger);

    return touchTrigger->getValueNode<CCMenuItemToggler>()->isToggled() || spawnTrigger->getValueNode<CCMenuItemToggler>()->isToggled();
}

void EditorPopup::addNoMultiActivateToggle(CCNode* container) {
    container->addChild(ToggleMenuNode::create(m_selected, this, *ToggleMenu::builder()
        .id("no-multi-activate"_spr)
        .title("No Multi\nActivate")
        .currentValue([](const Selected& selected, Popup* popup) {
            return getCommonValueOrDefault(selected, &EnhancedGameObject::m_isNoMultiActivate);
        })
        .onValue([](const bool value, const Selected& selected, Popup* popup) {
            applyValueToSelected(selected, &EnhancedGameObject::m_isNoMultiActivate, value);
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
            return getCommonValueOrDefault(selected, &EffectGameObject::m_isMultiTriggered);
        })
        .onValue([](const bool value, const Selected& selected, Popup* popup) {
            applyValueToSelected(selected, &EffectGameObject::m_isMultiTriggered, value);
        })
        .build()
        .get()));
    container->updateLayout();
}

void EditorPopup::onClose(CCObject* sender) {
    if (m_onClose) m_onClose(sender, m_selected, this);
    PopupExtra::onClose(sender);
}