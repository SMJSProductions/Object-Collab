---
title: Object API
order: 3
---

# Object API

Object-Collab exposes its internal register and all the utilities which come with it. For this reason there is the `ObjectAPI` class. This class allows you to manage registered objects using both the string and numeric IDs, and allows you to get the raw register where the registered objects are stored.

> :warning: Keep in mind that the register is by no means constant. All numeric IDs and the contents of the map are subject to change.

## Example

```cpp
#include <smjs.object-collab/include/object_collab.hpp>

using namespace geode::prelude;
using namespace object_collab::prelude;

$on_mod(Loaded) {
    std::string id = "object-id"_spr;

    // Puts the object into the register under "object-id"_spr
    ObjectAPI::registerObject(ObjectInfo::builder()
        .id(id)
        .sprite("sprite.png"_spr)
        .build());

    log::info("{} has sprite {}", id, ObjectAPI::getCustomObject(id)->getSprite());

    // Removes the object from the register and cleans it up from Geometry Dash.
    if (ObjectAPI::unregisterObject(id)) {
        log::info("{} is now removed", id);
    }
}
```