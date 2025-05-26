#include "handler.h"
#include "util.h"

#include "data.h"

#include <QVector>
#include <QString>

void Handler::onSDKInit(err e)
{
    LOG_DEBUG("onSDKInit with e={}", errWhat(e).toStdString());
    if (e != ok)
        return;

    Handler::instance()->call(cmd_dial_broker,
                              Config::instance()->brokerIp().toStdString().c_str(),
                              Config::instance()->brokerPort());
}

void Handler::onDialBroker(err e, char* ip, unsigned long port)
{
    LOG_DEBUG("onDialBroker with e={}, ip={}, port={}",
              errWhat(e).toStdString(), ip, port);
    if (e != ok)
        return;

    // // for test
    // Handler::instance()->call(cmd_md_sub, "sz002030");
}

void Handler::onCloseBroker(err e, char* ip, unsigned long port)
{
    LOG_DEBUG("onCloseBroker with e={}, ip={}, port={}",
              errWhat(e).toStdString(), ip, port);
    if (e != ok)
        return;
}

void Handler::onMdNtf(int num, market_data** mds)
{
    LOG_DEBUG("onMdNtf with num={}", num);

    // write database
    Data::instance()->onTickNtf(num, mds);
}

void Handler::onSub(int result, int num, char** args)
{
    QString topics;
    for (int i = 0; i < num; ++i)
    {
        topics.append(QString(args[i]));
        topics.append(",");
    }
    LOG_DEBUG("onSub with result={}, num={}, topics={}", result, num, topics.toStdString());
}

void Handler::onQuitSDK(err e)
{
    LOG_DEBUG("onQuitSDK with e={}", errWhat(e).toStdString());

    Handler::instance()->unloadSDK();
}
