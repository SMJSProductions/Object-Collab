---
title: 2. A Quick Object
order: 2
---

# A Quick Object

If you want a quick object without any extra features you can create one by doing something comparable to the following example:

```cpp
#include <Geode/Geode.hpp>
#include <smjs.object-collab/include/object_collab.hpp>

using namespace object_collab::prelude;

$on_mod(Loaded) {
    ObjectAPI::registerObject(ObjectInfo::builder()
        .id("id"_spr)
        .sprite("spriteframe.png"_spr)
        .construction(QuickObject::builder() // Optional
            .objectType(GameObjectType::Solid) // Optional
            .defaultZLayer(ZLayer::Default) // Optional
            .defaultZOrder(2) // Optional
            .build())
        .editorTab(EditorTab::Solids) // Optional
        .build());
}
```

## What Does The Code Do?

### Registration

The object is being registered using `"id"_spr`. Internally the string is being associated with a numeric object ID compatible with how GD handles object identification. Whenever you save the level the numeric IDs get optimized to whatever is in the level and get linked to the string ID provided here. For this reason you must keep your IDs the same to allow loading old level data. The identifier must also be suffixed by `_spr` to make sure that IDs between mods never overlap.

In this setup you also define some variables used to initialize the object and position it inside the editor UI.

> :warning: Sprites must always be part of a spritesheet.