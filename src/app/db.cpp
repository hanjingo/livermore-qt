#include "db.h"
#include "Config.h"
#include "libcpp/log/logger.hpp"

static std::once_flag once;
static QPointer<DB> db_inst;

DB::DB(std::size_t connNum, QThreadPool* threads, QObject* p)
    : QObject(p)
    , m_threads{threads}
{
    for (m_id = 0; m_id < connNum; ++m_id)
    {
        QSqlDatabase db = QSqlDatabase::addDatabase(Config::instance()->dbDriver(), QString::number(m_id));
        db.setDatabaseName(Config::instance()->dbPath());
        if (!db.open())
            continue;

        m_conns.push_back(db);
    }
}

DB::~DB()
{
    m_cond.notify_all();
    while (!m_conns.isEmpty())
    {
        auto conn = m_conns.dequeue();
        if (conn.isOpen())
            conn.close();
    }
}

QPointer<DB> DB::instance()
{
    std::call_once(once, [](){ db_inst = new DB(Config::instance()->dbMaxConn()); });
    return db_inst;
}

void DB::exec(const QString& sql, FQuery fn)
{
    LOG_INFO("Exec {}", sql.toStdString());
    auto db = acquire();
    if (!db.isOpen())
    {
        LOG_WARN("Open database fail");
        emit sigDBLastError("Database conn is invalid");
        return;
    }

    if (m_threads == nullptr)
    {
        QSqlQuery query{db};
        if (!query.exec(sql))
        {
            LOG_WARN("Exec {} fail", sql.toStdString());
            emit sigDBLastError(query.lastError().text());
        }

        fn(query);
        giveback(db);
        return;
    }

    m_threads->start([this, db, sql, fn](){
        QSqlQuery query{db};
        if (!query.exec(sql))
        {
            LOG_WARN("Exec {} fail", sql.toStdString());
            emit sigDBLastError(query.lastError().text());
        }

        fn(query);
        this->giveback(db);
    });
}

void DB::setThreadPool(QThreadPool* threads)
{
    std::lock_guard<std::mutex> lock{m_lock};
    m_threads = threads;
}

QSqlDatabase DB::acquire(const int timeout_ms)
{
    if (timeout_ms > 0 && m_conns.isEmpty())
    {
        std::unique_lock<std::mutex> lock{m_lock};
        bool ok = m_cond.wait_for(
            lock,
            std::chrono::milliseconds(timeout_ms),
            [this]{ return !this->m_conns.empty(); });
        if (!ok)
            return QSqlDatabase();
    }

    std::lock_guard<std::mutex> lock{m_lock};
    if (m_conns.isEmpty())
        return QSqlDatabase();

    do {
        auto conn = m_conns.dequeue();
        if (conn.isOpen())
            return conn;

        QSqlDatabase::removeDatabase(conn.connectionName());
        m_id++;
        QSqlDatabase::addDatabase(Config::instance()->dbDriver(), QString::number(m_id));
    } while (true);
}

void DB::giveback(const QSqlDatabase& db)
{
    std::lock_guard<std::mutex> lock{m_lock};
    m_conns.enqueue(db);
    m_cond.notify_one();
}
