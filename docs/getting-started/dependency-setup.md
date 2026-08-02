---
title: 1. Dependency Setup
order: 1
---

# Dependency Setup

## Mod.json

Add the following in your `mod.json`:

```json
{
  "dependencies": {
    "smjs.object-collab": {
      "version": ">={the latest version}",
      "required": true
    }
  }
}
```

## C++

Add the following to your code:

```cpp
#include <smjs.object-collab/include/object_collab.hpp>

// ::prelude is optional. It just makes popups simpler to setup.
using namespace object_collab::prelude;
```