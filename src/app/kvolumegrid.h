#ifndef KVOLUMEGRID_H
#define KVOLUMEGRID_H

#include "libqt/ui/autogrid.h"

#include <QPoint>
#include <QDate>
#include <QVector>
#include <tuple>

struct kVolume
{
public:
    QDate  tradingDay;
    double volume;
    double openPrice;
    double closePrice;
    double dayVolumeAverage5;
    double dayVolumeAverage10;
};

class kVolumeGrid : public AutoGrid
{
    Q_OBJECT

public:
    explicit kVolumeGrid(QWidget* parent);
    void drawYtick();
    void drawVolume();
    void virtual paintEvent(QPaintEvent* event);
    void drawAverageLine(int day);

private:
    void calcDayVolumeAverage5();
    void calcDayVolumeAverage10();

private slots:
    // [{tradingDay,volume,openPrice,closePrice}, ...]
    void kVolumeChg(QVector<std::tuple<QDate, double, double, double>>& data);

private:
    QDate  m_beginDay;
    QDate  m_endDay;
    double m_maxVolume;
    int    m_totalDay;
    int    m_lineWidth;
    QPoint m_mousePoint;

    QVector<kVolume> m_volumes;
};

#endif // KVOLUMEGRID_H
