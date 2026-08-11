---
title: Optional API
order: 3
---

# Optional API

Object-Collab comes with a small optional API to get access to the internal object register. This way you can find the details about an object and under what IDs they are registered without explicitly depending on Object-Collab.

All information provided by this method is made as minimal as possible to avoid over-reliance on Object-Collab to resolve the information.

## Example

### Mod.json

```json
{
  "dependencies": {
		"smjs.object-collab": {
      "version": ">={the latest version}",
      "required": false
    }
  }
}
```

### C++

```cpp
#include <smjs.object-collab/include/object_collab_optional.hpp>

using namespace object_collab::prelude;

void MyObjectHandling::handleCustomObjects() {
    object_collab::getOptionalRegister().listen([](OptionalRegister objectRegister) {
        for (auto& [numericID, info] : objectRegister) {
            // Do whatever you want with the object.
        }
    });
}
```