#ifndef HANDLER_H
#define HANDLER_H

#include <QObject>
#include <QPointer>
#include <QDebug>
#include <QLibrary>

#include "config.h"
#include "market_data.h"

#include "libqt/core/macro.h"
#include "libqt/log/logger.h"
#include "libqt/util/strutil.h"
#include "libqt/core/objpool.h"

extern "C" {
#include "livermoresdk.h"
}

enum cmd : int
{
    cmd_init = 0,
    cmd_dial_broker,
    cmd_close_broker,
    cmd_market_data_ntf,
    cmd_sub_market_data,
    cmd_quit,

    cmd_end,
};

class Handler : public QLibrary
{
    Q_OBJECT

public:
    Handler(QObject* p = nullptr)
        : QLibrary(p)
    {
    }

    ~Handler()
    {
        unload();
    }

    static QPointer<Handler> instance()
    {
        static QPointer<Handler> inst(new Handler());
        return inst;
    }

    bool load(const QString& path);
    bool unload();
    void call(const cmd api, sdk_callback cb, void* params);

    void init(const char* version);
    void dialBroker(const QString& ip, int port);
    void closeBroker(const QString& ip, int port);
    void subMarketData(const QStringList& topics);
    void quit();

protected:
    static void cbInitSdk(void*);
    static void cbDialBroker(void*);
    static void cbCloseBroker(void*);
    static void cbSubMarketData(void*);
    static void cbQuit(void*);

//signals:
//    void initSdkDone();
//    void mdNtf(int num, market_data** mds);

private:
    sdk_api m_cmds[cmd_end] = {nullptr};
};

#endif
