#include "util.h"

#include <QObject>

QString errWhat(err e)
{
    switch (e)
    {
    case ok: return QObject::tr("ok");
    case err_fail: return QObject::tr("err_fail");
    case err_cmd_not_exist: return QObject::tr("err_cmd_not_exist");
    default: return QObject::tr("unknow_err");
    }
}

std::string fmtMarketData(const market_data& md)
{
    std::string ret;
    return ret;
}
