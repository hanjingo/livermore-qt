#include "livermoresdk.h"
#include "market_data.h"

err init()
{
    return ok;
}

// -------------------------------------------------

err register_cb(cmd api, void* fn)
{
    if (api >= cmd_end)
        return err_cmd_not_exist;

    callbacks[api] = fn;
    return ok;
}

err exec(cmd api, ...)
{
    switch(api)
    {
    case cmd_init_sdk: { auto fn = (cb_init)(callbacks[api]); fn(init()); break; }

    // default
    default: break;
    }

    return ok;
}
