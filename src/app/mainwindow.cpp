#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "config.h"
#include "data.h"
#include "kvolumegrid.h"
#include "klinegrid.h"
#include "showdetail.h"

#include <QSplitter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pgrid = new kVolumeGrid(this);
    pgrid->setObjectName(tr("kline"));
    pgrid->setFocusPolicy(Qt::StrongFocus);

    auto p2 = new KLineGrid(this);
    p2->setFocusPolicy(Qt::StrongFocus);

    QSplitter *splitterMain = new QSplitter(Qt::Vertical, 0);
    QSplitter *splitterLeft = new QSplitter(Qt::Vertical, splitterMain);
    QSplitter *splitterRight = new QSplitter(Qt::Vertical, splitterMain);

    splitterMain->setHandleWidth(1);
    splitterLeft->addWidget(p2);
    splitterRight->addWidget(pgrid);
    this->setCentralWidget(splitterMain);

    resize(1200,800);

    // connect data model and gui
    connect(Data::instance(), SIGNAL(sigkVolumeChg(QVector<std::tuple<QDate, double, double, double>>&)),
            pgrid, SLOT(kVolumeChg(QVector<std::tuple<QDate, double, double, double>>&)));

    connect(Data::instance(), SIGNAL(sigkLineChg(QVector<std::tuple<QDate, double, double, double, double, double, double>>&)),
            pgrid, SLOT(kLineChg(QVector<std::tuple<QDate, double, double, double, double, double, double>>&)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
