#include "stockcanvas.h"

#include <QDebug>
#include <QPen>
#include <QPainter>
#include <QDateTime>

#include "libcpp/log/logger.hpp"

StockCanvas::StockCanvas(QWidget *parent) : QWidget(parent)
{
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Window,QColor("#000000"));
    this->setPalette(palette);
    setMouseTracking(true);
}

void StockCanvas::tickChg(QVector<std::tuple<double, double>>& data)
{
    LOG_DEBUG("on tickChg");
    if (data.isEmpty())
        return;

    m_priceStart = std::get<0>(data.front());
    m_ticks.clear();
    for (auto elem : data)
    {
        TickData tick;
        tick.price = std::get<0>(elem);
        tick.volume = std::get<1>(elem);
        m_ticks.push_back(tick);
    }

    // calc volume&price max/min
    m_priceMin = 100000000.0;
    for (TickData tk : m_ticks)
    {
        m_volumeMax = (tk.volume > m_volumeMax) ? tk.volume : m_volumeMax;
        m_priceMax = (tk.price > m_priceMax) ? tk.price : m_priceMax;
        m_priceMin = (tk.price < m_priceMin) ? tk.price : m_priceMin;
    }

    // calc price rate
    double UpRate, DnRate;
    if(m_priceMax > m_priceStart)
        UpRate = (m_priceMax - m_priceStart) / m_priceStart;
    if(m_priceMin < m_priceStart)
        DnRate = (m_priceStart - m_priceMin) / m_priceStart;
    m_priceRate = (UpRate > DnRate) ? UpRate : DnRate;
}

void StockCanvas::paintEvent(QPaintEvent *event)
{
    drawBK();
    drawMouseLine();
    drawTimeTblChart();
}

void StockCanvas::resizeEvent(QResizeEvent *event)
{
    m_windowHeight = this->height();
    m_windowWidth = this->width();
}

void StockCanvas::drawBorder()
{
    QPainter painter(this);
    QPen     pen;
    pen.setColor(QColor("#FF0000"));
    painter.setPen(pen);

    painter.drawLine(0 + BORDER_SIZE, 0 + BORDER_SIZE,
                     0 + BORDER_SIZE, m_windowHeight - BORDER_SIZE); //left |
    painter.drawLine(0 + BORDER_SIZE, 0 + BORDER_SIZE,
                     m_windowWidth - BORDER_SIZE, 0 + BORDER_SIZE); //top -
    painter.drawLine(m_windowWidth - BORDER_SIZE, 0 + BORDER_SIZE,
                     m_windowWidth - BORDER_SIZE, m_windowHeight - BORDER_SIZE); //right |
    painter.drawLine(0 + BORDER_SIZE, m_windowHeight - BORDER_SIZE,
                     m_windowWidth - BORDER_SIZE, m_windowHeight - BORDER_SIZE); //bottom -
}

void StockCanvas::drawBK()
{

    QPainter painter(this);
    QPen     pen;
    pen.setColor(QColor("#FF0000"));
    painter.setPen(pen);

    //drawBorder();
    painter.drawLine(0 + BORDER_SIZE+COORDINATE_X1, 0 + BORDER_SIZE+COORDINATE_Y1,
                     0 + BORDER_SIZE+COORDINATE_X1, m_windowHeight - BORDER_SIZE-COORDINATE_Y2);
    painter.drawLine(0 + BORDER_SIZE+COORDINATE_X1, 0 + BORDER_SIZE+COORDINATE_Y1,
                     m_windowWidth-BORDER_SIZE-COORDINATE_X2, 0 + BORDER_SIZE+COORDINATE_Y1);
    painter.drawLine(m_windowWidth-BORDER_SIZE-COORDINATE_X2, 0 + BORDER_SIZE+COORDINATE_Y1,
                     m_windowWidth-BORDER_SIZE-COORDINATE_X2, m_windowHeight-BORDER_SIZE-COORDINATE_Y2);
    painter.drawLine(0 + BORDER_SIZE+COORDINATE_X1, m_windowHeight-BORDER_SIZE-COORDINATE_Y2,
                     m_windowWidth-BORDER_SIZE-COORDINATE_X2, m_windowHeight-BORDER_SIZE-COORDINATE_Y2);

    m_yInterval = (double)((m_windowHeight-BORDER_SIZE-COORDINATE_Y2) - (0+BORDER_SIZE+COORDINATE_Y1)) / 15 + 0.5;
    pen.setColor(QColor("#FF0000"));
    for(int i = 1, j = m_yInterval; i < 15; i++, j += m_yInterval)
    {
        if( i % 5 != 0 )
        {
            pen.setStyle(Qt::DashLine);
            painter.setPen(pen);
        }
        else
        {
            pen.setStyle(Qt::SolidLine);
            painter.setPen(pen);
        }
        painter.drawLine(0 + BORDER_SIZE+COORDINATE_X1, 0 + BORDER_SIZE+COORDINATE_Y1 + j,
                         m_windowWidth - BORDER_SIZE - COORDINATE_X2, 0 + BORDER_SIZE + COORDINATE_Y1 + j);
    }

    m_xInterval = (double)((m_windowWidth - BORDER_SIZE - COORDINATE_X2) - (0 + BORDER_SIZE + COORDINATE_X1) ) / 8 + 0.5;
    for(int x = 1, y = m_xInterval; x < 8; x++, y += m_xInterval)
    {
        if( x % 2 != 0 )
        {
            pen.setStyle(Qt::DashLine);
            painter.setPen(pen);
        }
        else
        {
            pen.setStyle(Qt::SolidLine);
            painter.setPen(pen);
        }
        painter.setPen(pen);
        painter.drawLine(0 + BORDER_SIZE + COORDINATE_X1 + y, 0 + BORDER_SIZE + COORDINATE_Y1,
                         0 + BORDER_SIZE + COORDINATE_X1 + y, m_windowHeight - BORDER_SIZE - COORDINATE_Y2);
    }
}

void StockCanvas:: mouseMoveEvent(QMouseEvent* event)
{
    m_mousePoint = QPoint(event->pos());
    m_xGridMin = BORDER_SIZE+COORDINATE_X1;
    m_xGridMax = m_windowWidth - (BORDER_SIZE + COORDINATE_X1);
    m_yGridMin = BORDER_SIZE + COORDINATE_Y1;
    m_yGridMax = m_windowHeight - BORDER_SIZE - COORDINATE_Y2;

    if(m_mousePoint.y() < m_yGridMin || m_mousePoint.y() > m_yGridMax)
        return;
    if(m_mousePoint.x() < m_xGridMin || m_mousePoint.x() > m_xGridMax)
        return;
    update();
}

void StockCanvas::drawMouseLine()
{
    QLineF linex(0 + BORDER_SIZE + COORDINATE_X1, m_mousePoint.y(), this->width() - (BORDER_SIZE + COORDINATE_X1), m_mousePoint.y());
    QLineF liney(m_mousePoint.x(), BORDER_SIZE + COORDINATE_Y1, m_mousePoint.x(), this->height() - (BORDER_SIZE + COORDINATE_Y2));
    QPainter painter(this);
    QPen     pen;
    pen.setColor(QColor("#FFFFFF"));
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawLine(linex);
    painter.drawLine(liney);
    drawTips();
}

// draw time table chart
void StockCanvas::drawTimeTblChart()
{
    QPainter painter(this);
    QPen     pen;
    pen.setColor(Qt::white);
    painter.setPen(pen);

    m_yInterval = (double)((m_windowHeight - BORDER_SIZE - COORDINATE_Y2) - (0 + BORDER_SIZE+COORDINATE_Y1)) / 15;
    painter.drawText(QPoint(m_windowWidth - BORDER_SIZE - COORDINATE_X2 + 10,
                            0 + BORDER_SIZE + COORDINATE_Y1 + m_yInterval * 5 + 2),
                     QString("0.00%"));

    QString str;
    str.sprintf("%.2f", m_priceStart / 1000.0 );
    painter.drawText(QPoint(0 + BORDER_SIZE + COORDINATE_X1 - TEXT_LENGTH,
                            0 + BORDER_SIZE + COORDINATE_Y1 + m_yInterval * 5 + 2),
                     str);

    pen.setColor(Qt::red);
    painter.setPen(pen);
    double proportional;
    for( int i = 5; i > 0; i--)
    {
         proportional = m_priceRate / 5  * i * 100;
         str.sprintf("%.2f%", proportional);
         painter.drawText(QPoint(m_windowWidth - BORDER_SIZE - COORDINATE_X2 + 10,
                                 0 + BORDER_SIZE + COORDINATE_Y1 + m_yInterval * (5 - i) + 2),
                          str);
         str.sprintf("%.2f", m_priceStart / 1000 * (1 + proportional / 100));
         painter.drawText(QPoint(0 + BORDER_SIZE + COORDINATE_X1 - TEXT_LENGTH,
                                 0 + BORDER_SIZE + COORDINATE_Y1 + m_yInterval * (5 - i) + 2),
                          str);
    }

    pen.setColor(Qt::green);
    painter.setPen(pen);
    for(int i = 1; i <= 5; i++)
    {
        proportional = m_priceRate / 5  * i * 100;
        str.sprintf("%.2f%%", proportional);
        painter.drawText(QPoint(m_windowWidth - BORDER_SIZE - COORDINATE_X2 + 10,
                                0 + BORDER_SIZE + COORDINATE_Y1 + m_yInterval * (5 + i)),
                         str);

        str.sprintf("%.2f", m_priceStart / 1000 * (1 - proportional / 100));
        painter.drawText(QPoint(0 + BORDER_SIZE + COORDINATE_X1 - TEXT_LENGTH,
                                0 + BORDER_SIZE + COORDINATE_Y1 + m_yInterval * (5 + i) + 2),
                         str);
    }

    pen.setColor(Qt::red);
    painter.setPen(pen);
    for(int i = 0; i < 9; i++)
    {
        double rateI = (double)((m_windowWidth - BORDER_SIZE - COORDINATE_X2) - (0 + BORDER_SIZE + COORDINATE_X1) ) / 8 + 0.5;
        QPoint ft(BORDER_SIZE + COORDINATE_X2 - 10 + i * (rateI), m_windowHeight - BORDER_SIZE - COORDINATE_Y2 + 12);
        QDateTime time = QDateTime::fromString("09:30", "HH:mm");
        QDateTime newTime;
        if(i >= 4){
           newTime = time.addSecs(1800 * (i + 3));
        }else if(i < 4){
           newTime = time.addSecs(1800 * i);
        }
        QString str = newTime.toString("HH:mm");
        painter.drawText(ft, str);
    }

    pen.setColor(QColor("#FFFFFF"));
    pen.setWidth(1);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    int zeroX = 0 + BORDER_SIZE + COORDINATE_X1;
    int zeroY = 0 + BORDER_SIZE + COORDINATE_Y1 + m_yInterval * 5;
    double xInter = double(m_windowWidth - 2 * BORDER_SIZE - COORDINATE_X1 - COORDINATE_X2) / 241;
    QPoint p1(zeroX, zeroY), p2(zeroX, zeroY);
    for(int i = 0; i < m_ticks.size(); ++i)
    {
        if(i == m_ticks.size() - 1)
            p2.setX(this->width() - BORDER_SIZE - COORDINATE_X1);
        else
            p2.setX(zeroX + i * xInter);

        double tempRate = double(m_ticks[i].price - m_priceStart) / m_priceStart;
        p2.setY(zeroY - tempRate / m_priceRate * 5 * m_yInterval);
        painter.drawLine(p1, p2);
        p1 = p2;
    }

    double factor = 5 * m_yInterval / m_volumeMax;
    for(int i = 0; i < m_ticks.size(); ++i)
    {
        p2.setX(zeroX + i * xInter);
        p1.setX(zeroX + i * xInter);
        p1.setY(m_windowHeight - BORDER_SIZE - COORDINATE_Y2);
        p2.setY(BORDER_SIZE + COORDINATE_Y2 + 15 * m_yInterval - m_ticks[i].volume * factor);
        painter.drawLine(p1,p2);
    }

    drawTips();
}

void StockCanvas::setLSpace(QString &str, int n)
{
    for(int i = 0; i < n; i++)
    {
        str.push_front(" ");
    }
}

void StockCanvas::drawTips()
{
    double temp = m_mousePoint.y() - BORDER_SIZE - COORDINATE_Y1 - 5 * m_yInterval;
    double y_val = - temp * (m_priceStart * m_priceRate) / (5 * m_yInterval) + m_priceStart;
    double y_valPCT = (y_val - m_priceStart) / m_priceStart;
    y_val /= 1000;
    y_valPCT *= 100;

    int iTipsWidth  = 60;
    int iTipsHeight = 20;
    QRect rect(0 + BORDER_SIZE + COORDINATE_X1 - TEXT_LENGTH - 15,
               m_mousePoint.y() - iTipsHeight / 2, iTipsWidth, iTipsHeight);
    QPainter painter(this);
    QPen     pen;
    pen.setColor(Qt::white);

    if( y_valPCT > 0)
    {
        QBrush brush(Qt::red);
        painter.setBrush(brush);
    }
    else
    {
        QBrush brush(Qt::blue);
        painter.setBrush(brush);
    }

    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawRect(rect);
    QString text;
    text.sprintf("%.2f", y_val);
    painter.drawText(rect, Qt::AlignCenter,text);

    QRect rect2(m_windowWidth - BORDER_SIZE - COORDINATE_X2,
               m_mousePoint.y() - iTipsHeight / 2, iTipsWidth, iTipsHeight);
    painter.drawRect(rect2);
    text.sprintf("%.2f", y_valPCT);
    painter.drawText(rect2, Qt::AlignCenter,text);

    double temp2 = double(m_mousePoint.x() - BORDER_SIZE - COORDINATE_X1) / (m_windowWidth - 2 * BORDER_SIZE - COORDINATE_X1 - COORDINATE_X2) * 4;
    if(temp2 > 2)
        temp2 += 1.5;

    QDateTime time = QDateTime::fromString("09:30","HH:mm");
    QDateTime newTime;
    newTime = time.addSecs(temp2 * 60 * 60 );
    text = newTime.toString("HH:mm");

    QBrush brush(Qt::red);
    painter.setBrush(brush);
//    painter.setPen(pen);
    QRect rect3(m_mousePoint.x() - iTipsWidth / 2, m_windowHeight - BORDER_SIZE - COORDINATE_Y2, iTipsWidth, iTipsHeight);
    painter.drawRect(rect3);
    painter.drawText(rect3, text);
}
