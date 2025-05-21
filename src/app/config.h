#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QString>
#include <QPointer>
#include <QDir>
#include <QSettings>

#include "libcpp/os/dll.h"

class Config : public QObject
{
    Q_OBJECT
public:
    Config(QObject *p = Q_NULLPTR);
    ~Config();

    static QPointer<Config> instance()
    {
        static QPointer<Config> inst(new Config());
        return inst;
    }

    inline int     logLvl()      { return m_setting.value("log_lvl", 1).toInt(); }
    inline QString logPath()     { return m_setting.value("log_path", "./log").toString(); }
    inline int     logSize()     { return m_setting.value("log_size", 2).toInt(); }
    inline int     logFileNum()  { return m_setting.value("log_file_num", 5).toInt(); }
    inline bool    logIsRotate() { return m_setting.value("log_rotate_on_open", true).toBool(); }

    inline QString dbDriver()    { return m_setting.value("db_driver", "QSQLITE").toString(); }
    inline QString dbMain()      { return m_setting.value("db_main", "tick").toString(); }
    inline QString dbPath()      { return m_setting.value("db_path", "./livermore-qt.db").toString(); }
    inline int     dbMaxConn()   { return m_setting.value("db_max_conn", 1).toInt(); }
    inline bool    dbAsyncExec() { return m_setting.value("db_async_exec", 1).toInt() == 1; }

    inline QString qmPath()      { return m_setting.value("qm_path", "./kstock.qm").toString(); }

    inline QString sdkPath()     { return m_setting.value("sdk_path", "liblivermore-sdk").toString() + DLL_EXT; }
    inline QString brokerIp()    { return m_setting.value("broker_ip", "172.16.255.130").toString(); }
    inline unsigned long brokerPort()  { return m_setting.value("broker_port", 10086).toInt(); }
private:
    QSettings m_setting;
};

#endif
