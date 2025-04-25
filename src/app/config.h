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

    inline QString dbType()    { return m_setting.value("db_type", "QSQLITE").toString(); }
    inline QString dbMain()    { return m_setting.value("db_main", "md").toString(); }
    inline QString dbPath()    { return m_setting.value("db_path", "./app").toString(); }
    inline qint64  dbMaxConn() { return m_setting.value("db_max_conn", 1).toInt(); }

    inline QString qmPath() { return m_setting.value("qm_path", "./kstock.qm").toString(); }
private:
    QSettings m_setting;
};

#endif
