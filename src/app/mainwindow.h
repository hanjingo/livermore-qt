#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QSplitter>
#include <QDockWidget>
#include <QCloseEvent>
#include <QThread>
#include "libqt/ui/autogrid.h"

#include "handler.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initUI();
    void drawMain();
    void drawLeft();
    void drawRight1();
    void drawRight2();

    void closeEvent(QCloseEvent *event) override
    {
        qDebug() << "fuck1 event=nullptr ? " << (event == nullptr);
        Handler::instance()->quit();
        qDebug() << "fuck2";
        event->accept();
        qDebug() << "fuck3";
    }

private slots:
    void pageBtnClicked(int idx);

private:
    Ui::MainWindow* ui;

    AutoGrid*     m_kVolume;
    AutoGrid*     m_kLine;
    QWidget*      m_tmDivision;
    QDockWidget*  m_detail;

    QButtonGroup* m_pageBtnGroup;

    // layout
    QSplitter*    m_splitMain;
    // left
    QSplitter*    m_splitLeft;
    QSplitter*    m_splitLeftTopBtn;
    QSplitter*    m_splitLeftUp;
    QSplitter*    m_splitLeftDown;
    // right1
    QSplitter*    m_splitRight1;
    QSplitter*    m_splitRight1Tick;
    QSplitter*    m_splitRight1Detail;
    QSplitter*    m_splitRight1Bid;
    // right2
    QSplitter*    m_splitRight2;
    QSplitter*    m_splitRight2News;
};
#endif // MAINWINDOW_H
