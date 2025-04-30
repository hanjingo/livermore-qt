#ifndef HANDLER_H
#define HANDLER_H

#include <QObject>
#include <QPointer>

#include "livermoresdk.h"

// ------------------------ cmd callback ----------------------------
void onSDKInit(err);


// ------------------------ handler obj ----------------------------
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

    void init();

private:
    template<typename F>
    void reg(cmd, F);

signals:
    void sigInitSDK();

private slots:
    void initSDK();

private:
    bool m_bInit;
    err(*m_fnRegisterCB)(cmd, void*);
    err(*m_fnExec)(cmd, ...);
};

#endif
