#include "handler.h"
#include "config.h"
#include "util.h"

#include "libcpp/log/logger.hpp"
#include "libcpp/os/env.h"
#include "libcpp/util/once.hpp"

// void regCallback()
// {
//     RegisterCB(cmd_init, reinterpret_cast<void*>(onInit));
// }

void onSDKInit(err e)
{
    LOG_DEBUG("on sdk init ret={}", errWhat(e).toStdString());
}


// ------------------------ handler obj ----------------------------
Handler::Handler(QObject *p)
    : m_fnRegisterCB{nullptr}
    , m_fnExec{nullptr}
    , m_bInit{false}
{
}

Handler::~Handler()
{}

void Handler::init()
{
    if (m_bInit)
        return;

    m_bInit = true;
    LOG_DEBUG("load sdk with path={}", Config::instance()->sdkPath().toStdString());
    auto sdk = dll_open(Config::instance()->sdkPath().toStdString().c_str(), DLL_RTLD_NOW);
    if (sdk == nullptr)
    {
        LOG_CRITICAL("FAIL TO LOAD SDK WITH PATH={}", Config::instance()->sdkPath().toStdString());
        return;
    }
    m_fnRegisterCB = (err(*)(cmd, void*))dll_get(sdk, "register_cb");
    m_fnExec = (err(*)(cmd, ...))dll_get(sdk, "exec");

    // register callback
    Handler::instance()->reg(cmd_init_sdk, onSDKInit);

    // bind connect
    connect(this, SIGNAL(Handler::sigInitSDK()), this, SLOT(Handler::initSDK()));
}

template<typename F>
void Handler::reg(cmd api, F f)
{
    m_fnRegisterCB(api, reinterpret_cast<void*>(f));
}

void Handler::initSDK()
{
    m_fnExec(cmd_init_sdk);
}
