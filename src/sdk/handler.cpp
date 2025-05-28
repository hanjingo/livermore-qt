#include "handler.h"

#include <stdarg.h>

#include <QDebug>
#include <QString>

#include "market_data.h"
#include "proto.h"
#include "util.h"
#include "broker.h"

Handler::Handler(QObject *p)
{
}

Handler::~Handler()
{
}

err Handler::init()
{
    qDebug() << "SDK: init";
    return ok;
}

err Handler::dialBroker(const QString& ip, const unsigned long port)
{
    qDebug() << "SDK: dial broker with ip=" << ip << ", port=" << port;
    auto broker = new Broker();
    auto ret = broker->dial(ip, port, 2000);
    if (ret.code != ok.code)
    {
        delete broker;
        return ret;
    } 
    
    connect(broker, SIGNAL(mdNtf(int, market_data**)), this, SLOT(onMdNtf(int, market_data**)));
    connect(broker, SIGNAL(subMdRsp(int, int, char**)), this, SLOT(onSubMdRsp(int, int, char**)));
    m_brokers.insert(QString("%1:%2").arg(ip).arg(port), broker);
    return ret;
}

err Handler::closeBroker(const QString& ip, const unsigned long port)
{
    qDebug() << "SDK: close broker with ip=" << ip << ", port=" << port;
    auto addr = QString("%1:%2").arg(ip).arg(port);
    if (!m_brokers.contains(addr))
    {
        return err_broker_not_connected;
    }
       
    auto broker = m_brokers[addr];
    m_brokers.remove(addr);
    broker->close();
    delete broker;
    return ok;
}

void Handler::onMdNtf(int num, market_data** mds)
{
    qDebug() << "SDK: onMdNtf";
    //if (m_callbacks[cmd_md_ntf] == nullptr)
    //    return;
    //auto cb = (void(*)(int, market_data**))(m_callbacks[cmd_md_ntf]);
    //cb(num, mds);
}

err Handler::subMarketData(const QStringList& code)
{
    qDebug() << "subMarketDatas with code=" << code;
    for (auto broker : m_brokers)
    {
        //broker->subMarketData(code);
    }

    return ok;
}

void Handler::onSubMdRsp(int result, int num, char** topics)
{
    qDebug() << "SDK: onSubMdRsp with result=" << result << ", num=" << num;
    //if (m_callbacks[cmd_md_sub] == nullptr)
    //    return;
    //auto cb = (void(*)(int, int, char**))(m_callbacks[cmd_md_sub]);
    //cb(result, num, topics);
}

err Handler::quit()
{
    qDebug() << "SDK: quit";

    // remove broker
    for (auto broker : m_brokers.values())
    {
        if (broker == nullptr)
            continue;

        broker->close();
        delete broker;
    }
    m_brokers.clear();
    return ok;
}
