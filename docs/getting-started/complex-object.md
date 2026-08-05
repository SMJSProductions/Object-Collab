---
title: 3. A Complex Object
order: 3
---

# A Complex Object

If you want a complex object where you can add custom behavior you can create one by doing something comparable to the following example:

```cpp
#include <Geode/Geode.hpp>
#include <smjs.object-collab/include/object_collab.hpp>

using namespace geode::prelude;
using namespace object_collab::prelude;

// Extends CustomObject with traits of EffectGameObject
class $object(AutoJumpUfoPortal, EffectGameObject) {
public:
    static AutoJumpUfoPortal* create(ObjectInfo* info) {
        // The mod will internally handle auto releasing and calling init.
        return new AutoJumpUfoPortal(info);
    }

    static PopupOptions getEditSpecialConfig(const Selected& selected) {
        // Sets up a UI with a size of 270x120 with a standard no multi activate toggle and a single toggle menu to switch active on and off.
        return PopupConfig::builder()
            .width(270)
            .height(120)
            .title("Auto Jump UFO Portal")
            .info(InfoPopup::builder()
                .title("Auto Jump UFO Portal Help")
                .description("Works like a UFO but boosts the player when switching gamemode.")
                .build())
            .noMultiActivateToggle(true)
            .menu(ToggleMenu::builder()
                .title("Activate")
                .onValue([](bool value, const Selected& selected, Popup* popup) {
                    // Sets the value of the active field using the property key.
                    applyValueToSelected(selected, &AutoJumpUfoPortal::m_active, value);
                })
                .currentValue([](const Selected& selected, Popup* popup) {
                    // Gets the value of the active field using the property key.
                    // If no common value can be found it will use the default value set in the property.
                    return getCommonValueOrDefault(selected, &AutoJumpUfoPortal::m_active);
                })
                .build())
            .build();
    }

    bool m_active;

    AutoJumpUfoPortal(ObjectInfo* info): CustomObject(info, GameObjectType::UfoPortal) { }

    std::vector<std::string> getObjectDetails() override {
        return DetailsBuilder::builder()
            // Since this is a bool it will automatically translate the value to either "Yes" or "No"
            .field("Active", m_active)
            .build();
    }

    void activatedByPlayer(PlayerObject* player) override {
        if (m_active) {
            CustomObject::activatedByPlayer(player);

            player->boostPlayer(player->m_isUpsideDown ? -10 : 10);
        }
    }
};

$on_mod(Loaded) {
    ObjectAPI::registerObject(ObjectInfo::builder()
        .id("id"_spr)
        .sprite("portal.png"_spr)
        // Put the portal between the modifier objects (e.g. Other portals, pads & rings).
        .editorTab(EditorTab::Modifiers)
        .construction(ComplexObject::builder()
            .factory(AutoJumpUfoPortal::create)
            // Sets the custom property on key 150 (Which is not by default saved by EffectGameObject) with a default value of true.
            // These properties will automate saving, loading & updating values and handling their defaults when no common value can be found in a selection.
            .customProperties({
                PropertyInterface::from(150, &AutoJumpUfoPortal::m_active, true)
            })
            .build())
        .editSpecial(AutoJumpUfoPortal::getEditSpecialConfig)
        .build());
}
```

## What Does The Code Do?

### Registration

The object is being registered using `"id"_spr`. Internally the string is being associated with a numeric object ID compatible with how GD handles object identification. Whenever you save the level the numeric IDs get optimized to whatever is in the level and get linked to the string ID provided here. For this reason you must keep your IDs the same to allow loading old level data. The identifier must also be suffixed by `_spr` to make sure that IDs between mods never overlap.

In this setup you also define sprite used by the object and the custom properties which gets used to automate value saving, loading & updating inside the object.

> :warning: Sprites must always be part of a spritesheet.

### Initialization

The object gets initialized as a `CustomObject<EffectGameObject>`. This means it's a custom object with the inherited traits of an `EffectGameObject` and will thus allow features like being activated by the player. It also sets the `GameObjectType` to `UfoPortal` which means it inherits the collision handling of a UFO portal object.

> :information_source: Note that all auto release and initialization handling is left to the internal API. It is not recommended to do so yourself.

### Activating

When the player touches an `EffectGameObject` with a compatible `GameObjectType`, it will trigger the `activatedByPlayer` method. Here it checks if our portal is active using our custom property and based on that run the original along with boosting the player either negatively or positively depending on the flip state of the player.

### Getting Details

When you're in the editor and select an object. Given you have the option enabled you can see details about an object. Object Collab allows you to extend this block with your own information. Using the `DetailsBuilder` you can easily add standard formatted strings to this block. In this example it will turn the property into either `Active: Yes` or `Active: No`.

> :information_source: Note that DetailsBuilder is not required. It's just a QOL feature.

### Edit Special Config

Object Collab allows you to configure the popup shown when you click either edit special or edit object inside the editor. You can either return a raw Popup pointer or in the case of the example a UI completely auto generated using builder patterns.

Here we define the dimensions, title, info button content, menus & toggles. It also supports adding standard toggles GD often uses for triggers and modifiers.