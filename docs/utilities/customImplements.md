---
title: Custom Implements
order: 1
---

# Custom Implements

Object-Collab provides a definition to simplify the implementation of custom behavior for any custom object. To use it you need to call `CUSTOM_OBJECT_IMPLEMENT` with the args:

- `sourceVar`: The variable providing the `GameObject*`.
- `type`: The type to `geode::cast::typeinfo_cast` to.
- `customVar`: The variable name to define and store the casted object into.
- `...`: The custom implementation. If it's multi line, it needs `{}` around the implementation.

## Example

```cpp
#include <Geode/modify/PlayLayer.hpp>
#include <smjs.object-collab/include/object_collab.hpp>

class $modify(PlayLayer) {
    $override void screenFlipObject(GameObject* object) {
        CUSTOM_OBJECT_IMPLEMENT(object, MyCustomObject, customObject, {
            customObject->customScreenFlip();

            return;
        });

        PlayLayer::screenFlipObject(object);
    }
};
```