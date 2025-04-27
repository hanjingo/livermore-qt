#include "kvolumegrid.h"

#include <QMessageBox>
#include <QPainter>
#include <QPen>
#include <QString>

kVolumeGrid::kVolumeGrid(QWidget* parent)
    : AutoGrid( parent)
    , m_beginDay{0}
    , m_endDay{0}
    , m_totalDay{0}
    , m_currDay{0}
    , m_maxVolume{0.0}
    , m_lineWidth{0}
{
    setAtomGridHeightMin(40);

    connect(Data::instance(), SIGNAL(sigDataUpdated()), this, SLOT(dataUpdated()));
}

void kVolumeGrid::dataUpdated()
{
    this->repaint();
}

void kVolumeGrid::paintEvent(QPaintEvent *event)
{
    AutoGrid::paintEvent(event);

    drawYtick();
    drawVolume();
    drawAverageLine(5);
    drawAverageLine(10);
}

void kVolumeGrid::getIndicator()
{
    m_maxVolume = 0;
    for(int i = m_beginDay; i < m_endDay; ++i)
    {
        QString strVolume = QString::number(Data::instance()->mds()[i].volume, 'f', 3);
        strVolume = strVolume.mid(1,strVolume.length());
        strVolume = strVolume.mid(0,strVolume.length()-1);
        strVolume.replace(QString(","),QString(""));
        if( strVolume.toInt() > m_maxVolume)
            m_maxVolume = strVolume.toInt();
    }
    m_maxVolume = m_maxVolume / 100;
}

void kVolumeGrid::drawYtick()
{

    getIndicator();

    QPainter painter(this);
    QPen     pen;
    pen.setColor(Qt::red);
    painter.setPen(pen);
    double ystep = m_maxVolume / getHGridNum() ;
    QString str;
    for( int i = 0; i <= getHGridNum(); ++i)
    {
        str.sprintf("%d", (int)(i*ystep) );
        painter.drawText( QPoint( getWidgetWidth() - getMarginLeft() + 10,
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

    for( int i = m_beginDay; i < m_endDay; ++i)
    {
        if(Data::instance()->mds()[i].open_price > Data::instance()->mds()[i].close_price)
            pen.setColor(QColor(85,252,252));
        else
            pen.setColor(Qt::red);


        m_lineWidth = getGridWidth() / m_totalDay;
        m_lineWidth = m_lineWidth - 0.2*m_lineWidth;

        if( m_lineWidth < 3)
            m_lineWidth = 3;

        QPoint p1;
        QPoint p2;
        QPoint p3;
        QPoint p4;

        QString strtemp = QString::number(Data::instance()->mds()[i].volume, 'f', 3);
        strtemp = strtemp.mid(1,strtemp.length());
        strtemp = strtemp.mid(0,strtemp.length()-1);
        strtemp.replace(QString(","),QString(""));
        int temp = strtemp.toInt()/100;

        if(Data::instance()->mds()[i].open_price > Data::instance()->mds()[i].close_price)
        {
            pen.setWidth(m_lineWidth);
            painter.setPen(pen);
            p1.setX( getMarginLeft() + xstep *(i - m_beginDay) + 0.5*m_lineWidth);
            p1.setY( getWidgetHeight() - (temp ) *yscale - getMarginBottom());
            p2.setX( getMarginLeft() + xstep *(i - m_beginDay) + 0.5*m_lineWidth);
            p2.setY( getWidgetHeight()  - getMarginBottom() - 0.5*m_lineWidth);
            painter.drawLine(p1,p2);
        }

        else
        {
            pen.setWidth(1);
            painter.setPen(pen);

            p1.setX( getMarginLeft() + xstep *(i - m_beginDay) );
            p1.setY( getWidgetHeight() - (temp ) *yscale - getMarginBottom());
            p2.setX(getMarginLeft() + xstep *(i - m_beginDay) + m_lineWidth);
            p2.setY( getWidgetHeight() - (temp ) *yscale - getMarginBottom());
            p3.setX( getMarginLeft() + xstep *(i - m_beginDay) );
            p3.setY( getWidgetHeight()  - getMarginBottom() );
            p4.setX( getMarginLeft() + xstep *(i - m_beginDay) + m_lineWidth);
            p4.setY( getWidgetHeight()  - getMarginBottom() );

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

    double xstep = getGridWidth() / m_totalDay;
    double yscale = getGridHeight() / m_maxVolume;

    switch(day)
    {
    case 5:
        for( int i = m_beginDay; i < m_endDay; ++i)
        {
            if( Data::instance()->md5DayVolumeAverage() == 0)
                continue;
            temp.setX(getMarginLeft() + xstep *(i - m_beginDay) + 0.5*m_lineWidth);
            temp.setY(getWidgetHeight() - Data::instance()->md5DayVolumeAverage() /100 *yscale - getMarginBottom());
            point.push_back(temp);
        }
        break;
    case 10:
        for( int i = m_beginDay; i < m_endDay; ++i)
        {
            if( Data::instance()->md10DayVolumeAverage() == 0)
                continue;
            temp.setX(getMarginLeft() + xstep *(i - m_beginDay) + 0.5*m_lineWidth);
            temp.setY(getWidgetHeight() - Data::instance()->md10DayVolumeAverage() /100 *yscale - getMarginBottom());
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
