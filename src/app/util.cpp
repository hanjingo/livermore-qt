#include "util.h"

#include <QObject>

QString errWhat(err e)
{
    switch (e)
    {
    case ok: return QObject::tr("ok");
    case err_fail: return QObject::tr("err_fail");
    case err_cmd_not_exist: return QObject::tr("err_cmd_not_exist");
    case err_dial_timeout: return QObject::tr("err_dial_timeout");
    case err_tcp_broker_already_connected: return QObject::tr("err_tcp_broker_already_connected");
    case err_broker_not_connected: return QObject::tr("err_broker_not_connected");
    default: return QObject::tr("unknow_err");
    }
}

std::string fmtMarketData(const market_data& md)
{
    std::string ret;
    return ret;
}
