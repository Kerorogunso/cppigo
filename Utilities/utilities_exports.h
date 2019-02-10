#pragma once
#ifdef UTILITIES_EXPORTS
#define UTILITIES_DLL __declspec(dllexport)
#else
#define UTILITIES_DLL __declspec(dllimport)
#endif
#pragma warning(disable : 4251)  // disable warnings about subclasses not having dll-interfaces
