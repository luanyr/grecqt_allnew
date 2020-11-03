#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "headers.h"
MainWidget::MainWidget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    TW_MainPage = new QTabWidget(this);
    TW_MainPage->addTab(new MainU1(this), tr("客户端"));
    QRect deskRect = QApplication::desktop()->availableGeometry();
    TW_MainPage->resize(deskRect.width(), deskRect.height());
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = new QMenu(this);
    QAction *pNew = new QAction(tr("新建客户端"),this);
    menu->addAction(pNew);
    connect(pNew, SIGNAL(triggered()), this, SLOT(slot_addwidget()));
    QAction *pDel = new QAction(tr("关闭页面"), this);
    menu->addAction(pDel);
    connect(pDel, SIGNAL(triggered()), this, SLOT(slot_closewidget()));
    menu->move(cursor().pos());
    menu->show();
}


void MainWidget::slot_addwidget()
{
    TW_MainPage->addTab(new MainU1(this), tr("客户端"));
}

void MainWidget::slot_closewidget()
{
    int index = TW_MainPage->currentIndex();
    TW_MainPage->removeTab(index);
}
