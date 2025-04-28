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

    inline QVector<market_data>& mds() { return m_mds; }
    void load();
    void save();

signals:
    // [{tradingDay,volume,openPrice,closePrice}, ...]
    void sigkVolumeChg(QVector<std::tuple<QDate, double, double, double>>&);

    // [{tradingDay,volume,openPrice,closePrice,highestBid,lowestBid,amountOfIncrease}, ...]
    void sigkLineChg(QVector<std::tuple<QDate, double, double, double, double, double, double>>&);

private:
    QReadWriteLock       m_lock;
    QVector<market_data> m_mds;
};

#endif
