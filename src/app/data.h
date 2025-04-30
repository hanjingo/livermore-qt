#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <QVector>
#include <QPointer>
#include <QreadWriteLock>
#include <QDate>

#include "market_data.h"

class Data : public QObject
{
    Q_OBJECT

public:
    Data(QObject* p = Q_NULLPTR);
    ~Data();

    static QPointer<Data> instance()
    {
        static QPointer<Data> inst(new Data());
        return inst;
    }

    static QString dbID()
    {
        static qint64 id = 0;
        id++;
        return QString::number(id);
    }

    inline QVector<market_data*>& mds() { return m_mds; }
    inline void load(const int limit = -1) {load(QDateTime(), QDateTime(), limit);}
    inline void load(QDateTime start, const int limit = -1) {load(start, QDateTime(), limit);}
    void load(QDateTime start, QDateTime end, const int limit = -1);
    void save();
    void clear();
    void update();

    void onTickNtf(market_data** mds, const int len);

signals:
    // [{tradingDay,volume,openPrice,closePrice}, ...]
    void sigkVolumeChg(QVector<std::tuple<QDate, double, double, double>>&);

    // [{tradingDay,volume,openPrice,closePrice,highestBid,lowestBid,amountOfIncrease}, ...]
    void sigkLineChg(QVector<std::tuple<QDate, double, double, double, double, double, double>>&);

    // [{price,volume}, ...]
    void sigTickChg(QVector<std::tuple<double, double>>&);

private:
    QReadWriteLock        m_lock;
    QVector<market_data*> m_mds;
};

#endif
