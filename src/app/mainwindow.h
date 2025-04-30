#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QSplitter>
#include <QDockWidget>
#include "libqt/ui/autogrid.h"

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

private slots:
    void pageBtnClicked(int idx);

private:
    Ui::MainWindow* ui;

    AutoGrid*    m_kVolume;
    AutoGrid*    m_kLine;
    QWidget*     m_canvas;
    QDockWidget* m_detail;

    QButtonGroup* m_pageBtnGroup;

    QSplitter*    m_splitMain;
    QSplitter*    m_splitLeftUp;
    QSplitter*    m_splitLeftDown;
};
#endif // MAINWINDOW_H
