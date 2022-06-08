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

#ifdef PTC_DEBUG
#define PTC_ENABLE_ASSERTS
#endif


#ifdef PTC_ENABLE_ASSERTS
#define PTC_ASSERT(x, ...) {if(!(x)) {PTC_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#define PTC_CORE_ASSERT(x, ...) {if(!(x)) {PTC_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
#define PTC_ASSERT(x, ...)
#define PTC_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define PTC_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
