#ifndef STOCKCANVAS_H
#define STOCKCANVAS_H

#include <QWidget>
#include <QMouseEvent>
#include <QVector>
#include <tuple>

#define BORDER_SIZE         10
#define COORDINATE_X1       80
#define COORDINATE_Y1       20
#define COORDINATE_X2       80
#define COORDINATE_Y2       20
#define TEXT_LENGTH         50

struct TickData {
    double price;
    double volume;
};

class StockCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit StockCanvas(QWidget *parent = 0);

private:
    void virtual paintEvent(QPaintEvent* event);
    void virtual resizeEvent(QResizeEvent * event);
    void virtual mouseMoveEvent(QMouseEvent* event);
    void drawBK();
    void drawMouseLine();
    void setLSpace( QString& str, int n );
    void drawTips();
    void drawBorder();
    void drawTimeTblChart();

private slots:
    // [{price,volume}, ...]
    void tickChg(QVector<std::tuple<double, double>>&);

private:
    int m_windowWidth;
    int m_windowHeight;

    double m_xGridMin;
    double m_xGridMax;
    double m_yGridMin;
    double m_yGridMax;

    double m_MaxPrice;
    double m_minPrice;

    double m_xInterval;
    double m_yInterval;

    //int iTipsLength;

    QPoint m_mousePoint;
    double m_priceRate;
    double m_priceStart;
    double m_priceMax;
    double m_priceMin;
    double m_volumeMax;
    QVector<TickData> m_ticks; // capa == 241

signals:

public slots:
};

#endif // STOCKCANVAS_H
