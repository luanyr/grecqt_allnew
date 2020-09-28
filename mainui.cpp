#include "mainui.h"
#include "ui_mainui.h"

MainUI::MainUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainUI)
{
    ui->setupUi(this);
    TabWidget_Init();
    m_tcpclient = new QTcpSocket();
    m_pThrdRecv = new RecvThread();
    m_pThrdSend = new SendThread();
    m_tcpclient->setReadBufferSize(16384);

    connect(TW_displayUI, SIGNAL(signal_addwidget(int)), this, SLOT(slot_addwidget(int)));
    connect(this, SIGNAL(UImsg(QString)), this, SLOT(slot_UIstatusshow(QString)));
}

MainUI::~MainUI()
{
    delete ui;
}

void MainUI::TabWidget_Init()
{
//    QPalette palette(this->palette());
//    palette.setColor(QPalette::Background,QColor(173,173,173));
//    this ->setPalette(palette);
    tf = new Top_Form(this);
    TW_displayUI = new myTabWidget(this);
    TW_displayUI->resize(1500, 1200);
    TW_displayUI->addTab(tf, "client");
    TW_displayUI->setTabsClosable(true);

}

myTabWidget::myTabWidget(QWidget* parent):QTabWidget(parent)
{

}

void myTabWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(e->button() == Qt::RightButton)
        emit signal_addwidget(this->count());
}

void MainUI::slot_addwidget(int index)
{
    TW_displayUI->addTab(new Top_Form(this), "client");
}

void MainUI::slot_havaconnected()
{
    emit UImsg(tr("成功链接"));
}

void MainUI::slot_UIstatusshow(QString msg)
{
    this->tf->UIShowMsg(msg);
}
