#include "handler.h"
#include "util.h"

#include "data.h"

#include "libqt/util/strutil.h"

#include <QVector>
#include <QString>

bool Handler::load(const QString& path)
{
    if (isLoaded())
        return false;

    QLibrary::setFileName(path);
    if (!QLibrary::load())
        return false;
    qDebug() << "load sdk succ with path=" << fileName();

    // register api
    m_cmds[cmd_init] = (sdk_api)(resolve("livermore_sdk_init"));
    m_cmds[cmd_dial_broker] = (sdk_api)(resolve("livermore_sdk_dial_broker"));
    m_cmds[cmd_close_broker] = (sdk_api)(resolve("livermore_sdk_close_broker"));
    m_cmds[cmd_sub_market_data] = (sdk_api)(resolve("livermore_sdk_sub_market_data"));
    m_cmds[cmd_quit] = (sdk_api)(resolve("livermore_sdk_quit"));
    return true;
}

bool Handler::unload()
{
    if (!isLoaded())
        return false;

    QLibrary::unload();
    qDebug() << "DLL CLOSED with path=" << fileName();
}

void Handler::call(const cmd api, sdk_callback cb, void* params)
{
    auto fn = m_cmds[api];
    if (fn == nullptr)
    {
        qCritical() << "FAIL TO GET CMD=" << api;
        return;
    }

    context ctx;
    ctx.user_data = params;
    ctx.cb = cb;
    fn(ctx);
}

void Handler::init(const char* version)
{
    param_init_sdk param;
    param.version = const_cast<char*>(version);
    call(cmd_init, &Handler::cbInitSdk, static_cast<void*>(&param));  
}

void Handler::cbInitSdk(void* userData)
{
    auto param = static_cast<param_init_sdk*>(userData);
    qDebug() << "cbInitSdk with result=" << param->result.desc;
    if (param->result.code != ok.code)
        return;

    Handler::instance()->dialBroker(Config::instance()->brokerIp(), Config::instance()->brokerPort());
}

void Handler::dialBroker(const QString& ip, int port)
{
    param_dial_broker param;
    StrUtil::strncpy(param.ip, ip, 16);
    param.port = port;
    call(cmd_dial_broker, &Handler::cbDialBroker, static_cast<void*>(&param));
}

void Handler::cbDialBroker(void* userData)
{
    auto param = static_cast<param_dial_broker*>(userData);
    qDebug() << "cbDialBroker with result=" << param->result.desc;
    if (param->result.code != ok.code)
        return;

}

void Handler::closeBroker(const QString& ip, int port)
{
    param_close_broker param;
    StrUtil::strncpy(param.ip, ip, 16);
    param.port = port;
    call(cmd_close_broker, &Handler::cbCloseBroker, static_cast<void*>(&param));
}

void Handler::cbCloseBroker(void* userData)
{
    auto param = static_cast<param_close_broker*>(userData);
    qDebug() << "cbCloseBroker with result=" << param->result.desc;
    if (param->result.code != ok.code)
        return;


}
//
//void Handler::onMdNtf(int num, market_data** mds)
//{
//    qDebug() << "onMdNtf with num=" << num;
//
//    // write database
//    Data::instance()->onTickNtf(num, mds);
//}
//

void Handler::subMarketData(const QStringList& topics)
{
    param_sub_market_data param;
    call(cmd_sub_market_data, &Handler::cbSubMarketData, static_cast<void*>(&param));
}

void Handler::cbSubMarketData(void* userData)
{
    auto param = static_cast<param_sub_market_data*>(userData);
    qDebug() << "cbSubMarketData with result=" << param->result.desc;
    if (param->result.code != ok.code)
        return;
}

void Handler::quit()
{
    param_quit param;
    call(cmd_quit, &Handler::cbQuit, static_cast<void*>(&param));
}

void Handler::cbQuit(void* userData)
{
    auto param = static_cast<param_quit*>(userData);
    qDebug() << "cbQuit with result=" << param->result.desc;
    if (param->result.code != ok.code)
        return;
}
