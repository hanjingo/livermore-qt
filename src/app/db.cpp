#include "db.h"

#include <QtSql/QSqlError>
#include <QDir>

#include "Config.h"

DBPool::DBPool(QObject* p)
    : QObject(p)
    , m_rwlock{}
    , m_maxConn{0}
    , m_createConn{[]() -> QSqlDatabase { return QSqlDatabase(); }}
{
}

DBPool::DBPool(CreateConn&& fn, QObject* p)
    : QObject(p)
    , m_rwlock{}
    , m_maxConn{0}
    , m_createConn{std::move(fn)}
{
}

DBPool::~DBPool()
{
}

bool DBPool::empty()
{
    m_rwlock.lockForRead();
    bool ret = m_conns.empty();
    m_rwlock.unlock();
    return ret;
}

bool DBPool::full()
{
    m_rwlock.lockForRead();
    bool ret = (m_conns.size() == m_maxConn);
    m_rwlock.unlock();
    return ret;
}

void DBPool::setCreateConnFn(CreateConn&& fn)
{
    m_rwlock.lockForWrite();
    m_createConn = std::move(fn);
    m_rwlock.unlock();
}

qint64 DBPool::getMaxConn()
{
    m_rwlock.lockForRead();
    qint64 ret = m_maxConn;
    m_rwlock.unlock();
    return ret;
}

void DBPool::setMaxConn(const qint64 sz)
{
    m_rwlock.lockForWrite();
    while (m_conns.size() < sz)
        m_conns.enqueue(m_createConn());
    m_rwlock.unlock();
}

QSqlDatabase DBPool::getConn()
{
    if (empty())
        return QSqlDatabase();

    m_rwlock.lockForWrite();
    auto db = m_conns.dequeue();
    if (!db.isOpen())
        db.open();

    m_rwlock.unlock();
    return db;
}

void DBPool::releaseConn(QSqlDatabase& db)
{
    if (full())
        return;

    m_rwlock.lockForWrite();
    if (!db.isOpen())
        db.open();
    if (db.isValid())
        m_conns.push_back(db);
    m_rwlock.unlock();
}
