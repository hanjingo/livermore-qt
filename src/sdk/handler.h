#ifndef HANDLER_H
#define HANDLER_H

#include <QObject>
#include <QPointer>
#include <QString>
#include <QStringList>

#include "broker.h"
#include "livermoresdk.h"

class Handler : public QObject
{
    Q_OBJECT

public:
    Handler(QObject *p = Q_NULLPTR);
    ~Handler();

    static QPointer<Handler> instance()
    {
        static QPointer<Handler> inst(new Handler());
        return inst;
    }

    err init();
    err dialBroker(const QString& ip, const unsigned long port);
    err closeBroker(const QString& ip, const unsigned long port);
    err subMarketData(const QStringList& code);
    err quit();

private slots:
    void onMdNtf(int, market_data**);
    void onSubMdRsp(int, int, char**);

private:
    QMap<QString, Broker*> m_brokers;
};

#endif
