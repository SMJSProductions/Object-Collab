---
title: ID Swapping
order: 2
---

# ID Swapping

Sometimes object ID dependent behavior is too hardcoded deep inside a method. For this Object-Collab provides a utility class called `ObjectIDSwap`. This allows you to swap the ID of a given object while the swap instance is alive, somewhat similar to how [std::unique_lock](https://en.cppreference.com/cpp/thread/unique_lock) works. After the swap instance is destroyed the ID gets set back to the original, ensuring correct behavior.

> :warning: Do not swap object IDs without checking what you will trigger. GD has a lot of blind state/type assumptions & side effects which can produce undefined behavior.

## Example

```cpp
#include <smjs.object-collab/include/object_collab.hpp>

using namespace object_collab::prelude;

void MyCustomObject::activatedByPlayer(PlayerObject* player) {
    // Put the swap in a scope to kill it as soon as it's no longer needed.
    {
        ObjectIDSwap swap(this, 99);

        this->playShineEffect();
        // Swap gets killed here and sets the object ID back to the original.
    }

    // Other behavior.
}
```