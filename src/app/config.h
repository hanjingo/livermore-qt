#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QString>
#include <QPointer>
#include <QDir>
#include <QSettings>

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

    inline QString dbDriver()    { return m_setting.value("db_driver", "QSQLITE").toString(); }
    inline QString dbMain()      { return m_setting.value("db_main", "tick").toString(); }
    inline QString dbPath()      { return m_setting.value("db_path", "./livermore-qt.db").toString(); }
    inline int     dbMaxConn()   { return m_setting.value("db_max_conn", 1).toInt(); }
    inline bool    dbAsyncExec() { return m_setting.value("db_async_exec", 1).toInt() == 1; }

    inline QString qmPath()      { return m_setting.value("qm_path", "./kstock.qm").toString(); }
private:
    QSettings m_setting;
};

#endif
