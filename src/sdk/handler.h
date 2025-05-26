#ifndef HANDLER_H
#define HANDLER_H

#include <QObject>
#include <QPointer>

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

    inline err reg(cmd api, void* fn)
    {
        if (api >= cmd_end)
            return err_cmd_not_exist;

        m_callbacks[api] = fn;
        return ok;
    }

    void init();
    void dialBroker(char* ip, unsigned long port);
    void closeBroker(char* ip, unsigned long port);
    void subMd(char* code);
    void quit();

private slots:
    void onMdNtf(int, market_data**);
    void onSubMdRsp(int, int, char**);

private:
    QMap<QString, Broker*> m_brokers;
    void* m_callbacks[cmd_end] = {nullptr};
};

#endif
