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
    err_dial_timeout,
    err_tcp_broker_already_connected,
    err_broker_not_connected,

    err_end,
};

// command define
enum cmd : unsigned long
{
    cmd_start = 0xFF,
    cmd_init_sdk,
    cmd_dial_broker,
    cmd_close_broker,
    cmd_md_ntf,
    cmd_md_sub,
    cmd_quit_sdk,

    cmd_end,
};

// interface for client
C_STYLE_EXPORT err register_cb(cmd api, void* fn);

C_STYLE_EXPORT err exec(cmd api, ...);

#endif
