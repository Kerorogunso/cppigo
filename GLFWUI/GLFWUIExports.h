#pragma once
#ifdef GLFWUI_EXPORTS
#define GLFWUI_DLL __declspec(dllexport)
#else
#define GLFWUI_DLL __declspec(dllimport)
#endif
#pragma warning(disable : 4251)  // disable warnings about subclasses not having dll-interfaces
