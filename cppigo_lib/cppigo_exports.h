#pragma once
#ifdef CPPIGO_EXPORTS
#define CPPIGO_DLL __declspec(dllexport)
#else
#define CPPIGO_DLL __declspec(dllimport)
#endif
#pragma warning(disable : 4251)  // disable warnings about subclasses not having dll-interfaces
