#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "headers.h"
#define WINDOW_TITLE "存储设备管理软件_J991_V1.0_201110"
MainWidget::MainWidget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    this->setWindowTitle(tr(WINDOW_TITLE));
    TW_MainPage = new QTabWidget(this);
    QString str = tr("界面") + QString("%1").arg(count);
    TW_MainPage->addTab(new MainU1(this), str);
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
    QAction *pNew = new QAction(tr("新建界面"),this);
    menu->addAction(pNew);
    QAction *pDel = new QAction(tr("关闭页面"), this);
    menu->addAction(pDel);
    menu->move(cursor().pos());
    menu->show();
    connect(pNew, SIGNAL(triggered()), this, SLOT(slot_addwidget()));
    connect(pDel, SIGNAL(triggered()), this, SLOT(slot_closewidget()));

}


void MainWidget::slot_addwidget()
{
    count++;
    QString str = tr("界面") + QString("%1").arg(count);
    TW_MainPage->addTab(new MainU1(this), str);
}

void MainWidget::slot_closewidget()
{
    if(TW_MainPage->count() == 1)
    {
        QMessageBox:: StandardButton result = QMessageBox::warning(this, tr("警告"), tr("仅剩一个界面，关闭将退出程序"), QMessageBox::Yes | QMessageBox::No);
        if(result == QMessageBox::Yes)
        {
            this->close();
            QApplication::quit();
        }
    } else {
        int index = TW_MainPage->currentIndex();
        TW_MainPage->removeTab(index);
    }

}
