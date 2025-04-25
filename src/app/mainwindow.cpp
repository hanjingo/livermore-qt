#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "config.h"
#include "kvolumegrid.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pgrid = new kVolumeGrid(this);
    pgrid->setObjectName(tr("kline"));
    pgrid->setFocusPolicy(Qt::StrongFocus);

    resize(1200,800);
}

MainWindow::~MainWindow()
{
    delete ui;
}
