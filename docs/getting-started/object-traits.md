---
title: 4. Object Traits
order: 4
---

# Object Traits

An object has 2 different kinds of trait systems. method traits and general behavior traits. The latter was already showcased in the [previous chapter](/Object-Collab/getting-started/complex-object) as `GameObjectType`. Besides this Object-Collab provides a bunch of virtual methods which allows you to set method traits.

## Game Object Type

> :information_source: `triggerActivated` is called on collision for particle effects regardless of the object already been triggered before. `triggerObject` is called when the object should execute trigger behavior.

> :warning: Using a class which can't be casted to the base class of a GameObjectType should be considered UB as GD tends to blindly cast based on the latter.

| Type                   | Behavior                                                                               | Base Class             | Activation Method                  |
|------------------------|----------------------------------------------------------------------------------------|------------------------|------------------------------------|
| `Solid`                | An object which is solid & lethal to touch from the sides.                             | `GameObject`           | `collidedByPlayer`                 |
| `Hazard`               | An object which is lethal to touch.                                                    | `GameObject`           | `collidedByPlayer`                 |
| `InverseGravityPortal` | An object which inverses gravity on touch.                                             | `EffectGameObject`     | `activatedByPlayer`                |
| `NormalGravityPortal`  | An object which sets the gravity back to normal on touch.                              | `EffectGameObject`     | `activatedByPlayer`                |
| `ShipPortal`           | An object which turns the player into a ship on touch.                                 | `EffectGameObject`     | `activatedByPlayer`                |
| `CubePortal`           | An object which turns the player into a cube on touch.                                 | `EffectGameObject`     | `activatedByPlayer`                |
| `Decoration`           | An object with no hitbox.                                                              | `GameObject`           | N.A.                               | 
| `YellowJumpPad`        | An object which gives the player a medium boost on touch.                              | `EffectGameObject`     | `activatedByPlayer`                |
| `PinkJumpPad`          | An object which gives the player a small boost on touch.                               | `EffectGameObject`     | `activatedByPlayer`                |
| `GravityPad`           | An object which flips the gravity on touch.                                            | `EffectGameObject`     | `activatedByPlayer`                |
| `YellowJumpRing`       | An object which gives the player a medium boost on click.                              | `RingObject`           | `activatedByPlayer`                |
| `PinkJumpRing`         | An object which gives the player a small boost on click.                               | `RingObject`           | `activatedByPlayer`                |
| `GravityRing`          | An object which flips the gravity on click.                                            | `RingObject`           | `activatedByPlayer`                |
| `InverseMirrorPortal`  | An object which mirrors the level on touch.                                            | `EffectGameObject`     | `triggerActivated`                 |
| `NormalMirrorPortal`   | An object which undoes the level mirror effect on touch.                               | `EffectGameObject`     | `triggerActivated`                 |
| `BallPortal`           | An object which turns the player into a ball on touch.                                 | `EffectGameObject`     | `activatedByPlayer`                |
| `RegularSizePortal`    | An object which turns the player into the regular size on touch.                       | `EffectGameObject`     | `activatedByPlayer`                |
| `MiniSizePortal`       | An object which turns the player into the mini size on touch.                          | `EffectGameObject`     | `activatedByPlayer`                |
| `UfoPortal`            | An object which turns the player into a UFO on touch.                                  | `EffectGameObject`     | `activatedByPlayer`                |
| `Modifier`             | An object which functions as a trigger or speed portal.                                | `EffectGameObject`     | `triggerActivated`/`triggerObject` |
| `Breakable`            | An object which is solid on the top, breakable on touch from any other size.           | `GameObject`           | `collidedByPlayer`                 |
| `SecretCoin`           | An object which counts as a secret coin (It's not recommended to use this).            | `EffectGameObject`     | `triggerActivated`/`triggerObject` |
| `DualPortal`           | An object which turns the player into a dual on touch.                                 | `EffectGameObject`     | `triggerActivated`                 |
| `SoloPortal`           | An object which turns the player into a solo on touch.                                 | `EffectGameObject`     | `triggerActivated`                 |
| `Slope`                | An object which has a sloped hitbox & is lethal to touch from the side.                | `GameObject`           | `collidedByPlayer`                 |
| `WavePortal`           | An object which turns the player into a wave on touch.                                 | `EffectGameObject`     | `activatedByPlayer`                |
| `RobotPortal`          | An object which turns the player into a robot on touch.                                | `EffectGameObject`     | `activatedByPlayer`                |
| `TeleportPortal`       | An object which teleports the player to a tethered object on touch.                    | `TeleportPortalObject` | `activatedByPlayer`                |
| `GreenRing`            | An object which flips gravity & gives the player a medium boost on click.              | `RingObject`           | `activatedByPlayer`                |
| `Collectible`          | An object which can be collected on touch.                                             | `EffectGameObject`     | `triggerActivated`/`triggerObject` |
| `UserCoin`             | An object which counts as a user coin (It's not recommended to use this).              | `EffectGameObject`     | `triggerActivated`/`triggerObject` |
| `DropRing`             | An object which gives the player an extra large downwards boost on click.              | `RingObject`           | `activatedByPlayer`                |
| `SpiderPortal`         | An object which turns the player into a spider on touch.                               | `EffectGameObject`     | `activatedByPlayer`                |
| `RedJumpPad`           | An object which gives the player a large boost on touch.                               | `EffectGameObject`     | `activatedByPlayer`                |
| `RedJumpRing`          | An object which gives the player a large boost on click.                               | `RingObject`           | `activatedByPlayer`                |
| `CustomRing`           | An object which can be jumped on to trigger.                                           | `RingObject`           | `activatedByPlayer`                |
| `DashRing`             | An object which makes the player dash into a direction on hold.                        | `DashRingObject`       | `activatedByPlayer`                |
| `GravityDashRing`      | An object which makes the player dash into a direction while swapping gravity on hold. | `DashRingObject`       | `activatedByPlayer`                |
| `CollisionObject`      | An object which reports a collision from a given block ID (Unsupported).               | `EffectGameObject`     | N.A.                               |
| `Special`              | An object which triggers when touched (Unsupported).                                   | `EffectGameObject`     | N.A.                               |
| `SwingPortal`          | An object which turns the player into a swing on touch.                                | `EffectGameObject`     | `activatedByPlayer`                |
| `GravityTogglePortal`  | An object which flips gravity on touch.                                                | `EffectGameObject`     | `activatedByPlayer`                |
| `SpiderOrb`            | An object which teleports the player to the nearest surface on click.                  | `RingObject`           | `activatedByPlayer`                |
| `SpiderPad`            | An object which teleports the player to the nearest surface on touch.                  | `EffectGameObject`     | `activatedByPlayer`                |
| `EnterEffectObject`    | An object which spawns all objects in an area.                                         | `EnterEffectObject`    | `triggerActivated`/`triggerObject` |
| `TeleportOrb`          | An object which teleports the player to a targeted object using the target group ID.   | `TeleportPortalObject` | `activatedByPlayer`                |
| `AnimatedHazard`       | An object which is lethal to touch.                                                    | `AnimatedGameObject`   | `collidedByPlayer`                 |

## Method Traits

These are methods which can either be inherited or statically define behavior depending on the return state. These should all be filled in correctly to ensure that the game handles the object correctly.

| Method                     | Behavior                                                                                  | Base Class           | Game Object Type | Can Be Inherited |
|----------------------------|-------------------------------------------------------------------------------------------|----------------------|------------------|------------------|
| `getDefaultMainColorID`    | Gets the default main color ID given when the object is created, 0 = color disabled.      | `GameObject`         | N.A.             | Yes              |
| `getSpeedMod`              | Gets the speed mod of the object to apply to the gameplay.                                | `EffectGameObject`   | `Modifier`       | Yes              |
| `canRotateFree`            | If the object can be rotated without 90deg snapping (Unsupported).                        | `GameObject`         | N.A.             | No               |
| `ignoreEditorDuration`     | If the trigger duration handling should be removed.                                       | `EffectGameObject`   | `Modifier`       | Yes              |
| `isColorTrigger`           | If the trigger can affect color channels.                                                 | `EffectGameObject`   | `Modifier`       | Yes              |
| `isEditorSpawnableTrigger` | If the trigger should be simulated in the editor.                                         | `EffectGameObject`   | `Modifier`       | Yes              |
| `isSettingsObject`         | If the play layer should ignore this object as its reserved for the editor.               | `GameObject`         | N.A.             | Yes              |
| `isSpawnableTrigger`       | If the trigger can be spawned.                                                            | `EffectGameObject`   | `Modifier`       | Yes              |
| `isSpecialObject`          | If the object should be omitted from rendering in the custom delete & delete all buttons. | `GameObject`         | N.A.             | Yes              |
| `isSpeedObject`            | If the object can change the gameplay speed.                                              | `EffectGameObject`   | `Modifier`       | Yes              |
| `isStoppableTrigger`       | If the trigger can be manipulated by a stop trigger.                                      | `EffectGameObject`   | `Modifier`       | Yes              |
| `isTrigger`                | If the object should be considered a trigger.                                             | `EffectGameObject`   | `Modifier`       | No               |
| `shouldLockX`              | If the object can be affected by move triggers on the X axis.                             | `GameObject`         | N.A.             | Yes              |
| `shouldNotHideAnimFreeze`  | If the object should disable once the animation freezes.                                  | `EnhancedGameObject` | N.A.             | Yes              |
| `usesFreezeAnimation`      | If the object uses an animation with a delayed start.                                     | `EnhancedGameObject` | N.A.             | Yes              |
| `usesSpecialAnimation`     | If the object is animated.                                                                | `EnhancedGameObject` | N.A.             | Yes              |