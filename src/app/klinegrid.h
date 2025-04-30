#ifndef KLINEGRID_H
#define KLINEGRID_H

#include <QPoint>
#include <QString>
#include <QDate>

#include "libqt/ui/autogrid.h"

struct kLine
{
public:
    QDate  tradingDay;
    double volume;
    double openPrice;
    double closePrice;
    double highestBid;
    double lowestBid;
    double amountOfIncrease;
    double dayAverage5;
    double dayAverage10;
    double dayAverage20;
    double dayAverage30;
    double dayAverage60;
};

class KLineGrid : public AutoGrid
{
    Q_OBJECT

public:
    explicit KLineGrid(QWidget *parent = 0);
    void virtual paintEvent(QPaintEvent* event);
    void virtual keyPressEvent(QKeyEvent* event);
    void virtual mouseMoveEvent(QMouseEvent* event);
    void virtual mousePressEvent(QMouseEvent* event);
    void virtual resizeEvent(QResizeEvent* event);
    ~KLineGrid();

    void drawLine();
    void drawYtick();
    void drawKline();

    // Crosshair with key pressed
    void drawCross();
    void drawCrossVerLine();
    void drawCrossHorLine();
    void drawTips();

    // Crosshair with key unpressed
    void drawCross2();
    void drawMouseMoveCrossVerLine();
    void drawMouseMoveCrossHorLine();
    void drawTips2();

    // draw average line
    void drawAverageLine(int day);

signals:

private slots:
    // [{tradingDay,volume,openPrice,closePrice,highestBid,lowestBid,amountOfIncrease}, ...]
    void kLineChg(QVector<std::tuple<QDate, double, double, double, double, double, double>>& data);

private:
    kLine getDayKLine(const QDate& date);
    void calAverageLine5();
    void calAverageLine10();
    void calAverageLine20();
    void calAverageLine30();
    void calAverageLine60();
    void calcOther();

private:
    // date time
    QDate m_beginDay;
    QDate m_endDay;
    QDate m_currDay;
    int m_totalDay;

    // price and volume
    double m_highestBid;
    double m_lowestBid;
    double m_maxVolume;

    // scale
    double m_xscale;
    double m_yscale;

    bool m_bCross = false;
    QPoint m_mousePoint;
    int m_lineWidth;
    bool m_isKeyDown = false;
    bool m_isDrawAverageLine = true;

    QVector<kLine> m_data;
};

#endif // KLINEGRID_H
