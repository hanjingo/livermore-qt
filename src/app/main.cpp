#include <QApplication>
#include <QTranslator>

#include "config.h"
#include "data.h"
#include "handler.h"
#include "mainwindow.h"

#include "libcpp/log/logger.hpp"
#include "libcpp/os/env.h"

#include "libqt/db/dbconnpool.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // init log
    libcpp::logger::instance()->add_sink(
        libcpp::logger::create_rotate_file_sink(
            Config::instance()->logPath().toStdString(),
            Config::instance()->logSize(),
            Config::instance()->logFileNum(),
            Config::instance()->logIsRotate()));
    libcpp::logger::instance()->set_level(static_cast<libcpp::log_lvl>(Config::instance()->logLvl()));

    // translator
    QTranslator translator;
    translator.load(Config::instance()->qmPath());
    a.installTranslator(&translator);

    // add water mark
    LOG_INFO("livermore-qt");
    LOG_INFO("livermore-qt {}.{}.{}", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
    LOG_INFO("livermore-qt compile time {}", COMPILE_TIME);
    LOG_INFO("livermore-qt email {}", "hehehunanchina@live.com");

    // init database
    DBConnPool::instance()->setFactoryFn([]()->QSqlDatabase{
        auto db = QSqlDatabase::addDatabase(Config::instance()->dbDriver(), Data::dbID());
        db.setDatabaseName(Config::instance()->dbPath());
        if (!db.open())
            LOG_WARN("FAIL TO CREATE DB CONN");
        else
            LOG_INFO("create db conn succ");
        return db;
    });
    DBConnPool::instance()->setConnNum(Config::instance()->dbMaxConn());
    if (Config::instance()->dbAsyncExec())
        DBConnPool::instance()->setThreadPool(QThreadPool::globalInstance());
    QString sql(
R"(CREATE TABLE IF NOT EXISTS "tick" (
  "action_time" text,
  "action_ms" NUMBER,
  "trading_day" date,
  "name" TEXT,
  "code" TEXT,
  "open_price" NUMBER,
  "pre_close_price" NUMBER,
  "price" NUMBER,
  "high_price" NUMBER,
  "low_price" NUMBER,
  "volume" NUMBER,
  "amount" NUMBER,
  "bid_volume1" NUMBER,
  "bid_price1" NUMBER,
  "bid_volume2" NUMBER,
  "bid_price2" NUMBER,
  "bid_volume3" NUMBER,
  "bid_price3" NUMBER,
  "bid_volume4" NUMBER,
  "bid_price4" NUMBER,
  "bid_volume5" NUMBER,
  "bid_price5" NUMBER,
  "ask_volume1" NUMBER,
  "ask_price1" NUMBER,
  "ask_volume2" NUMBER,
  "ask_price2" NUMBER,
  "ask_volume3" NUMBER,
  "ask_price3" NUMBER,
  "ask_volume4" NUMBER,
  "ask_price4" NUMBER,
  "ask_volume5" NUMBER,
  "ask_price5" NUMBER
);)");
    DBConnPool::instance()->exec(sql, [sql](QSqlQuery& query){
        auto err = query.lastError();
        if(!err.text().isEmpty())
            LOG_ERROR("fail to init db with err = {}, sql={}",
                        err.text().toStdString(), sql.toStdString());
    });

    // show main window
    MainWindow w;
    w.show();

    // repaint kvolumegrid
    Data::instance()->load(QDateTime::fromString("20250421", "yyyyMMdd"));

    // // load sdk
    // Handler::instance()->init();
    // emit Handler::instance()->sigInitSDK();
    
    LOG_INFO("livermore-qt running");
    return a.exec();
}
