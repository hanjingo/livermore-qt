#include "kvolumegrid.h"

#include <QMessageBox>
#include <QPainter>
#include <QPen>
#include <QString>

#include "libcpp/log/logger.hpp"

kVolumeGrid::kVolumeGrid(QWidget* parent)
    : AutoGrid( parent)
    , m_beginDay{QDate::currentDate()}
    , m_endDay{QDate::currentDate()}
    , m_maxVolume{0.0}
    , m_totalDay{0}
    , m_lineWidth{0}
{
    setAtomGridHeightMin(40);
}

void kVolumeGrid::kVolumeChg(QVector<std::tuple<QDate, double, double, double>>& data)
{
    LOG_DEBUG("on sigkVolumeChg");
    if (data.isEmpty())
        return;

    m_volumes.clear();
    for (auto elem : data)
    {
        kVolume k;
        k.tradingDay = std::get<0>(elem);
        k.volume = std::get<1>(elem);
        k.openPrice = std::get<2>(elem);
        k.closePrice = std::get<3>(elem);
        k.dayVolumeAverage5 = 0;
        k.dayVolumeAverage10 = 0;
        m_volumes.push_back(k);
    }

    // calc date
    m_beginDay = m_volumes.front().tradingDay;
    m_endDay   = m_volumes.back().tradingDay;
    m_totalDay = m_beginDay.daysTo(m_endDay);

    // calc max volume
    m_maxVolume = 0;
    for(kVolume k : m_volumes)
    {
        m_maxVolume = (k.volume > m_maxVolume) ? k.volume : m_maxVolume;
    }

    // calc average volume
    calcDayVolumeAverage5();
    calcDayVolumeAverage10();

    // repaint
    this->update();
}

void kVolumeGrid::paintEvent(QPaintEvent *event)
{
    AutoGrid::paintEvent(event);

    drawYtick();
    drawVolume();
    drawAverageLine(5);
    drawAverageLine(10);
}

void kVolumeGrid::drawYtick()
{
    QPainter painter(this);
    QPen     pen;

    pen.setColor(Qt::red);
    painter.setPen(pen);
    double ystep = m_maxVolume / getHGridNum();
    LOG_DEBUG("cal m_maxVolume={}, getHGridNum()={}, ystep={}", m_maxVolume, getHGridNum(), ystep);
    QString str;
    for(int i = 0; i <= getHGridNum(); ++i)
    {
        str.sprintf("%d", (int)(i * ystep) );
        painter.drawText( QPoint(getWidgetWidth() - getMarginLeft() + 10,
                                 getWidgetHeight() - getMarginBottom() - i * getAtomGridHeight()),
                          str);
    }
}

void kVolumeGrid::drawVolume()
{
    QPainter painter(this);
    QPen     pen;
    double xstep = getGridWidth() / m_totalDay;
    double yscale = getGridHeight() / m_maxVolume;

    for(kVolume k : m_volumes)
    {
        if(k.openPrice > k.closePrice)
            pen.setColor(QColor(85,252,252)); // Price Down
        else
            pen.setColor(Qt::red); // Price Up

        m_lineWidth = getGridWidth() / m_totalDay;
        m_lineWidth = m_lineWidth - 0.2 * m_lineWidth;
        LOG_DEBUG("getGridWidth()={}, m_totalDay={}, m_lineWidth={}", getGridWidth(), m_totalDay, m_lineWidth);
        if( m_lineWidth < 3)
            m_lineWidth = 3;

        QPoint p1;
        QPoint p2;
        QPoint p3;
        QPoint p4;
        if(k.openPrice > k.closePrice)
        {
            pen.setWidth(m_lineWidth);
            painter.setPen(pen);
            p1.setX(getMarginLeft() + xstep * m_beginDay.daysTo(k.tradingDay) + 0.5 * m_lineWidth);
            p1.setY(getWidgetHeight() - k.volume * yscale - getMarginBottom());
            p2.setX(getMarginLeft() + xstep * m_beginDay.daysTo(k.tradingDay) + 0.5 * m_lineWidth);
            p2.setY(getWidgetHeight() - getMarginBottom() - 0.5 * m_lineWidth);
            painter.drawLine(p1,p2);
        } else {
            pen.setWidth(1);
            painter.setPen(pen);

            p1.setX(getMarginLeft() + xstep * m_beginDay.daysTo(k.tradingDay));
            p1.setY(getWidgetHeight() - k.volume * yscale - getMarginBottom());
            p2.setX(getMarginLeft() + xstep * m_beginDay.daysTo(k.tradingDay) + m_lineWidth);
            p2.setY(getWidgetHeight() - k.volume * yscale - getMarginBottom());
            p3.setX(getMarginLeft() + xstep * m_beginDay.daysTo(k.tradingDay));
            p3.setY(getWidgetHeight()  - getMarginBottom() );
            p4.setX(getMarginLeft() + xstep * m_beginDay.daysTo(k.tradingDay) + m_lineWidth);
            p4.setY(getWidgetHeight()  - getMarginBottom() );

            painter.drawLine(p1,p2);
            painter.drawLine(p1,p3);
            painter.drawLine(p2,p4);
            painter.drawLine(p3,p4);
        }
    }
}

void kVolumeGrid::drawAverageLine(int day)
{
    QVector<QPoint> point;
    QPoint temp;
    double xstep  = getGridWidth() / m_totalDay;
    double yscale = getGridHeight() / m_maxVolume;

    switch(day)
    {
    case 5:
        for(kVolume k : m_volumes)
        {
            if(k.dayVolumeAverage5 == 0)
                continue;

            temp.setX(getMarginLeft() + xstep * m_beginDay.daysTo(k.tradingDay) + 0.5 * m_lineWidth);
            temp.setY(getWidgetHeight() - k.dayVolumeAverage5 /100 * yscale - getMarginBottom());
            point.push_back(temp);
        }
        break;
    case 10:
        for(kVolume k : m_volumes)
        {
            if(k.dayVolumeAverage10 == 0)
                continue;

            temp.setX(getMarginLeft() + xstep * m_beginDay.daysTo(k.tradingDay) + 0.5 * m_lineWidth);
            temp.setY(getWidgetHeight() - k.dayVolumeAverage10 /100 * yscale - getMarginBottom());
            point.push_back(temp);
        }
        break;
    }

    QPainter painter(this);
    QPen     pen;
    switch(day)
    {
    case 5:
        pen.setColor(Qt::white);
        break;
    case 10:
        pen.setColor(Qt::yellow);
        break;
    }

    painter.setPen(pen);
    QPolygon polykline(point);
    painter.drawPolyline(polykline);
}

void kVolumeGrid::calcDayVolumeAverage5()
{
    for( int i = 4; i < m_volumes.size(); ++i)
    {
        double sum = 0;
        for(int j = i - 4; j <= i; ++j)
        {
            sum += m_volumes[j].closePrice;
        }
        m_volumes[i].dayVolumeAverage5 = sum / 5;
    }
}

void kVolumeGrid::calcDayVolumeAverage10()
{
    for( int i = 9; i < m_volumes.size(); ++i)
    {
        double sum = 0;
        for(int j = i - 9; j <= i; ++j)
        {
            sum += m_volumes[j].closePrice;
        }
        m_volumes[i].dayVolumeAverage5 = sum / 10;
    }
}
