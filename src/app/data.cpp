#include "data.h"

#include "db.h"
#include "util.h"
#include "libcpp/log/logger.hpp"

#include <QVariant>

Data::Data(QObject *p)
    : QObject(p)
{
}

Data::~Data()
{
}

double Data::md5DayVolumeAverage()
{
    return 0.0;
}

double Data::md10DayVolumeAverage()
{
    return 0.0;
}

void Data::_load()
{
    DB::instance()->exec("SELECT * FROM tick LIMIT 10;", [this](QSqlQuery& query){
        auto err = query.lastError();
        if(!err.text().isEmpty())
        {
            LOG_ERROR("fail to load tick data with err = {}", err.text().toStdString());
            return;
        }

        this->mds().clear();
        while (query.next())
        {
            market_data md;
            memcpy(md.instrument_id, query.value(0).toString().toStdString().c_str(), 31);
            memcpy(md.instrument_name, query.value(1).toString().toStdString().c_str(), 21);
            memcpy(md.exchange_id, query.value(2).toString().toStdString().c_str(), 9);
            memcpy(md.trading_day, query.value(3).toString().toStdString().c_str(), 9);
            memcpy(md.action_time, query.value(4).toString().toStdString().c_str(), 9);
            md.last_price = query.value(5).toDouble();
            md.pre_close_price = query.value(6).toDouble();
            md.open_price = query.value(7).toDouble();
            md.pre_settlement_price = query.value(8).toDouble();
            md.highest_price = query.value(9).toDouble();
            md.lowest_price = query.value(10).toDouble();
            md.close_price = query.value(11).toDouble();
            md.settlement_price = query.value(12).toDouble();
            md.upper_limit_price = query.value(13).toDouble();
            md.lower_limit_price = query.value(14).toDouble();
            md.average_price = query.value(15).toDouble();
#ifdef L2
            md.bid_price1 = query.value(16).toDouble();
            md.ask_price1 = query.value(17).toDouble();
            md.bid_price2 = query.value(18).toDouble();
            md.ask_price2 = query.value(19).toDouble();
            md.bid_price3 = query.value(20).toDouble();
            md.ask_price3 = query.value(21).toDouble();
            md.bid_price4 = query.value(22).toDouble();
            md.ask_price4 = query.value(23).toDouble();
            md.bid_price5 = query.value(24).toDouble();
            md.ask_price5 = query.value(25).toDouble();
            md.bid_volume1 = query.value(26).toDouble();
            md.ask_volume1 = query.value(27).toDouble();
            md.bid_volume2 = query.value(28).toDouble();
            md.ask_volume2 = query.value(29).toDouble();
            md.bid_volume3 = query.value(30).toDouble();
            md.ask_volume3 = query.value(31).toDouble();
            md.bid_volume4 = query.value(32).toDouble();
            md.ask_volume4 = query.value(33).toDouble();
            md.bid_volume5 = query.value(34).toDouble();
            md.ask_volume5 = query.value(35).toDouble();
#endif
            md.volume = query.value(36).toDouble();
            md.pre_open_interest = query.value(37).toDouble();
            md.open_interest = query.value(38).toDouble();
            md.turnover = query.value(39).toDouble();
            md.action_ms = query.value(40).toDouble();

            LOG_DEBUG("load tick data with: {}", fmtMarketData(md));
            this->m_mds.push_back(md);
        }
        emit sigDataUpdated();
    });
}

void Data::_save()
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
