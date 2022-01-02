#pragma once

#ifdef ENGINE_DLL
    #define EG_API __declspec(dllexport)
#else
    #define EG_API __declspec(dllimport)
#endif // ENGINE_DLL
