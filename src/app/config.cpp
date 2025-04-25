#include "config.h"

#include <QCoreApplication>

Config::Config(QObject *p)
    : QObject(p)
    , m_setting(QCoreApplication::applicationDirPath() + "/conf.ini", QSettings::IniFormat)
{
}

Config::~Config()
{

}
