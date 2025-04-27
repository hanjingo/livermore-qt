#ifndef KVOLUMEGRID_H
#define KVOLUMEGRID_H

#include "autogrid.h"
#include "data.h"

#include <QPoint>

class kVolumeGrid : public AutoGrid
{
    Q_OBJECT

public:
    explicit kVolumeGrid(QWidget* parent);
    void drawYtick();
    void drawVolume();
    void virtual paintEvent(QPaintEvent* event);
    void getIndicator();
    void drawAverageLine(int day);

    void setEndDay(const int endDay) {
        m_endDay = endDay;
        m_beginDay = m_endDay - m_totalDay;
        m_currDay = m_beginDay + m_totalDay /2; }
    void setTotalDay(const int totalDay) {
        m_totalDay = totalDay;
        m_beginDay = m_endDay - m_totalDay;
        m_currDay = m_beginDay + m_totalDay /2; }

private slots:
    void dataUpdated();

private:
    int m_beginDay;
    int m_endDay;
    int m_totalDay;
    int m_currDay;
    double m_maxVolume;
    int m_lineWidth;

    QPoint m_mousePoint;
};

#endif // KVOLUMEGRID_H
