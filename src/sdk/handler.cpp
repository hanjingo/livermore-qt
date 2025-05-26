#include "handler.h"

#include <stdarg.h>

#include "livermoresdk.h"
#include "market_data.h"
#include "proto.h"
#include "util.h"
#include "broker.h"

#include "libcpp/log/logger.hpp"

// export api
err register_cb(cmd api, void* fn)
{
    return Handler::instance()->reg(api, fn);
}

err exec(cmd api, ...)
{
    LOG_DEBUG("Exec api={}", int(api));
    switch(api)
    {
    case cmd_init_sdk:
    {
        Handler::instance()->init();
        break;
    }
    case cmd_dial_broker:
    {
        va_list args;
        va_start(args, 2);
        char* ip = va_arg(args, char*);
        unsigned long port = va_arg(args, unsigned long);
        Handler::instance()->dialBroker(ip, port);
        va_end(args);
        break;
    }
    case cmd_close_broker:
    {
        va_list args;
        va_start(args, 2);
        char* ip = va_arg(args, char*);
        unsigned long port = va_arg(args, unsigned long);
        Handler::instance()->closeBroker(ip, port);
        va_end(args);
        break;
    }
    case cmd_md_sub:
    {
        va_list args;
        va_start(args, 1);
        char* topic = va_arg(args, char*);
        Handler::instance()->subMd(topic);
        va_end(args);
        break;
    }
    case cmd_quit_sdk:
    {
        Handler::instance()->quit();
        break;
    }

    // default
    default: break;
    }

    return ok;
}

// ------------------------handler source-------------------------

Handler::Handler(QObject *p)
{
}

Handler::~Handler()
{
}

void Handler::init()
{
    LOG_DEBUG("SDK: init");
    if (m_callbacks[cmd_init_sdk] == nullptr)
        return;

    auto cb = (void(*)(err))(m_callbacks[cmd_init_sdk]);
    cb(ok);
}

void Handler::dialBroker(char* ip, unsigned long port)
{
    LOG_DEBUG("SDK: dial broker with ip={}, port={}", ip, port);
    auto broker = new Broker();
    auto ret = broker->dial(ip, port, 2000);
    if (ret != ok)
    {
        delete broker;
    } else {
        connect(broker, SIGNAL(mdNtf(int, market_data**)), this, SLOT(onMdNtf(int, market_data**)));
        connect(broker, SIGNAL(subMdRsp(int, int, char**)), this, SLOT(onSubMdRsp(int, int, char**)));
        m_brokers.insert(QString("%1:%2").arg(ip).arg(port), broker);
    }

    if (m_callbacks[cmd_dial_broker] == nullptr)
        return;
    auto cb = (void(*)(err, char*, unsigned long))(m_callbacks[cmd_dial_broker]);
    cb(ret, ip, port);
}

void Handler::closeBroker(char* ip, unsigned long port)
{
    LOG_DEBUG("SDK: close broker with ip={}, port={}", ip, port);
    err e = ok;
    auto addr = QString("%1:%2").arg(ip).arg(port);
    if (!m_brokers.contains(addr))
    {
        e = err_broker_not_connected;
    } else {
        auto broker = m_brokers[addr];
        broker->close();
        delete broker;
        m_brokers.remove(addr);
    }

    if (m_callbacks[cmd_close_broker] == nullptr)
        return;
    auto cb = (void(*)(err, char*, unsigned long))(m_callbacks[cmd_close_broker]);
    cb(e, ip, port);
}

void Handler::onMdNtf(int num, market_data** mds)
{
    LOG_DEBUG("SDK: onMdNtf");
    if (m_callbacks[cmd_md_ntf] == nullptr)
        return;
    auto cb = (void(*)(int, market_data**))(m_callbacks[cmd_md_ntf]);
    cb(num, mds);
}

void Handler::subMd(char* code)
{
    LOG_DEBUG("subMd with code={}", code);
    for (auto broker : m_brokers)
    {
        broker->subMd(code);
    }
}

void Handler::onSubMdRsp(int result, int num, char** topics)
{
    LOG_DEBUG("SDK: onSubMdRsp with result={}, num={}", result, num);
    if (m_callbacks[cmd_md_sub] == nullptr)
        return;
    auto cb = (void(*)(int, int, char**))(m_callbacks[cmd_md_sub]);
    cb(result, num, topics);
}

void Handler::quit()
{
    LOG_DEBUG("SDK: quit");
    for (auto broker : m_brokers)
    {
        broker->close();
        delete broker;
    }
    m_brokers.clear();

    if (m_callbacks[cmd_quit_sdk] == nullptr)
        return;
    auto cb = (void(*)(err))(m_callbacks[cmd_quit_sdk]);
    cb(ok);

    // remove registered handler
    for (auto i = 0; i < cmd_end; ++i)
        m_callbacks[i] = nullptr;

    // remove broker
    for (auto broker : m_brokers.values())
    {
        if (broker == nullptr)
            continue;

        broker->close();
        delete broker;
    }
    m_brokers.clear();
}
