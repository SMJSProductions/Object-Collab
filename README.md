# <img src="resources/small-logo.png" height="45"> Object Collab

A mod which provides a complete framework for custom objects while solving compatibility issues.

## Features

### Client features

- Full editor support.
- Full gameplay support.
- Upload/download support.
- Multi mod support.
- Automatic compatibility resolving support.
- Mod of origin info in the editor.
- A custom spawn trigger based on mod presence.
- Automatically creating a dependency list of used objects.
- Preserving all modded objects with the mod unloaded as long as guidelines aren't recorded.

### Framework features

- A complete framework.
- Full access to the internal configuration.
- Object inheritance support.
- Automatic property handling.
- Automatic trigger handling.
- Full support for all object types.
- Custom object details support.
- Quick simple object configuration.
- Builder based property editing UIs.
- Full customization support for the property editing UIs.

## How it works

All objects are registered under a given ID with the mod name as a prefix. This way it avoids double registrations. The mod will then internally assign it a numeric object ID compatible with the GD object ID system. Once a level is edited and saved it will then create a minimal reference sheet which instructs the mod for others how to rebind the object IDs in the level with their environment.

This way you can run on completely different mod setups & versions without running into conflicts as long as the mods which were used for the level are installed.

## Credits

- [Alphalaneous](https://github.com/Alphalaneous) for general help with the editor & the object command system.
- [Undefined06855](https://github.com/undefined06855) for the GD web decomp.
- [Flow](https://github.com/FlowVix) for the object properties documentation.
- [Dankmeme](https://github.com/dankmeme01) for pointing out some missed issues.