#ifndef DB_H
#define DB_H

#include <functional>
#include <condition_variable>
#include <mutex>

#include <QQueue>
#include <QObject>
#include <QPointer>
#include <QThreadPool>
#include <QReadWriteLock>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

class DB : public QObject
{
    Q_OBJECT

public:
    using FQuery = std::function<void(QSqlQuery&)>;

public:
    DB(std::size_t connNum, QThreadPool* threads = nullptr, QObject* p = Q_NULLPTR);
    ~DB();

    static QPointer<DB> instance();

    void setThreadPool(QThreadPool* threads);
    void exec(const QString& sql, FQuery fn);

    QSqlDatabase acquire(const int timeout_ms = 0);
    void giveback(const QSqlDatabase& db);

signals:
    void sigDBLastError(QString err);

private:
    std::mutex                         m_lock;
    QQueue<QSqlDatabase>               m_conns;
    QThreadPool*                       m_threads;
    std::condition_variable            m_cond;
    qint64                             m_id;
};

#endif
