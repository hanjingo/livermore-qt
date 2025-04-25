#ifndef KVOLUMEGRID_H
#define KVOLUMEGRID_H

#include "autogrid.h"
#include "datafile.h"

#include <QPoint>

class kVolumeGrid : public AutoGrid
{
public:
    explicit kVolumeGrid(QWidget* parent);
    bool readData(QString strFile);
    void initial();
    void drawYtick();
    void drawVolume();
    void virtual paintEvent(QPaintEvent* event);
    void getIndicator();
    void drawAverageLine(int day);

private:
    DataFile mDataFile;

    int beginDay;
    int endDay;
    int totalDay;
    int currentDay;

    double maxVolume;

    QPoint mousePoint;

    int lineWidth;
};

#endif // KVOLUMEGRID_H
