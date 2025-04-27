#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <QVector>
#include <QPointer>
#include <QreadWriteLock>

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

    double md5DayVolumeAverage();
    double md10DayVolumeAverage();
    void refreshData() { _load(); }

signals:
    void sigDataUpdated();

private:
    void _load();
    void _save();

private:
    QReadWriteLock       m_lock;
    QVector<market_data> m_mds;
};

#endif
