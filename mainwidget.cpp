#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "headers.h"
MainWidget::MainWidget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    TW_MainPage = new QTabWidget(this);
    TW_MainPage->addTab(new MainU1(this), "client1s");
    TW_MainPage->addTab(new MainU1(this), "client2");
    TW_MainPage->addTab(new MainU1(this), "client2");
    TW_MainPage->resize(1500, 1200);
    TW_MainPage->setTabsClosable(true);
}

MainWidget::~MainWidget()
{
    delete ui;
}
