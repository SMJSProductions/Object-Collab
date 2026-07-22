#pragma once

#ifdef GEODE_IS_WINDOWS
    #ifdef Object_Collab_EXPORTS
        #define OBJC_API_DLL __declspec(dllexport)
    #else
        #define OBJC_API_DLL __declspec(dllimport)
    #endif
#else
    #define OBJC_API_DLL __attribute__((visibility("default")))
#endif