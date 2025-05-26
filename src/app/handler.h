#ifndef HANDLER_H
#define HANDLER_H

#include <QObject>
#include <QPointer>
#include <QDebug>

#include "config.h"
#include "market_data.h"
#include "livermoresdk.h"

#include "libcpp/log/logger.hpp"

class Handler : public QObject
{
    Q_OBJECT
public:
    Handler(QObject *p = nullptr)
        : m_sdk{nullptr}
        , m_fnRegisterCB{nullptr}
        , m_fnExec{nullptr}
        , m_bLoaded{false}
    {}

    ~Handler()
    {
        unloadSDK();
    }

    static QPointer<Handler> instance()
    {
        static QPointer<Handler> inst(new Handler());
        return inst;
    }

    void init()
    {
        if (m_bLoaded)
            return;

        m_bLoaded = true;
        m_sdk = dll_open(Config::instance()->sdkPath().toStdString().c_str(), DLL_RTLD_NOW);
        LOG_DEBUG("load sdk with m_sdk={}, path={}", m_sdk, Config::instance()->sdkPath().toStdString());
        if (m_sdk == nullptr)
        {
            LOG_CRITICAL("FAIL TO LOAD SDK WITH PATH={}", Config::instance()->sdkPath().toStdString());
            return;
        }
        m_fnRegisterCB = (err(*)(cmd, void*))dll_get(m_sdk, "register_cb");
        m_fnExec = (err(*)(cmd, ...))dll_get(m_sdk, "exec");

        // register callback
        reg(cmd_init_sdk, &Handler::onSDKInit);
        reg(cmd_dial_broker, &Handler::onDialBroker);
        reg(cmd_close_broker, &Handler::onCloseBroker);
        reg(cmd_md_ntf, &Handler::onMdNtf);
        reg(cmd_md_sub, &Handler::onSub);
        reg(cmd_quit_sdk, &Handler::onQuitSDK);

        // init sdk
        Handler::instance()->call(cmd_init_sdk);
    }

    template<typename F>
    void reg(cmd api, F f)
    {
        if (m_fnRegisterCB == nullptr)
        {
            LOG_WARN("register not exist");
            return;
        }

        LOG_DEBUG("reg api={}", int(api));
        m_fnRegisterCB(api, reinterpret_cast<void*>(f));
    }

    template<typename... Args>
    void call(const cmd api, Args&& ... args)
    {
        if (m_fnExec == nullptr)
            return;

        m_fnExec(api, std::forward<Args>(args)...);
    }

    void quit()
    {
        call(cmd_quit_sdk);
    }

    void unloadSDK()
    {
        if (!m_bLoaded)
            return;

        LOG_DEBUG("DLL CLOSE WITH m_sdk={}", m_sdk);
        m_bLoaded = false;
        if (m_sdk != nullptr)
        {
            auto ret = dll_close(m_sdk);
            LOG_DEBUG("DLL CLOSE RET={}", ret);
        }

        LOG_DEBUG("DLL CLOSED");
        m_sdk = nullptr;
        m_fnRegisterCB = nullptr;
        m_fnExec = nullptr;
    }

private:
    static void onSDKInit(err);
    static void onDialBroker(err, char*, unsigned long);
    static void onCloseBroker(err, char*, unsigned long);
    static void onMdNtf(int num, market_data** mds);
    static void onSub(int result, int num, char** args);
    static void onQuitSDK(err);

private:
    bool m_bLoaded;
    void* m_sdk;
    err(*m_fnRegisterCB)(cmd, void*);
    err(*m_fnExec)(cmd, ...);
};

#endif
