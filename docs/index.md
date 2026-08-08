# Object-Collab

This site documents the workings of Object-Collab. A mod which aims to add easy to setup custom objects to GD while allowing compatibility between gameplay mods.

## Getting Started

See [Getting Started](getting-started) for a by step tutorial on getting started with Object-Collab.

## How it works

All objects are registered under a given ID with the mod name as a prefix. This way it avoids double registrations. The mod will then internally assign it a numeric object ID compatible with the GD object ID system. Once a level is edited and saved it will then create a minimal reference sheet which instructs the mod for others how to rebind the object IDs in the level with their environment.

This way you can run on completely different mod setups & versions without running into conflicts as long as the mods which were used for the level are installed.