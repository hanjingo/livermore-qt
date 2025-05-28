#include <QApplication>
#include <QTranslator>

#include "config.h"
#include "data.h"
#include "handler.h"
#include "mainwindow.h"

#include "libqt/log/logger.h"
#include "libqt/core/macro.h"

#include "libqt/db/dbconnpool.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // init log
    Logger::instance()->addSink(
       Logger::createRotateFileSink(
           Config::instance()->logPath(),
           Config::instance()->logSize(),
           Config::instance()->logFileNum(),
           Config::instance()->logIsRotate()));
    Logger::instance()->setLevel(Config::instance()->logLvl());
    Logger::catchQtMsg();
    qInfo() << "main function enter";

    // translator
    QTranslator translator;
    translator.load(Config::instance()->qmPath());
    a.installTranslator(&translator);

    // add water mark
    qInfo() << "livermore-qt ";
    qInfo() << "livermore-qt " << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH;
    qInfo() << "livermore-qt compile time " << COMPILE_TIME;
    qInfo() << "livermore-qt email " << "hehehunanchina@live.com";

    // show main window
    MainWindow w;
    w.show();

    // init database
    DBConnPool::instance()->setFactoryFn([]()->QSqlDatabase{
        auto db = QSqlDatabase::addDatabase(Config::instance()->dbDriver(), Data::dbID());
        db.setDatabaseName(Config::instance()->dbPath());
        if (!db.open())
            qWarning() << "FAIL TO CREATE DB CONN";
        else
            qInfo() << "create db conn succ";
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
            qCritical() << "fail to init db with err = " << err.text() << ", sql=" << sql;
    });

    // repaint kvolumegrid
    Data::instance()->load(QDateTime::fromString("20250421", "yyyyMMdd"));

    // load sdk: init->dial->sub topic
    Handler::instance()->load(Config::instance()->sdkPath());
    char version[20] = {0};
    StrUtil::strncpy(version, Config::instance()->sdkVersion(), 20);
    Handler::instance()->init(version);
    
    qInfo() << "livermore-qt running";
    return a.exec();
}
