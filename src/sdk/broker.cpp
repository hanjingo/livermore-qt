#include "broker.h"

#include <functional>
#include <QJsonArray>

Broker::Broker(QObject *p)
    : m_objs{1024}
    , m_cli{new TcpClient()}
{
    for (int i = 0; i < m_objs.capa(); ++i)
    {
        m_objs.giveback(new JsonMsg());
    }
}

Broker::~Broker()
{
    JsonMsg* msg = nullptr;
    while (m_objs.tryAcquire(msg))
    {
        delete msg;
    }
}

err Broker::dial(const QString& ip, const quint16 port, int ms)
{
    if (m_cli->isConnected())
        return err_tcp_broker_already_connected;

    m_cli->dial(ip, port, ms, std::bind(&Broker::constructMsg, this));
    if (!m_cli->isConnected())
        return err_dial_timeout;

    connect(m_cli, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    return ok;
}

err Broker::close()
{
    if (!m_cli->isConnected())
        return err_broker_not_connected;

    m_cli->close();
}

err Broker::subMd(char* topic)
{
    auto msg = m_objs.acquire();
    msg->id = msg_id_md_sub_req;
    QJsonObject obj;
    QJsonArray arr;
    arr.append(QString(topic));
    obj.insert("topics", arr);
    msg->payload = obj;
    m_cli->writeMsg(msg);
}

JsonMsg* Broker::constructMsg()
{
    return m_objs.acquire();
}

void Broker::onReadyRead()
{
    auto msg = m_cli->readMsg();
    if (msg == nullptr)
        return;

    JsonMsg* js = static_cast<JsonMsg*>(msg);
    switch (js->id)
    {
    case msg_id_md_ntf: {
        market_data* md = new market_data();
        json_to_md(js->payload, md);
        market_data* mds[1] = {md};
        emit this->mdNtf(1, mds);
        break;
    }
    case msg_id_md_sub_rsp: {
        int result = js->payload["result"].toInt();
        QJsonArray arr = js->payload["topics"].toArray();
        char* topics[arr.count()];
        for (int i = 0; i < arr.count(); ++i)
        {
            char* str;
            strcpy(str, arr[i].toString().toStdString().c_str());
            topics[i] = str;
        }
        emit this->subMdRsp(result, arr.count(), topics);
        break;
    }
    case msg_id_md_unsub_rsp: {
        break;
    }
    default: {}
    }

    js->reset();
    m_objs.giveback(std::move(js));
}
