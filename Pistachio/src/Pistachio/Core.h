#pragma once

#ifdef PTC_PLATFORM_WINDOWS
#ifdef  PTC_BUILD_DLL
#define PISTACHIO_API __declspec(dllexport)
#else
#define PISTACHIO_API __declspec(dllimport)
#endif
#else
#error Only support Windows!

#endif

