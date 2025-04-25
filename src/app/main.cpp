#include <QApplication>
#include <QTranslator>

#include "config.h"
#include "db.h"
#include "mainwindow.h"

#include "libcpp/log/logger.hpp"
#include "libcpp/os/env.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

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
    DBPool::instance()->setCreateConnFn([]() -> QSqlDatabase {
        auto db = QSqlDatabase::addDatabase(Config::instance()->dbType());
        db.setDatabaseName(Config::instance()->dbPath() + "/" + Config::instance()->dbMain());
        db.open();
        return db;
    });
    DBPool::instance()->setMaxConn(Config::instance()->dbMaxConn());

    // show main window
    MainWindow w;
    w.show();
    
    LOG_INFO("livermore-qt exit");
    return a.exec();
}
