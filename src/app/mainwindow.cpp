#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "config.h"
#include "data.h"
#include "kvolumegrid.h"
#include "klinegrid.h"
#include "showdetail.h"
#include "tmdivisionchart.h"
#include "libqt/log/logger.h"

#include <QSplitter>
#include <QPushButton>
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

    m_tmDivision = new TimeDivision(this);
    m_tmDivision->setObjectName(tr("tmDivision"));
    m_kLine->setFocusPolicy(Qt::StrongFocus);

    m_detail = new ShowDetail(this);
    // m_detail->setModal(false);

    m_pageBtnGroup = new QButtonGroup(this);
    m_pageBtnGroup->setExclusive(true);
    m_pageBtnGroup->addButton(new QPushButton(this), 0x00); // Time Table Btn
    m_pageBtnGroup->addButton(new QPushButton(this),  0x01); // kDay Btn
    m_pageBtnGroup->buttons().at(0x0)->setText(tr("tmTbl"));
    m_pageBtnGroup->buttons().at(0x0)->setCheckable(true);
    m_pageBtnGroup->buttons().at(0x1)->setText(tr("kDay"));
    m_pageBtnGroup->buttons().at(0x1)->setCheckable(true);

    // connect data model and gui
    connect(Data::instance(), SIGNAL(sigkVolumeChg(QVector<std::tuple<QDate, double, double, double>>&)),
            m_kVolume, SLOT(kVolumeChg(QVector<std::tuple<QDate, double, double, double>>&)));

    connect(Data::instance(), SIGNAL(sigkLineChg(QVector<std::tuple<QDate, double, double, double, double, double, double>>&)),
            m_kLine, SLOT(kLineChg(QVector<std::tuple<QDate, double, double, double, double, double, double>>&)));

    connect(Data::instance(), SIGNAL(sigTickChg(QVector<std::tuple<double, double>>&)),
            m_tmDivision, SLOT(tickChg(QVector<std::tuple<double, double>>&)));

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
    setCentralWidget(m_splitMain);
    resize(1200, 800);
    m_splitMain->setSizes(QList<int>{width() * 3 / 5, width() * 1 / 5, width() * 1 / 5});

    // -----------left area (kLine graph, time table graph, ...)--------------
    m_splitLeft = new QSplitter(Qt::Vertical, m_splitMain);
    m_splitMain->addWidget(m_splitLeft);
    m_splitLeft->setHandleWidth(1);

    // left top button group/kLine/kVolume/tmDivision split
    m_splitLeftTopBtn = new QSplitter(Qt::Horizontal, m_splitLeft);
    m_splitLeftUp = new QSplitter(Qt::Vertical, m_splitLeft);
    m_splitLeftDown = new QSplitter(Qt::Vertical, m_splitLeft);


    // -----------right1 area (detail info)--------------
    m_splitRight1 = new QSplitter(Qt::Horizontal, m_splitMain);
    m_splitMain->addWidget(m_splitRight1);
    m_splitRight1->setHandleWidth(1);

    // tick, detail, bid
    m_splitRight1Tick = new QSplitter(Qt::Vertical, m_splitRight1);
    m_splitRight1Detail = new QSplitter(Qt::Vertical, m_splitRight1);
    m_splitRight1Bid = new QSplitter(Qt::Vertical, m_splitRight1);


    // -----------right2 area (desc info)--------------
    m_splitRight2 = new QSplitter(Qt::Horizontal, m_splitMain);
    m_splitMain->addWidget(m_splitRight2);
    m_splitRight2->setHandleWidth(1);

    m_splitRight2News = new QSplitter(Qt::Vertical, m_splitRight2);


    // -----------draw main----------------------------
    drawMain();

    // default show time table
    emit m_pageBtnGroup->buttonClicked(1);
}

void MainWindow::pageBtnClicked(int idx)
{
    m_pageBtnGroup->button(idx)->setChecked(true);

    drawLeft();
}

void MainWindow::drawMain()
{
    drawLeft();
    drawRight1();
    drawRight2();
}

void MainWindow::drawLeft()
{
    // top button area
    if (m_splitLeftTopBtn->children().empty())
    {
        for (auto btn : m_pageBtnGroup->buttons())
        {
            // background and font
            btn->setMaximumSize({60, 30});
            m_splitLeftTopBtn->addWidget(btn);
        }
        QWidget* topBtnRightSpace = new QWidget(this);
        m_splitLeftTopBtn->addWidget(topBtnRightSpace);
        m_splitLeftTopBtn->setStretchFactor(m_pageBtnGroup->buttons().size(), 1);
    }

    // left up charts
    if (m_splitLeftUp->children().empty())
    {
        m_splitLeftUp->addWidget(m_kLine);
    }

    // left down charts
    if (m_splitLeftDown->children().empty())
    {
        m_splitLeftDown->addWidget(m_kVolume);
    }

    // switch charts
    int idx = m_pageBtnGroup->checkedId();
    switch (idx) {
    // Time Table
    case 0x0: {
        m_splitLeftDown->hide();
        m_splitLeftUp->replaceWidget(0, m_tmDivision);
        break;
    }
    // kDay
    case 0x01: {
        m_splitLeftDown->show();
        m_splitLeftUp->replaceWidget(0, m_kLine);
        break;
    }
    default:{break;}
    }
}

void MainWindow::drawRight1()
{
    // detail
    if (m_splitRight1Detail->children().empty())
    {
        m_splitRight1Detail->addWidget(m_detail);
    }
}

void MainWindow::drawRight2()
{

}
