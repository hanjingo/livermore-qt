#ifndef DB_H
#define DB_H

#include <QObject>
#include <QThreadPool>
#include <QReadWriteLock>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QQueue>
#include <functional>
#include <QPointer>

class DBPool : public QObject
{
    Q_OBJECT
public:
    using CreateConn = std::function<QSqlDatabase()>;

public:
    DBPool(QObject* p = Q_NULLPTR);
    DBPool(CreateConn&& fn, QObject* p = Q_NULLPTR);
    ~DBPool();

    static QPointer<DBPool> instance()
    {
        static QPointer<DBPool> inst(new DBPool());
        return inst;
    }

    bool empty();
    bool full();
    void setCreateConnFn(CreateConn&& fn);
    qint64 getMaxConn();
    void setMaxConn(const qint64 sz);
    QSqlDatabase getConn();
    void releaseConn(QSqlDatabase& db);

private:
    QReadWriteLock       m_rwlock;
    qint64               m_maxConn;
    QQueue<QSqlDatabase> m_conns;
    CreateConn           m_createConn;
};

#endif
