#ifndef LIVERMORESDK_H
#define LIVERMORESDK_H

#include <stdarg.h>

#include "libcpp/os/dll.h"

// error code define
enum err : unsigned long
{
    ok = 0,
    err_fail,
    err_cmd_not_exist,

    err_end,
};

// command define
enum cmd : unsigned long
{
    cmd_init_sdk = 0,

    cmd_end,
};

// glob callback array
static void* callbacks[cmd_end];

// define api callback function style
typedef void (* cb_init)(err);


// interface for client
C_STYLE_EXPORT err register_cb(cmd api, void* fn);

C_STYLE_EXPORT err exec(cmd api, ...);

#endif
