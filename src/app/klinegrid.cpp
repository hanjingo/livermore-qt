#include "klinegrid.h"

#include <QMessageBox>
#include <QPainter>
#include <QPen>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QVector>
#include <QDockWidget>
#include <QWidget>

#include "libcpp/log/logger.hpp"

KLineGrid::KLineGrid(QWidget *parent) : AutoGrid(parent)
{
    setMouseTracking(true);
}

KLineGrid::~KLineGrid()
{
}

void KLineGrid::kLineChg(QVector<std::tuple<QDate, double, double, double, double, double, double>>& data)
{
    LOG_DEBUG("on sigkLineChg");
    m_data.clear();
    for (auto elem : data)
    {
        kLine k;
        k.tradingDay = std::get<0>(elem);
        k.volume = std::get<1>(elem);
        k.openPrice = std::get<2>(elem);
        k.closePrice = std::get<3>(elem);
        k.highestBid = std::get<4>(elem);
        k.lowestBid = std::get<5>(elem);
        k.amountOfIncrease = std::get<6>(elem);
        m_data.push_back(k);
    }

    // calc date
    m_beginDay = m_data.front().tradingDay;
    m_endDay   = m_data.back().tradingDay;
    m_totalDay = m_beginDay.daysTo(m_endDay);

    // calc max volume
    m_maxVolume = 0;
    for(kLine k : m_data)
    {
        m_maxVolume = (k.volume > m_maxVolume) ? k.volume : m_maxVolume;
    }

    // calc average pice
    calAverageLine5();
    calAverageLine10();
    calAverageLine20();
    calAverageLine30();
    calAverageLine60();

    // calc otehr
    calcOther();

    // repaint
    this->update();
}

void KLineGrid::paintEvent(QPaintEvent *event)
{
    AutoGrid::paintEvent(event);

    // draw Crosshair
    drawLine();
}

void KLineGrid::drawLine()
{
    drawYtick();
    drawKline();
    if(!m_isKeyDown && m_bCross)
        drawCross2();

    if(m_isKeyDown && m_bCross)
        drawCross();

    drawAverageLine(5);
    drawAverageLine(10);
    drawAverageLine(20);
    drawAverageLine(30);
    drawAverageLine(60);
}

void KLineGrid::drawYtick()
{
    QPainter painter(this);
    QPen     pen;
    pen.setColor(Qt::red);
    painter.setPen(pen);

    double ystep = (m_highestBid - m_lowestBid) / getHGridNum();
    QString str;
    if(0 == getHGridNum())
    {
        str.sprintf("%.2f", m_lowestBid);
        painter.drawText(QPoint(getWidgetWidth() - getMarginLeft() + 10,
                                getWidgetHeight() - getMarginBottom()),
                          str);
        str.sprintf("%.2f", m_highestBid);
        painter.drawText(QPoint(getWidgetWidth() - getMarginLeft() + 10,
                                getMarginTop() ),
                         str);
        return;
    }

    for( int i=0;i<=getHGridNum();++i)
    {
        str.sprintf("%.2f", m_lowestBid+ i * ystep);
        painter.drawText(QPoint(getWidgetWidth() - getMarginLeft() + 10,
                                getWidgetHeight() - getMarginBottom() - i * getAtomGridHeight()),
                         str);
    }
}

void KLineGrid::drawKline()
{
    QPainter painter(this);
    QPen     pen;
    pen.setColor(Qt::red);
    painter.setPen(pen);
    if (m_beginDay.isNull())
        return;

    m_yscale = getGridHeight() / (m_highestBid - m_lowestBid);
    QPoint p1;
    QPoint p2;
    QPoint p3;
    QPoint p4;
    double xstep = getGridWidth() / m_totalDay;
    for(kLine k : m_data)
    {
        if(k.openPrice > k.closePrice)
            pen.setColor(QColor(85,252,252));
        else
            pen.setColor(Qt::red);

        m_lineWidth = getGridWidth() / m_totalDay;
        m_lineWidth = m_lineWidth - 0.2 * m_lineWidth; // add line gap
        if(m_lineWidth < 3)
            m_lineWidth = 3;

        // negative line
        int dayOffset = m_beginDay.daysTo(k.tradingDay);
        if(k.openPrice > k.closePrice)
        {
            pen.setWidth(m_lineWidth);
            painter.setPen(pen);
            p1.setX(getMarginLeft() + xstep * dayOffset + 0.5 * m_lineWidth);
            p1.setY(getWidgetHeight() - (k.openPrice - m_lowestBid) * m_yscale - getMarginBottom());
            p2.setX(getMarginLeft() + xstep * dayOffset + 0.5 * m_lineWidth);
            p2.setY(getWidgetHeight() - (k.closePrice - m_lowestBid) * m_yscale - getMarginBottom() - 0.5 * m_lineWidth);
            painter.drawLine(p1,p2);

            pen.setWidth(1);
            painter.setPen(pen);
            p1.setX(getMarginLeft() + xstep * dayOffset + 0.5 * m_lineWidth);
            p1.setY(getWidgetHeight() - (k.highestBid - m_lowestBid) * m_yscale - getMarginBottom());
            p2.setX(getMarginLeft() + xstep * dayOffset + 0.5 * m_lineWidth);
            p2.setY(getWidgetHeight() - (k.lowestBid - m_lowestBid) * m_yscale - getMarginBottom());
            painter.drawLine(p1,p2);
        }
        else
        {
            pen.setWidth(1);
            painter.setPen(pen);

            p1.setX(getMarginLeft() + xstep * dayOffset);
            p1.setY(getWidgetHeight() - (k.openPrice - m_lowestBid) * m_yscale - getMarginBottom());

            p2.setX(getMarginLeft() + xstep * dayOffset + m_lineWidth);
            p2.setY(getWidgetHeight() - (k.openPrice - m_lowestBid) * m_yscale - getMarginBottom());

            p3.setX(getMarginLeft() + xstep * dayOffset);
            p3.setY(getWidgetHeight() - (k.closePrice - m_lowestBid) * m_yscale - getMarginBottom());

            p4.setX(getMarginLeft() + xstep * dayOffset + m_lineWidth);
            p4.setY(getWidgetHeight() - (k.closePrice - m_lowestBid) * m_yscale - getMarginBottom());

            painter.drawLine(p1,p2);
            painter.drawLine(p1,p3);
            painter.drawLine(p2,p4);
            painter.drawLine(p3,p4);

            pen.setWidth(1);
            painter.setPen(pen);
            p1.setX(getMarginLeft() + xstep * dayOffset + 0.5 * m_lineWidth);
            p1.setY(getWidgetHeight() - (k.highestBid - m_lowestBid) * m_yscale - getMarginBottom());

            double y1,y2;
            if(k.openPrice > k.closePrice)
            {
                y1 = k.openPrice;
                y2 = k.closePrice;
            }
            else
            {
                y1 = k.closePrice;
                y2 = k.openPrice;
            }

            p2.setX(getMarginLeft() + xstep * dayOffset + 0.5 * m_lineWidth);
            p2.setY(getWidgetHeight() - (y1 - m_lowestBid) * m_yscale - getMarginBottom());
            p3.setX(getMarginLeft() + xstep * dayOffset + 0.5 * m_lineWidth);
            p3.setY(getWidgetHeight() - (y2 - m_lowestBid) * m_yscale - getMarginBottom());
            p4.setX(getMarginLeft() + xstep * dayOffset + 0.5 * m_lineWidth);
            p4.setY(getWidgetHeight() - (k.lowestBid - m_lowestBid) * m_yscale - getMarginBottom());
            painter.drawLine(p1,p2);
            painter.drawLine(p3,p4);
        }
    }
}

void KLineGrid::keyPressEvent(QKeyEvent *event)
{
    auto dayOffset = (double)(m_mousePoint.x() - getMarginLeft()) / getGridWidth() * m_totalDay;
    m_currDay = m_beginDay.addDays(static_cast<qint64>(dayOffset));

    m_isKeyDown = true;
    switch(event->key())
    {
    case Qt::Key_Left:
    {
        if (m_data.isEmpty())
            return;

        double xstep = getGridWidth() / m_totalDay;
        if(m_mousePoint.x() - xstep < getMarginLeft())
        {
            if(m_beginDay.isNull())
                return;
            m_endDay.addDays(-1);
        }
        else
            m_mousePoint.setX(m_mousePoint.x() - xstep);

        update();
        break;
    }
    case Qt::Key_Right:
    {
        if (m_data.isEmpty())
            return;

        double xstep = getGridWidth() / m_totalDay ;
        if(m_mousePoint.x() + xstep > getWidgetWidth() - getMarginRight())
        {
            if(m_endDay.isNull() || m_data.isEmpty() || m_endDay >= m_data.back().tradingDay)
                return;
            m_endDay.addDays(1);
            m_beginDay.addDays(1);
        }
        else
            m_mousePoint.setX(m_mousePoint.x() + xstep);

        update();
        break;
    }
    case Qt::Key_Up:
    {
        if (m_data.isEmpty())
            return;

        m_totalDay = m_totalDay /2;
        // at least show 10 days data
        if(m_totalDay < 10)
        {
            m_totalDay *= 2;
            return;
        }

        m_endDay = m_currDay.addDays(m_totalDay / 2);
        m_beginDay = m_currDay.addDays(-(m_totalDay / 2));
        // if(m_endDay > m_data.back().tradingDay.addDays(-10))
        // {
        //     m_endDay = m_data.back().tradingDay.addDays(-10);
        //     m_beginDay = m_endDay.addDays(-totalDay);
        // }

        if(m_beginDay.isNull())
        {
            m_beginDay = m_data.front().tradingDay;
            m_endDay = m_beginDay.addDays(m_totalDay);
        }

        update();
        break;
    }
    case Qt::Key_Down:
    {
        if(m_data.isEmpty() || m_totalDay >= m_data.size())
            return;

        m_totalDay = m_totalDay * 2;
        if(m_totalDay > m_data.size())
        {
            m_totalDay = m_data.size();
        }

        m_endDay = m_currDay.addDays(m_totalDay / 2);
        if(m_endDay > m_data.back().tradingDay.addDays(-10))
        {
            m_endDay = m_data.back().tradingDay.addDays(-10);
        }

        m_beginDay = m_currDay.addDays(-(m_totalDay / 2));
        if(m_beginDay < m_data.front().tradingDay)
            m_beginDay = m_data.front().tradingDay;

        m_totalDay = m_beginDay.daysTo(m_endDay);
        update();
    }
    default:
        break;
    }
}

void KLineGrid::mouseMoveEvent(QMouseEvent *event)
{
    m_mousePoint = event->pos();
    m_isKeyDown = false;
    update();
}

void KLineGrid::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bCross = !m_bCross;
        update();
    }
}

void KLineGrid::resizeEvent(QResizeEvent* event)
{
    AutoGrid::resizeEvent(event);
    m_bCross = false;
}

void KLineGrid::drawCross()
{
    drawCrossVerLine();
    drawCrossHorLine();
    drawTips();
}

void KLineGrid::drawCrossVerLine()
{
    QPainter painter(this);
    QPen     pen;
    pen.setColor(QColor("#FFFFFF"));
    pen.setWidth(1);
    painter.setPen(pen);

    double xstep = getGridWidth() / m_totalDay ;
    double xPos = getMarginLeft() ;
    while(m_mousePoint.x() - xPos > xstep)
        xPos += xstep;

    xPos += 0.5 * m_lineWidth;
    QLine horline(xPos, getMarginTop(), xPos, getWidgetHeight() - getMarginBottom());
    painter.drawLine(horline);
}

void KLineGrid::drawCrossHorLine()
{
    QPainter painter(this);
    QPen     pen;
    pen.setColor(QColor("#FFFFFF"));
    pen.setWidth(1);
    painter.setPen(pen);

    double yPos;
    auto dayOffset = (m_mousePoint.x() - getMarginLeft()) / getGridWidth() * m_totalDay;
    auto day = m_beginDay.addDays(static_cast<qint64>(dayOffset));
    auto k = getDayKLine(day);
    if (k.tradingDay.isNull())
        return;

    if(k.openPrice < k.closePrice )
        yPos = (k.closePrice - m_lowestBid) * m_yscale;
    else
        yPos = (k.closePrice - m_lowestBid) * m_yscale;

    QLine verline(getMarginLeft(), getWidgetHeight() - getMarginBottom() - yPos,
                  getWidgetWidth() - getMarginRight(), getWidgetHeight() - getMarginBottom() - yPos);
    painter.drawLine(verline);
}

void KLineGrid::drawTips()
{
    QPainter painter(this);
    QPen     pen;
    QBrush brush(QColor(64,0,128));
    painter.setBrush(brush);
    pen.setColor(QColor("#FFFFFF"));
    pen.setWidth(1);
    painter.setPen(pen);

    auto dayOffset = (m_mousePoint.x() - getMarginLeft()) / getGridWidth() * m_totalDay;
    auto day = m_beginDay.addDays(static_cast<qint64>(dayOffset));
    auto k = getDayKLine(day);
    if (k.tradingDay.isNull())
        return;

    double yval = k.closePrice;
    double yPos;
    if(k.openPrice < k.closePrice )
        yPos = (k.closePrice - m_lowestBid) * m_yscale;
    else
        yPos = (k.closePrice - m_lowestBid) * m_yscale;

    yPos = getWidgetHeight() - getMarginBottom() - yPos;

    int iTipsWidth = 60;
    int iTipsHeight = 30;
    QString str;
    QRect rect(getWidgetWidth() - getMarginRight(),
               yPos - iTipsHeight / 2, iTipsWidth, iTipsHeight);
    painter.drawRect(rect);

    QRect rectText(getWidgetWidth() - getMarginRight() + iTipsWidth / 4,
                   yPos - iTipsHeight / 4, iTipsWidth, iTipsHeight);
    painter.drawText(rectText, str.sprintf("%.2f",yval));

    auto preK = getDayKLine(day.addDays(-1));
    if (preK.tradingDay.isNull())
        return;

    QColor openingColor = (k.openPrice > preK.openPrice) ? QColor("#FF0000") : QColor("#00FF00");
    QColor highestColor = (k.highestBid > preK.closePrice) ? QColor("#FF0000") : QColor("#00FF00");
    QColor lowestColor = (k.lowestBid > preK.closePrice) ? QColor("#FF0000") : QColor("#00FF00");
    QColor closeingColor = (k.closePrice > k.openPrice) ? QColor("#FF0000") : QColor("#00FF00");
    QColor amountOfIncreaseColor = (k.amountOfIncrease > 0) ? QColor("#FF0000") : QColor("#00FF00");
    // m_showDetail->receiveParams(k.m_time, QColor("#FFFFFF"),
    //                             k.m_closePrice, QColor("#FF0000"),
    //                             k.m_openPrice, openingColor,
    //                             k.m_highestBid, highestColor,
    //                                  mDataFile.kline[currentDay].lowestBid,lowestColor,
    //                                  mDataFile.kline[currentDay].closeingPrice,closeingColor,
    //                                  mDataFile.kline[currentDay].amountOfIncrease,amountOfIncreaseColor,
    //                                  mDataFile.kline[currentDay].amountOfAmplitude,QColor("#02E2F4"),
    //                                  mDataFile.kline[currentDay].totalVolume,QColor("#02E2F4"),
    //                                  mDataFile.kline[currentDay].totalAmount,QColor("#02E2F4"),
    //                                  mDataFile.kline[currentDay].turnoverRate,QColor("#02E2F4")
    //                                  );
}


void KLineGrid::drawMouseMoveCrossVerLine()
{
    if(m_mousePoint.x() < getMarginLeft() || m_mousePoint.x() > getWidgetWidth() - getMarginRight())
        return;
    if(m_mousePoint.y() < getMarginTop() || m_mousePoint.y() > getWidgetHeight() - getMarginBottom())
        return;

    QPainter painter(this);
    QPen     pen;
    pen.setColor(QColor("#FFFFFF"));
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawLine(m_mousePoint.x(), getMarginTop(),
                     m_mousePoint.x(), getWidgetHeight() - getMarginBottom());

}

void KLineGrid::drawMouseMoveCrossHorLine()
{
    if(m_mousePoint.x() < getMarginLeft() || m_mousePoint.x() > getWidgetWidth() - getMarginRight())
        return;
    if(m_mousePoint.y() < getMarginTop() || m_mousePoint.y() > getWidgetHeight() - getMarginBottom())
        return;

    QPainter painter(this);
    QPen     pen;
    pen.setColor(QColor("#FFFFFF"));
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawLine(getMarginLeft(), m_mousePoint.y(),
                     getWidgetWidth() - getMarginRight(), m_mousePoint.y());
}

void KLineGrid::drawCross2()
{
    drawMouseMoveCrossHorLine();
    drawMouseMoveCrossVerLine();
    drawTips2();
}

void KLineGrid::drawTips2()
{
    if(m_mousePoint.x() < getMarginLeft() || m_mousePoint.x() > getWidgetWidth() - getMarginRight())
        return;
    if(m_mousePoint.y() < getMarginTop() || m_mousePoint.y() > getWidgetHeight() - getMarginBottom())
        return;

    QPainter painter(this);
    QPen     pen;
    QBrush brush(QColor(64,0,128));
    painter.setBrush(brush);
    pen.setColor(QColor("#FFFFFF"));
    pen.setWidth(1);
    painter.setPen(pen);
    double yval = m_highestBid - (m_mousePoint.y() - getMarginTop()) / m_yscale;
    double yPos = m_mousePoint.y();
    int iTipsWidth = 60;
    int iTipsHeight = 30;
    QString str;
    QRect rect(getWidgetWidth() - getMarginRight(),
               yPos - iTipsHeight / 2, iTipsWidth, iTipsHeight);
    painter.drawRect(rect);

    QRect rectText(getWidgetWidth() - getMarginRight() + iTipsWidth / 4,
                   yPos - iTipsHeight / 4, iTipsWidth, iTipsHeight);
    painter.drawText(rectText, str.sprintf("%.2f", yval));
}

void KLineGrid::drawAverageLine(int day)
{
    m_yscale = getGridHeight() / (m_highestBid - m_lowestBid);
    QVector<QPoint> point;
    QPoint temp;
    double xstep = getGridWidth() / m_totalDay;
    if(m_beginDay.isNull())
        return;

    switch(day)
    {
    case 5:
        for(auto k : m_data)
        {
            if(k.dayAverage5 == 0)
                continue;

            auto dayOffset = m_beginDay.daysTo(k.tradingDay);
            temp.setX(getMarginLeft() + xstep * dayOffset + 0.5 * m_lineWidth);
            temp.setY(getWidgetHeight() - (k.dayAverage5 - m_lowestBid) * m_yscale - getMarginBottom());
            point.push_back(temp);
        }
        break;
    case 10:
        for(auto k : m_data)
        {
            if(k.dayAverage10 == 0)
                continue;

            auto dayOffset = m_beginDay.daysTo(k.tradingDay);
            temp.setX(getMarginLeft() + xstep * dayOffset + 0.5 * m_lineWidth);
            temp.setY(getWidgetHeight() - (k.dayAverage10 - m_lowestBid) * m_yscale - getMarginBottom());
            point.push_back(temp);
        }
        break;
    case 20:
        for(auto k : m_data)
        {
            if(k.dayAverage20 == 0)
                continue;

            auto dayOffset = m_beginDay.daysTo(k.tradingDay);
            temp.setX(getMarginLeft() + xstep * dayOffset + 0.5 * m_lineWidth);
            temp.setY(getWidgetHeight() - (k.dayAverage20 - m_lowestBid) * m_yscale - getMarginBottom());
            point.push_back(temp);
        }
        break;
    case 30:
        for(auto k : m_data)
        {
            if(k.dayAverage30 == 0)
                continue;

            auto dayOffset = m_beginDay.daysTo(k.tradingDay);
            temp.setX(getMarginLeft() + xstep * dayOffset + 0.5 * m_lineWidth);
            temp.setY(getWidgetHeight() - (k.dayAverage30 - m_lowestBid) * m_yscale - getMarginBottom());
            point.push_back(temp);
        }
        break;
    case 60:
        for(auto k : m_data)
        {
            if(k.dayAverage60 == 0)
                continue;

            auto dayOffset = m_beginDay.daysTo(k.tradingDay);
            temp.setX(getMarginLeft() + xstep * dayOffset + 0.5 * m_lineWidth);
            temp.setY(getWidgetHeight() - (k.dayAverage60 - m_lowestBid) * m_yscale - getMarginBottom());
            point.push_back(temp);
        }
        break;
    default:
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
    case 20:
        pen.setColor(Qt::magenta);
        break;
    case 30:
        pen.setColor(Qt::green);
        break;
    case 60:
        pen.setColor(Qt::cyan);
        break;
    default:
        pen.setColor(Qt::white);
        break;
    }
    painter.setPen(pen);
    QPolygon polykline(point);
    painter.drawPolyline(polykline);
}

kLine KLineGrid::getDayKLine(const QDate& date)
{
    if (date.isNull())
        return kLine();

    for (auto k : m_data)
    {
        if (k.tradingDay != date)
            continue;
        return k;
    }
    return kLine();
}

void KLineGrid::calAverageLine5()
{
    for(int i = 4; i < m_data.size(); ++i)
    {
        double sum = 0;
        for(int j = i - 4; j <= i; ++j)
        {
            sum += m_data[j].closePrice;
        }
        m_data[i].dayAverage5 = sum / 5;
    }
}

void KLineGrid::calAverageLine10()
{
    for(int i = 9; i < m_data.size(); ++i)
    {
        double sum = 0;
        for(int j = i - 9; j <= i; ++j)
        {
            sum += m_data[j].closePrice;
        }
        m_data[i].dayAverage10 = sum / 10;
    }
}

void KLineGrid::calAverageLine20()
{
    for(int i = 19; i < m_data.size(); ++i)
    {
        double sum = 0;
        for(int j = i - 19; j <= i; ++j)
        {
            sum += m_data[j].closePrice;
        }
        m_data[i].dayAverage20 = sum / 20;
    }
}

void KLineGrid::calAverageLine30()
{
    for(int i = 29; i < m_data.size(); ++i)
    {
        double sum = 0;
        for(int j = i - 29; j <= i; ++j)
        {
            sum += m_data[j].closePrice;
        }
        m_data[i].dayAverage30 = sum / 30;
    }
}

void KLineGrid::calAverageLine60()
{
    for(int i = 59; i < m_data.size(); ++i)
    {
        double sum = 0;
        for(int j = i - 59; j <= i; ++j)
        {
            sum += m_data[j].closePrice;
        }
        m_data[i].dayAverage60 = sum / 60;
    }
}

void KLineGrid::calcOther()
{

    m_highestBid = 0;
    m_lowestBid = 1000;
    m_maxVolume = 0;
    for(kLine k : m_data)
    {
        m_highestBid = (k.highestBid > m_highestBid) ? k.highestBid : m_highestBid;
        m_lowestBid = (k.lowestBid < m_lowestBid) ? k.lowestBid : m_lowestBid;
    }
}
