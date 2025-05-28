#ifndef LIVERMORESDK_H
#define LIVERMORESDK_H

#include <stddef.h>

// export/import dll
#if defined(_MSC_VER) //  Microsoft
#define LIVERMORE_SDK_API __declspec(dllexport)
#elif defined(__GNUC__) //  GCC
#define LIVERMORE_SDK_API __attribute__((visibility("default")))
#else // Warnning
#define LIVERMORE_SDK_API
#pragma WARNING UNKNOWN DYNAMIC LINK IMPORT/EXPORT SEMANTICS.
#endif

// export/import c style dll
#ifdef __cplusplus
#define LIVERMORE_SDK_C_API extern "C" LIVERMORE_SDK_API
#else
#define LIVERMORE_SDK_C_API LIVERMORE_SDK_API
#endif

#pragma pack(push, 8)
typedef struct err
{
    unsigned long code;
    const char* desc;
} err;
#pragma pack(pop)

#pragma pack(push, 8)
typedef struct context
{
    unsigned long sz = sizeof(context);
    void* user_data = NULL;
    void (*cb)(void*) = NULL;
} context;
#pragma pack(pop)

#pragma pack(push, 8)
typedef struct param_init_sdk
{
    err result;
    char* version = NULL;
} param_init_sdk;
#pragma pack(pop)

#pragma pack(push, 8)
typedef struct param_dial_broker
{
    err result;
    char ip[16] = {0}; // 255.255.255.255\n
    unsigned long port = 0;
} param_dial_broker;
#pragma pack(pop)

#pragma pack(push, 8)
typedef struct param_close_broker
{
    err result;
    char ip[16] = { 0 }; // 255.255.255.255\n
    unsigned long port = 0;
} param_close_broker;
#pragma pack(pop)

#pragma pack(push, 8)
typedef struct param_sub_market_data
{
    err result;
    char topics[20][31]; // [[sz002030], ...]
    unsigned long sz = 0;
} param_sub_market_data;
#pragma pack(pop)

#pragma pack(push, 8)
typedef struct param_quit
{
    err result;
} param_quit;
#pragma pack(pop)

typedef void(*sdk_callback)(void*);
typedef void(*sdk_api)(context);

static const err ok{ 0, "succ" };
static const err err_fail{ 1, "fail" };
static const err err_dial_timeout{ 1, "dial timeout" };
static const err err_broker_already_connected{ 1, "broker already connected" };
static const err err_broker_not_connected{ 1, "broker not connected" };

// ------------------------------------- API DEFINE --------------------------------
LIVERMORE_SDK_C_API void livermore_sdk_init(context ctx);

LIVERMORE_SDK_C_API void livermore_sdk_dial_broker(context ctx);

LIVERMORE_SDK_C_API void livermore_sdk_close_broker(context ctx);

LIVERMORE_SDK_C_API void livermore_sdk_sub_market_data(context ctx);

LIVERMORE_SDK_C_API void livermore_sdk_quit(context ctx);

#endif
