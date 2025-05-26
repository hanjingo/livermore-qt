#ifndef BROKER_H
#define BROKER_H

#include <QObject>
#include <QPointer>
#include <QString>

#include "livermoresdk.h"
#include "proto.h"
#include "market_data.h"
#include "libqt/net/tcp/tcpclient.h"
#include "libqt/core/objpool.h"

class Broker : public QObject
{
    Q_OBJECT

public:
    enum msg_id : unsigned long
    {
        // user defined
        msg_id_start = 0x0,

        msg_id_md_ntf,
        msg_id_md_sub_req,
        msg_id_md_sub_rsp,
        msg_id_md_unsub_req,
        msg_id_md_unsub_rsp,

        msg_id_end = 0xFF,
    };

public:
    Broker(QObject *p = Q_NULLPTR);
    ~Broker();

    err dial(const QString& ip, const quint16 port, int ms);
    err close();
    err subMd(char* topic);

private slots:
    void onReadyRead();

signals:
    void mdNtf(int, market_data**);
    void subMdRsp(int, int, char**);

private:
    JsonMsg* constructMsg();

private:
    ObjPool<JsonMsg*>     m_objs;
    ObjPool<market_data*> m_mds;
    TcpClient*            m_cli;
};

#endif
