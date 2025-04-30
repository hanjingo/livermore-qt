#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "config.h"
#include "data.h"
#include "kvolumegrid.h"
#include "klinegrid.h"
#include "showdetail.h"
#include "stockcanvas.h"
#include "libcpp/log/logger.hpp"

#include <QSplitter>
#include <QToolButton>
#include <QSpacerItem>
#include <QPalette>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_kVolume = new kVolumeGrid(this);
    m_kVolume->setObjectName(tr("kVolume"));
    m_kVolume->setFocusPolicy(Qt::StrongFocus);

    m_kLine = new KLineGrid(this);
    m_kLine->setObjectName(tr("kLine"));
    m_kLine->setFocusPolicy(Qt::StrongFocus);

    m_canvas = new StockCanvas(this);
    m_canvas->setObjectName(tr("tmTbl"));

    m_detail = new ShowDetail(this);
    // m_detail->setModal(false);

    m_pageBtnGroup = new QButtonGroup(this);
    m_pageBtnGroup->addButton(new QToolButton(this), 0x00); // Time Table Btn
    m_pageBtnGroup->addButton(new QToolButton(this),  0x01); // kDay Btn
    m_pageBtnGroup->buttons().at(0x0)->setText(tr("tmTbl"));
    m_pageBtnGroup->buttons().at(0x1)->setText(tr("kDay"));

    // connect data model and gui
    connect(Data::instance(), SIGNAL(sigkVolumeChg(QVector<std::tuple<QDate, double, double, double>>&)),
            m_kVolume, SLOT(kVolumeChg(QVector<std::tuple<QDate, double, double, double>>&)));

    connect(Data::instance(), SIGNAL(sigkLineChg(QVector<std::tuple<QDate, double, double, double, double, double, double>>&)),
            m_kLine, SLOT(kLineChg(QVector<std::tuple<QDate, double, double, double, double, double, double>>&)));

    connect(Data::instance(), SIGNAL(sigTickChg(QVector<std::tuple<double, double>>&)),
            m_canvas, SLOT(tickChg(QVector<std::tuple<double, double>>&)));

    connect(m_pageBtnGroup, SIGNAL(buttonClicked(int)), this, SLOT(pageBtnClicked(int)));

    initUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUI()
{
    this->setWindowTitle("livermore-qt");

    m_splitMain = new QSplitter(Qt::Horizontal, 0);
    m_splitMain->setHandleWidth(1);
    this->setCentralWidget(m_splitMain);
    resize(1200, 800);
    m_splitMain->setSizes(QList<int>{this->width() * 3 / 5, this->width() * 1 / 5, this->width() * 1 / 5});

    // -----------left area (kLine graph, time table graph, ...)--------------
    QSplitter* splitLeft = new QSplitter(Qt::Vertical, m_splitMain);
    m_splitMain->addWidget(splitLeft);
    splitLeft->setHandleWidth(1);

    // left button group
    QSplitter* topBtnleftSplit = new QSplitter(Qt::Horizontal, splitLeft);
    topBtnleftSplit->setHandleWidth(1);
    for (auto btn : m_pageBtnGroup->buttons())
    {
        // background and font
        btn->setMaximumSize({60, 30});
        topBtnleftSplit->addWidget(btn);
    }
    QWidget* topBtnRightSpace = new QWidget(this);
    topBtnleftSplit->addWidget(topBtnRightSpace);
    topBtnleftSplit->setStretchFactor(m_pageBtnGroup->buttons().size(), 1);

    // kLine/kVolume split
    m_splitLeftUp = new QSplitter(Qt::Vertical, splitLeft);
    m_splitLeftDown = new QSplitter(Qt::Vertical, splitLeft);
    this->m_splitLeftUp->addWidget(m_kLine);
    this->m_splitLeftDown->addWidget(m_kVolume);


    // -----------right1 area (detail info)--------------
    QSplitter* splitRight1 = new QSplitter(Qt::Horizontal, m_splitMain);
    m_splitMain->addWidget(splitRight1);
    splitRight1->setHandleWidth(1);

    QSplitter* tick = new QSplitter(Qt::Vertical, splitRight1);
    QSplitter* detail = new QSplitter(Qt::Vertical, splitRight1);
    QSplitter* bid = new QSplitter(Qt::Vertical, splitRight1);
    detail->addWidget(m_detail);


    // -----------right2 area (desc info)--------------
    QSplitter* splitRight2 = new QSplitter(Qt::Horizontal, m_splitMain);
    m_splitMain->addWidget(splitRight2);
    splitRight2->setHandleWidth(1);

    QSplitter* news = new QSplitter(Qt::Vertical, splitRight2);


    // default show time table
    emit m_pageBtnGroup->buttonClicked(0);
}

void MainWindow::pageBtnClicked(int idx)
{
    LOG_DEBUG("emit idx={}", idx);
    if(!m_pageBtnGroup->buttons().at(idx)->isChecked())
    {
        m_pageBtnGroup->buttons().at(idx)->setChecked(true);
    }

    switch (idx) {
    // Time Table
    case 0x0: {
        m_splitLeftUp->replaceWidget(0, m_canvas);
        m_splitLeftDown->replaceWidget(0, m_kVolume);
        break;
    }
    // kDay
    case 0x01: {
        m_splitLeftUp->replaceWidget(0, m_kLine);
        m_splitLeftDown->replaceWidget(0, m_kVolume);
        break;
    }
    default:{break;}
    }
}
