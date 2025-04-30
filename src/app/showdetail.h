#ifndef SHOWDETAIL_H
#define SHOWDETAIL_H


#include <QWidget>
#include <QDockWidget>
#include <QDialog>

class ShowDetail : public QDockWidget
{
    Q_OBJECT

public:
    explicit ShowDetail(QWidget* parent);
    void virtual paintEvent(QPaintEvent* event);
    void virtual drawBK();
    void drawStr();

private:
    QString m_time = "";
    double  m_currentPrice = 0;
    double  m_openingPrice = 0;
    double  m_highestBid = 0;
    double  m_lowestBid =0;
    double  m_closeingPrice =0;
    double  m_amountOfIncrease=0;
    double  m_amountOfAmplitude =0;
    QString m_totalVolume = "";
    QString m_totalAmount = "";
    double  m_turnoverRate = 0;

    QColor m_timeColor;
    QColor m_currentPriceColor;
    QColor m_openingPriceColor;
    QColor m_highestBidColor;
    QColor m_lowestBidColor;
    QColor m_closeingPriceColor;
    QColor m_amountOfIncreaseColor;
    QColor m_amountOfAmplitudeColor;
    QColor m_totalVolumeColor;
    QColor m_totalAmountColor;
    QColor m_turnoverRateColor;

public :
  void receiveParams(QString time, QColor timeColor,
                     double currentPrice, QColor currentPriceColor,
                     double openingPrice, QColor openingPriceColor,
                     double highestBid, QColor highestBidColor,
                     double lowestBid, QColor lowestBidColor,
                     double closeingPrice, QColor closeingPriceColor,
                     double amountOfIncrease, QColor amountOfIncreaseColor,
                     double amountOfAmplitude, QColor amountOfAmplitudeColor,
                     QString totalVolume, QColor totalVolumeColor,
                     QString totalAmount, QColor totalAmountColor,
                     double turnoverRate, QColor turnoverRateColor);
};

#endif // SHOWDETAIL_H
