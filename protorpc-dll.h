#pragma once

#ifdef __cplusplus
#define PROTORPC_C extern "C"
#else
#define PROTORPC_C extern
#endif

#if 0 // disabling DLL
#ifndef WIN32
#define PROTORPC_API PROTORPC_C __attribute__((visibility ("default")))
#elif defined BUILDING_PROTORPC
#define PROTORPC_API PROTORPC_C __declspec(dllexport)
#else
#define PROTORPC_API PROTORPC_C __declspec(dllimport)
#endif
#else
#define PROTORPC_API PROTORPC_C
#endif
