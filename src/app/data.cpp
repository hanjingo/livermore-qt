#include "data.h"

#include "util.h"
#include "libqt/log/logger.h"
#include "libqt/db/dbconnpool.h"
#include "libqt/util/strutil.h"

#include <QVariant>
#include <tuple>

Data::Data(QObject *p)
    : QObject(p)
{
}

Data::~Data()
{
    save();
    clear();
}

void Data::load(QDateTime start, QDateTime end, const int limit)
{
    QString sql("SELECT * FROM tick");
    QStringList conds;
    if (!start.isNull())
    {
        conds.push_back(QString("trading_day >= '%1'").arg(start.toString("yyyyMMdd")));
        conds.push_back(QString("action_time >= '%1'").arg(start.toString("hh:mm:ss")));
    }
    if (!end.isNull())
    {
        conds.push_back(QString("trading_day <= '%1'").arg(end.toString("yyyyMMdd")));
        conds.push_back(QString("action_time <= '%1'").arg(end.toString("hh:mm:ss")));
    }
    if (!conds.isEmpty())
    {
        QString and_cond("");
        sql += QString(" WHERE ");
        for (auto cond : conds)
        {
            sql += QString("%1%2").arg(and_cond).arg(cond);
            and_cond = " AND ";
        }
    }
    if (limit > 0)
        sql += QString(" LIMIT %1;").arg(limit);
    else
        sql += QString(";");

    DBConnPool::instance()->exec(sql, [this](QSqlQuery& query){

        auto err = query.lastError();
        if(!err.text().isEmpty())
        {
            qCritical() << "fail to load tick data with err = " << err.text();
            return;
        }

        this->save();
        this->clear();
        while (query.next())
        {
            market_data* md = new market_data();
            StrUtil::strncpy(md->instrument_id, query.value(0).toString(), 31);
            StrUtil::strncpy(md->instrument_name, query.value(1).toString(), 21);
            StrUtil::strncpy(md->exchange_id, query.value(2).toString(), 9);
            StrUtil::strncpy(md->trading_day, query.value(3).toString(), 9);
            StrUtil::strncpy(md->action_time, query.value(4).toString(), 9);
            md->last_price = query.value(5).toDouble();
            md->pre_close_price = query.value(6).toDouble();
            md->open_price = query.value(7).toDouble();
            md->pre_settlement_price = query.value(8).toDouble();
            md->highest_price = query.value(9).toDouble();
            md->lowest_price = query.value(10).toDouble();
            md->close_price = query.value(11).toDouble();
            md->settlement_price = query.value(12).toDouble();
            md->upper_limit_price = query.value(13).toDouble();
            md->lower_limit_price = query.value(14).toDouble();
            md->average_price = query.value(15).toDouble();
#ifdef L2
            md->bid_price1 = query.value(16).toDouble();
            md->ask_price1 = query.value(17).toDouble();
            md->bid_price2 = query.value(18).toDouble();
            md->ask_price2 = query.value(19).toDouble();
            md->bid_price3 = query.value(20).toDouble();
            md->ask_price3 = query.value(21).toDouble();
            md->bid_price4 = query.value(22).toDouble();
            md->ask_price4 = query.value(23).toDouble();
            md->bid_price5 = query.value(24).toDouble();
            md->ask_price5 = query.value(25).toDouble();
            md->bid_volume1 = query.value(26).toDouble();
            md->ask_volume1 = query.value(27).toDouble();
            md->bid_volume2 = query.value(28).toDouble();
            md->ask_volume2 = query.value(29).toDouble();
            md->bid_volume3 = query.value(30).toDouble();
            md->ask_volume3 = query.value(31).toDouble();
            md->bid_volume4 = query.value(32).toDouble();
            md->ask_volume4 = query.value(33).toDouble();
            md->bid_volume5 = query.value(34).toDouble();
            md->ask_volume5 = query.value(35).toDouble();
#endif
            md->volume = query.value(36).toDouble();
            md->pre_open_interest = query.value(37).toDouble();
            md->open_interest = query.value(38).toDouble();
            md->turnover = query.value(39).toDouble();
            md->action_ms = query.value(40).toDouble();

            // LOG_DEBUG("load tick data with: {}", fmtMarketData(md));
            this->m_mds.push_back(md);
        }

        this->update();
    });
}

void Data::save()
{
    // DB::instance()->exec("INSERT INTO tick ;", [this](QSqlQuery& query){
    //     auto err = query.lastError();
    //     if(!err.text().isEmpty())
    //     {
    //         LOG_ERROR("fail to save tick data with err = {}", err.text().toStdString());
    //         return;
    //     }
    // });
}

void Data::clear()
{
    for (auto ptr : m_mds)
    {
        delete ptr;
        ptr = nullptr;
    }
}

void Data::update()
{
    QString strkDay;
    QVector<std::tuple<QDate, double, double, double>> kVolumes;
    QVector<std::tuple<double, double>> ticks;

    for (auto md : m_mds)
    {
        QString day = QString(md->trading_day);
        // day change update k line
        if (strkDay != day)
        {
            strkDay = day;
            QDate day = QDate::fromString(strkDay, "yyyyMMdd");
            kVolumes.push_back(std::make_tuple(day, md->volume, md->open_price, md->close_price));
        }

        // tick data
        ticks.push_back(std::make_tuple(md->settlement_price,
                                        md->volume));
    }

    // draw day K line
    qDebug() << "emit sigkVolumeChg";
    emit sigkVolumeChg(kVolumes);

    // draw stock canvas
    qDebug() << "emit tickChg";
    emit sigTickChg(ticks);
}

void Data::onTickNtf(int num, market_data** mds)
{
    for (int i = 0; i < num; ++i)
    {
        m_mds.append(mds[i]);
    }

    this->update();
}
