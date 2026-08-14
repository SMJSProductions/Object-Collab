# Object-Collab Changelog

## v0.5.5-beta.1

- Added an on close callback for editor popups.

## v0.5.4-beta.1

- Final touches for the documentation.
- Refactored the optional API to include its own prelude and has a name consistent with the main include file.

## v0.5.3-beta.1

- Fixed input menus default allowed characters blocking all inputs.

## v0.5.2-beta.1

- Added custom editor tab support.

## v0.5.1-beta.1

- Added editor button color support.
- Added official support for CUSTOM_OBJECT_IMPLEMENT.
- Added an object ID swap to semi safely simulate hardcoded behavior.
- Made the object mod info show the mod name rather than the ID.
- Fixed min, max being ignored for numeric text inputs.

## v0.5.0-beta.1

- Added a way to set raw trigger text.
- Added command traits based on Alphalaneous' implementation.
- Fixed the compatibility popup not cancelling.
- Fixed onPlayShineEffect not being correctly implemented.
- Fixed the portal back ZLayer not working as intended.
- Full unsupported speed objects.

## v0.4.1-beta.1

- Added padding support for AxisLayoutMenu.
- Fixed back frames not rendering due to not being in the node container.

## v0.4.0-beta.1

- Added a builder based trait system for better ABI.
- Added a play layer post init.

## v0.3.4-beta.1

- Disable speed object support due to instability.

## v0.3.3-beta.1

- Fixed a bug with placing objects.

## v0.3.2-beta.1

- Removed a test object.

## v0.3.1-beta.1

- Added speed mod support.

## v0.3.0-beta.1

- Added scaling options for editor popup config.
- Added collision handling.
- Added back frame support.
- Added speed object support.
- Added playShineEffect virtual support.
- Added a force swap for coin game object types.
- Fixed speed portals being invisible.

## v0.2.0-beta.1

- Added an optional API.
- Added a trailing newline to the object info label for compatibility.
- Added an inverse option for toggle menu.
- Fix objects not calling post inits on pasting.
- Fix the right toggle menu alignment.

## v0.1.0-alpha.6

- General metadata changes.

## v0.1.0-alpha.5

- Added documentation.
- Added the $object macro.
- Refactored Effects to Animated in the editor tabs.
- Made the vanilla toggles use the standard methods.

## v0.1.0-alpha.4

- Reworked properties.
- Properly separated quick & complex objects.
- Fix some undefined behavior on save string splitting.

## v0.1.0-alpha.3

- Fix triggers being visible in the PlayLayer.

## v0.1.0-alpha.2

- Fix some toggle issues.
- Fix level storage API being included in the API section.

## v0.1.0-alpha.1

- The initial release.