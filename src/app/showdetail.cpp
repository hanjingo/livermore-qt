#include "showdetail.h"

#include <QDockWidget>
#include <QPainter>
#include <QFont>

ShowDetail::ShowDetail(QWidget* parent) : QDockWidget(parent)
{

    QFont ft;
    ft.setPointSize(14);
    this->setFont(ft);
}

void ShowDetail::drawBK()
{
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Window,QColor("#000000"));
    this->setPalette(palette);
}

void ShowDetail::paintEvent(QPaintEvent *event)
{
    drawBK();
    drawStr();
}

void ShowDetail::drawStr()
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(QColor("#FFFFFF"));
    painter.setPen(pen);

    QRect rectTime( 20,20,100,30);
    painter.drawText(rectTime,tr("time"));

    pen.setColor(m_timeColor);
    painter.setPen(pen);
    QRect rectTime2( 20,50,100,30);
    QString str = m_time.mid(1,10);
    painter.drawText(rectTime2,str);

    pen.setColor(QColor("#FFFFFF"));
    painter.setPen(pen);
    QRect rectTime3(20,80,100,30);
    painter.drawText(rectTime3,tr("currentValue"));

    pen.setColor(m_currentPriceColor);
    painter.setPen(pen);
    QRect rectTime4(20,110,100,30);
    painter.drawText(rectTime4,str.sprintf("%.2f", m_currentPrice));

    pen.setColor(QColor("#FFFFFF"));
    painter.setPen(pen);
    QRect rectTime5(20,140,100,30);
    painter.drawText(rectTime5,tr("opening"));

    pen.setColor(m_openingPriceColor);
    painter.setPen(pen);
    QRect rectTime6(20,170,100,30);
    painter.drawText(rectTime6,str.sprintf("%.2f", m_openingPrice));

    pen.setColor(QColor("#FFFFFF"));
    painter.setPen(pen);
    QRect rectTime7(20,200,100,30);
    painter.drawText(rectTime7,tr("highestBid"));

    pen.setColor(m_highestBidColor);
    painter.setPen(pen);
    QRect rectTime8(20,230,100,30);
    painter.drawText(rectTime8,str.sprintf("%.2f", m_highestBid));

    pen.setColor(QColor("#FFFFFF"));
    painter.setPen(pen);
    QRect rectTime9(20,260,100,30);
    painter.drawText(rectTime9,tr("lowestBid"));

    pen.setColor(m_lowestBidColor);
    painter.setPen(pen);
    QRect rectTime10(20,290,100,30);
    painter.drawText(rectTime10,str.sprintf("%.2f", m_lowestBid));

    pen.setColor(QColor("#FFFFFF"));
    painter.setPen(pen);
    QRect rectTime11(20,320,100,30);
    painter.drawText(rectTime11,tr("closeingPrice"));

    pen.setColor(m_closeingPriceColor);
    painter.setPen(pen);
    QRect rectTime12(20,350,100,30);
    painter.drawText(rectTime12,str.sprintf("%.2f", m_closeingPrice));

    pen.setColor(QColor("#FFFFFF"));
    painter.setPen(pen);
    QRect rectTime13(20,380,100,30);
    painter.drawText(rectTime13,tr("amountOfIncrease"));

    pen.setColor(m_amountOfIncreaseColor);
    painter.setPen(pen);
    QRect rectTime14(20,410,100,30);
    painter.drawText(rectTime14,str.sprintf("%.2f%", m_amountOfIncrease));

    pen.setColor(QColor("#FFFFFF"));
    painter.setPen(pen);
    QRect rectTime15(20,440,100,30);
    painter.drawText(rectTime15,tr("amountOfAmplitude"));

    pen.setColor(m_amountOfAmplitudeColor);
    painter.setPen(pen);
    QRect rectTime16(20,470,100,30);
    painter.drawText(rectTime16,str.sprintf("%.2f%", m_amountOfAmplitude));

    pen.setColor(QColor("#FFFFFF"));
    painter.setPen(pen);
    QRect rectTime17(20,500,100,30);
    painter.drawText(rectTime17,tr("totalVolume"));

    pen.setColor(m_totalVolumeColor);
    painter.setPen(pen);
    QRect rectTime18(20,530,100,30);
    str = m_totalVolume;
    str = str.mid(1, str.length());
    str = str.mid(0, str.length() - 1);
    painter.drawText(rectTime18,str);

    pen.setColor(QColor("#FFFFFF"));
    painter.setPen(pen);
    QRect rectTime19(20,560,100,30);
    painter.drawText(rectTime19,tr("totalAmount"));

    pen.setColor(m_totalAmountColor);
    painter.setPen(pen);
    QRect rectTime20(20,590,100,30);

    str = m_totalAmount;
    str = str.mid(1, str.length());
    str = str.mid(0, str.length()-1);
    painter.drawText(rectTime20, str);

    pen.setColor(QColor("#FFFFFF"));
    painter.setPen(pen);
    QRect rectTime21(20,620,100,30);
    painter.drawText(rectTime21,tr("turnoverRate"));

    pen.setColor(m_turnoverRateColor);
    painter.setPen(pen);
    QRect rectTime22(20,650,100,30);
    painter.drawText(rectTime22,str.sprintf("%.2f", m_turnoverRate));

    // QRect rectTime4( 20,50,100,30);
    // painter.drawText(rectTime4,str.sprintf("%.2f", m_currentPrice));
}

void ShowDetail::receiveParams(QString time, QColor timeColor,
                               double currentPrice, QColor currentPriceColor,
                               double openingPrice, QColor openingPriceColor,
                               double highestBid, QColor highestBidColor,
                               double lowestBid, QColor lowestBidColor,
                               double closeingPrice, QColor closeingPriceColor,
                               double amountOfIncrease, QColor amountOfIncreaseColor,
                               double amountOfAmplitude, QColor amountOfAmplitudeColor,
                               QString totalVolume, QColor totalVolumeColor,
                               QString totalAmount, QColor totalAmountColor,
                               double turnoverRate, QColor turnoverRateColor)
{
    this->m_time = time;
    this->m_currentPrice = currentPrice;
    this->m_openingPrice = openingPrice;
    this->m_highestBid = highestBid;
    this->m_lowestBid = lowestBid;
    this->m_closeingPrice = closeingPrice;
    this->m_amountOfIncrease = amountOfIncrease;
    this->m_amountOfAmplitude = amountOfAmplitude;
    this->m_totalVolume = totalVolume;
    this->m_totalAmount = totalAmount;
    this->m_turnoverRate = turnoverRate;

    this->m_timeColor = timeColor;
    this->m_currentPriceColor = currentPriceColor;
    this->m_openingPriceColor = openingPriceColor;
    this->m_highestBidColor = highestBidColor;
    this->m_lowestBidColor = lowestBidColor;
    this->m_closeingPriceColor = closeingPriceColor;
    this->m_amountOfIncreaseColor = amountOfIncreaseColor;
    this->m_amountOfAmplitudeColor = amountOfAmplitudeColor;
    this->m_totalVolumeColor = totalVolumeColor;
    this->m_totalAmountColor = totalAmountColor;
    this->m_turnoverRateColor = turnoverRateColor;

    update();
}
