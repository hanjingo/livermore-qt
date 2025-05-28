#include "util.h"

#include "libqt/util/strutil.h"

void md_to_json(market_data* md, QJsonObject& js)
{
    js["trading_day"] = md->trading_day;
    js["instrument_name"] = md->instrument_name;
    js["instrument_id"] = md->instrument_id;
    js["exchange_id"] = md->exchange_id;
    js["last_price"] = md->last_price;
    js["pre_close_price"] = md->pre_close_price;
    js["open_price"] = md->open_price;
    js["pre_settlement_price"] = md->pre_settlement_price;
    js["highest_price"] = md->highest_price;
    js["lowest_price"] = md->lowest_price;
    js["close_price"] = md->close_price;
    js["settlement_price"] = md->settlement_price;
    js["upper_limit_price"] = md->upper_limit_price;
    js["lower_limit_price"] = md->lower_limit_price;
    js["average_price"] = md->average_price;
#ifdef USE_DEEP_DATA
    js["bid_price1"] = md->bid_price1;
    js["ask_price1"] = md->ask_price1;
    js["bid_price2"] = md->bid_price2;
    js["ask_price2"] = md->ask_price2;
    js["bid_price3"] = md->bid_price3;
    js["ask_price3"] = md->ask_price3;
    js["bid_price4"] = md->bid_price4;
    js["ask_price4"] = md->ask_price4;
    js["bid_price5"] = md->bid_price5;
    js["ask_price5"] = md->ask_price5;
    js["bid_volume1"] = md->bid_volume1;
    js["ask_volume1"] = md->ask_volume1;
    js["bid_volume2"] = md->bid_volume2;
    js["ask_volume2"] = md->ask_volume2;
    js["bid_volume3"] = md->bid_volume3;
    js["ask_volume3"] = md->ask_volume3;
    js["bid_volume4"] = md->bid_volume4;
    js["ask_volume4"] = md->ask_volume4;
    js["bid_volume5"] = md->bid_volume5;
    js["ask_volume5"] = md->ask_volume5;
#endif
    js["volume"] = md->volume;
    js["pre_open_interest"] = md->pre_open_interest;
    js["open_interest"] = md->open_interest;
    js["turnover"] = md->turnover;
    js["action_time"] = md->action_time;
    js["action_ms"] = md->action_ms;
}

void json_to_md(QJsonObject& js, market_data* md)
{
    memcpy(md->trading_day, js["trading_day"].toString().toStdString().c_str(), 9);
    memcpy(md->instrument_name, js["instrument_name"].toString().toStdString().c_str(), 21);
    memcpy(md->instrument_id, js["instrument_id"].toString().toStdString().c_str(), 31);
    memcpy(md->exchange_id, js["exchange_id"].toString().toStdString().c_str(), 9);
    md->last_price = js["last_price"].toDouble();
    md->pre_close_price = js["pre_close_price"].toDouble();
    md->open_price = js["open_price"].toDouble();
    md->pre_settlement_price = js["pre_settlement_price"].toDouble();
    md->highest_price = js["highest_price"].toDouble();
    md->lowest_price = js["lowest_price"].toDouble();
    md->close_price = js["close_price"].toDouble();
    md->settlement_price = js["settlement_price"].toDouble();
    md->upper_limit_price = js["upper_limit_price"].toDouble();
    md->lower_limit_price = js["lower_limit_price"].toDouble();
    md->average_price = js["average_price"].toDouble();
#ifdef USE_DEEP_DATA
    md->bid_price1 = js["bid_price1"].toDouble();
    md->ask_price1 = js["ask_price1"].toDouble();
    md->bid_price2 = js["bid_price2"].toDouble();
    md->ask_price2 = js["ask_price2"].toDouble();
    md->bid_price3 = js["bid_price3"].toDouble();
    md->ask_price3 = js["ask_price3"].toDouble();
    md->bid_price4 = js["bid_price4"].toDouble();
    md->ask_price4 = js["ask_price4"].toDouble();
    md->bid_price5 = js["bid_price5"].toDouble();
    md->ask_price5 = js["ask_price5"].toDouble();
    md->bid_volume1 = js["bid_volume1"].toDouble();
    md->ask_volume1 = js["ask_volume1"].toDouble();
    md->bid_volume2 = js["bid_volume2"].toDouble();
    md->ask_volume2 = js["ask_volume2"].toDouble();
    md->bid_volume3 = js["bid_volume3"].toDouble();
    md->ask_volume3 = js["ask_volume3"].toDouble();
    md->bid_volume4 = js["bid_volume4"].toDouble();
    md->ask_volume4 = js["ask_volume4"].toDouble();
    md->bid_volume5 = js["bid_volume5"].toDouble();
    md->ask_volume5 = js["ask_volume5"].toDouble();
#endif
    md->volume = js["volume"].toDouble();
    md->pre_open_interest = js["pre_open_interest"].toDouble();
    md->open_interest = js["open_interest"].toDouble();
    md->turnover = js["turnover"].toDouble();
    memcpy(md->action_time, js["action_time"].toString().toStdString().c_str(), 9);
    md->action_ms = js["action_ms"].toInt();
}

bool checkContext(context* ctx)
{
    return ctx != nullptr 
        && sizeof(*ctx) == ctx->sz;
}