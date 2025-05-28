#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QString>
#include <QPointer>
#include <QDir>
#include <QSettings>
#include <QCoreApplication>

#include <string>

#include "libqt/core/macro.h"
#include "libqt/log/logger.h"
#include "libqt/util/strutil.h"

class Config : public QObject
{
    Q_OBJECT
public:
    Config(QObject *p = Q_NULLPTR)
        : QObject(p)
        , m_setting(QCoreApplication::applicationDirPath() + "/conf.ini", QSettings::IniFormat)
    {
    }
    ~Config()
    {
    }

    static QPointer<Config> instance()
    {
        static QPointer<Config> inst(new Config());
        return inst;
    }

    inline LogLvl  logLvl()          { return static_cast<LogLvl>(m_setting.value("log_lvl", 1).toInt()); }
    inline QString logPath()         { return m_setting.value("log_file", "log/livermore.log").toString(); }
    inline int     logSize()         { return m_setting.value("log_size_mb", 2).toInt() * MB(1); }
    inline int     logFileNum()      { return m_setting.value("log_file_num", 5).toInt(); }
    inline bool    logIsRotate()     { return m_setting.value("log_rotate_on_open", false).toBool(); }

    inline QString dbDriver()        { return m_setting.value("db_driver", "QSQLITE").toString(); }
    inline QString dbMain()          { return m_setting.value("db_main", "tick").toString(); }
    inline QString dbPath()          { return m_setting.value("db_path", "./livermore-qt.db").toString(); }
    inline int     dbMaxConn()       { return m_setting.value("db_max_conn", 1).toInt(); }
    inline bool    dbAsyncExec()     { return m_setting.value("db_async_exec", 1).toInt() == 1; }

    inline QString qmPath()          { return m_setting.value("qm_path", "./kstock.qm").toString(); }

    inline QString sdkPath()         { return m_setting.value("sdk_path", "liblivermore-sdk").toString().append(QString(DLL_EXT)); }
    inline QString sdkVersion()      { return m_setting.value("sdk_version", "1.0.1").toString(); }
    inline QString brokerIp()        { return m_setting.value("broker_ip", "172.16.255.130").toString(); }
    inline unsigned long brokerPort(){ return m_setting.value("broker_port", 10086).toInt(); }
private:
    QSettings m_setting;
};

#endif
