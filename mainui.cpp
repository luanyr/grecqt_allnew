#if 0
#include "mainui.h"
#include "ui_mainui.h"

MainUI::MainUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainUI)
{
    ui->setupUi(this);
    TabWidget_Init();
    m_connectstatus = false;
    m_tcpclient = new QTcpSocket();
    m_pThrdRecv = new RecvThread();
    m_pThrdSend = new SendThread();
    m_tcpclient->setReadBufferSize(16384);

    connect(TW_displayUI, SIGNAL(signal_addwidget(int)), this, SLOT(slot_addwidget(int)));
    connect(this, SIGNAL(UImsg(QString)), this, SLOT(slot_UIstatusshow(QString)));
    connect(this->tf, SIGNAL(signals_pbtype(int)), this, SLOT(slot_handlesignal(int)));
    connect(this->m_tcpclient, SIGNAL(connected()), this, SLOT(slot_havaconnected()));
    connect(this->m_tcpclient, SIGNAL(disconnected()), this, SLOT(slot_havedisconnected()));
    connect(this, SIGNAL(Signal_Usrlogin(QString, QString)), this->m_pThrdSend, SLOT(UserLoginSlot(QString,QString)));
    connect(this, SIGNAL(Signal_Usrlogout()), this->m_pThrdSend, SLOT(UserQuitSlot()));
    connect(this->m_pThrdRecv, SIGNAL(UserLoginSignal(QByteArray)), this, SLOT(slot_userlogin(QByteArray)));
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
    TW_displayUI->addTab(tf, "client");
}

void MainUI::slot_havaconnected()
{
    m_connectstatus = true;
    emit UImsg(tr("成功连接"));
}

void MainUI::slot_havedisconnected()
{
    m_connectstatus = false;
    emit UImsg(tr("断开连接"));
}

void MainUI::slot_UIstatusshow(QString msg)
{
    this->tf->UIShowMsg(msg);
}

void MainUI::connectserver()
{
    if(!m_connectstatus)
    {
        m_infoStore.m_uPort = this->tf->Getipport().toUInt(0, 10);
        m_infoStore.m_szIP = this->tf->Getipaddr().toLatin1();
        qDebug() << m_infoStore.m_uPort;
        qDebug() << m_infoStore.m_szIP;
        m_tcpclient->connectToHost(m_infoStore.m_szIP,m_infoStore.m_uPort);
        emit UImsg(tr("正在连接")+m_infoStore.m_szIP+tr(":")+QString::number(m_infoStore.m_uPort,10)+tr("..."));
    }
}

void MainUI::disconnectserver()
{
    if(m_connectstatus)
        m_tcpclient->disconnectFromHost();
}


void MainUI::slot_handlesignal(int type)
{
    switch (type) {
    case PBconnect:
        connectserver();
        break;
    case PBdisconnect:
        disconnectserver();
        break;
    case PBuserlogin:
        userlogin();
        break;
    default:
        break;
    }
}

void MainUI::userlogin()
{
    if(!m_loginstatus)
    {
        emit UImsg(tr("警告：必须退出当前用户才能重新登录！"));
        return;
    }
    m_infoStore.m_strCurUser = this->tf->Getusrname();
    m_infoStore.m_strCurPw = this->tf->Getuserpswd();

    emit Signal_Usrlogin(m_infoStore.m_strCurUser, m_infoStore.m_strCurPw);
}

void MainUI::userlogout()
{
    if(m_loginstatus)
    {
        emit UImsg(tr("警告：尚未登陆，无法退出！"));
        return;
    }
    emit Signal_Usrlogout();
}

void MainUI::slot_userlogin(QByteArray data)
{
    RW_MSGR__USR_LOGIN *pMsg = (RW_MSGR__USR_LOGIN *)data.data();
//    if((pMsg->result == LOGIN_OK) || (m_bLogined))
//        {
//            m_labelStatUser->setText(tr("当前用户: ")+m_infoStore.m_strCurUser);
//            emit Uimsg(tr("用户")+m_infoStore.m_strCurUser+tr("登录成功。"));
//            m_bLogined = true;
//            uiChgToolEnable();
//        }
}
#endif
