#include "mainu1.h"
#include "ui_mainu1.h"

MainU1::MainU1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainU1)
{
    ui->setupUi(this);
    this->resize(2500, 2000);
    m_connectstatus = false;
    m_tcpclient = new QTcpSocket();
    m_pThrdRecv = new RecvThread();
    m_pThrdSend = new SendThread();
    m_tcpclient->setReadBufferSize(16384);
    tf = new Top_Form(this);
    tf->move(5, 5);
    ChannelPageInit();
    connect(this, SIGNAL(UImsg(QString)), this, SLOT(slot_UIstatusshow(QString)));
    connect(this->tf, SIGNAL(signals_pbtype(int)), this, SLOT(slot_handlesignal(int)));
    connect(this->tf, SIGNAL(signals_cmtype(int)), this, SLOT(slot_handlecmsignals(int)));
    connect(this->m_tcpclient, SIGNAL(connected()), this, SLOT(slot_havaconnected()));
    connect(this->m_tcpclient, SIGNAL(disconnected()), this, SLOT(slot_havedisconnected()));
    connect(this, SIGNAL(Signal_Usrlogin(QString, QString)), this->m_pThrdSend, SLOT(UserLoginSlot(QString,QString)));
    connect(this, SIGNAL(Signal_Usrlogout()), this->m_pThrdSend, SLOT(UserQuitSlot()));
    connect(this->m_pThrdRecv, SIGNAL(UserLoginSignal(QByteArray)), this, SLOT(slot_userlogin(QByteArray)));
}

MainU1::~MainU1()
{
    delete ui;
}


void MainU1::slot_addwidget(int index)
{

}

void MainU1::slot_havaconnected()
{
    m_connectstatus = true;
    m_pThrdRecv->SetSocketWord(m_tcpclient);
    m_pThrdSend->SetSocketWord(m_tcpclient);
    m_pThrdRecv->start();
    m_pThrdRecv->setPriority(QThread::HighPriority);
    this->tf->CuStatusNet(statusnet_connected);
    emit UImsg(tr("成功连接"));

}

void MainU1::slot_havedisconnected()
{
    m_connectstatus = false;
    this->tf->CuStatusNet(statusnet_disconnected);
    emit UImsg(tr("断开连接"));

}

void MainU1::slot_UIstatusshow(QString msg)
{
    this->tf->UIShowMsg(msg);
}

void MainU1::connectserver()
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

void MainU1::disconnectserver()
{
    if(m_connectstatus)
        m_tcpclient->disconnectFromHost();
}


void MainU1::slot_handlesignal(int type)
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
    case PBuserlogout:
        userlogout();
        break;
    case PBturnoffapp:
        slot_closeapp();
        break;
    default:
        break;
    }
}

void MainU1::userlogin()
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

void MainU1::userlogout()
{
    if(!m_bLogined)
    {
        emit UImsg(tr("警告：尚未登陆，无法退出！"));
        return;
    }
    this->tf->CuStatusUsr(QString(" "), statususr_logout);
    m_bLogined = false;
    emit Signal_Usrlogout();
}

void MainU1::slot_userlogin(QByteArray data)
{
    RW_MSGR__USR_LOGIN *pMsg = (RW_MSGR__USR_LOGIN *)data.data();
    if((pMsg->result == LOGIN_OK) || (m_bLogined))
    {
        this->tf->CuStatusUsr(m_infoStore.m_strCurUser, statususr_login);
        emit UImsg(tr("用户")+m_infoStore.m_strCurUser+tr("登录成功。"));
        m_bLogined = true;
    } else {
        m_bLogined = false;
        emit UImsg(tr("用户")+m_infoStore.m_strCurUser+tr("登录失败！错误代码：%1。").arg(pMsg->result));
    }
}

void MainU1::slot_closeapp()
{
    this->close();
    QApplication::quit();
}

void MainU1::cmconfirm()
{
    m_ListStrChannel.clear();
    for(int i = 0; i < this->tf->setchannel()->columnCount(); i++)
    {
        m_ListStrChannel << this->tf->setchannel()->item(1, i)->text();
    }
    UpdateChannelName();
}

void MainU1::cmsetdefault()
{
    m_ListStrChannel = SetDefaultChannelName();
    for(int i = 0; i < this->tf->setchannel()->columnCount(); i++)
    {
        QTableWidgetItem *item1 = new QTableWidgetItem(m_ListStrChannel.at(i));
        this->tf->setchannel()->setItem(1, i, item1);
    }
    UpdateChannelName();
}

void MainU1::UpdateChannelName()
{
    m_infoStore.m_listStrChannel = m_ListStrChannel;
    for(int i=0; i<m_ListStrChannel.count(); i++)
    {
        QString t = m_ListStrChannel.at(i);
        m_listStrLink.replace(i, t);
        m_listStrBit.replace(i, t);
    }
    for(int i=0; i<m_listChkRec.count(); i++)
    {
        m_listChkRec.at(i)->setToolTip(m_ListStrChannel.at(i));
    }
    for(int i=0; i<m_listChkReplay.count(); i++)
    {
        m_listChkReplay.at(i)->setToolTip(m_ListStrChannel.at(i));
    }
    for(int i=0; i<m_listChkDownload.count(); i++)
    {
        m_listChkDownload.at(i)->setToolTip(m_ListStrChannel.at(i));
    }
    for(int i=0; i<m_listChkSelect.count(); i++)
    {
        m_listChkSelect.at(i)->setToolTip(m_ListStrChannel.at(i));
    }
}

void MainU1::slot_handlecmsignals(int type)
{
    switch (type) {
    case cmsignal_setchanneldefault:
        cmsetdefault();
        break;
    case cmsignal_confirm:
        cmconfirm();
        break;
    default:
        break;
    }
}

void MainU1::ChannelPageInit()
{
    m_ListStrChannel = SetDefaultChannelName();
    QStringList a;
    a << "A" << "B" << "C" << "D" << "E" << "F" << "G"
      << "H" << "I" << "J" << "K" << "L" << "M" << "N" << "O";
    this->tf->setchannel()->setColumnCount(a.count());
    this->tf->setchannel()->setRowCount(2);

    QStringList heads;
    heads << tr("标识") << tr("通道名");
    this->tf->setchannel()->setVerticalHeaderLabels(heads);


    QHeaderView *headerView = this->tf->setchannel()->horizontalHeader();
    headerView->setHidden(true);

    this->tf->setchannel()->setEditTriggers(QAbstractItemView::AllEditTriggers);
    this->tf->setchannel()->setSelectionBehavior(QAbstractItemView::SelectColumns);  //单击选择一列
    this->tf->setchannel()->setSelectionMode(QAbstractItemView::SingleSelection); //设置只能选择一列，不能多行选中
    this->tf->setchannel()->setAlternatingRowColors(true);

    this->tf->setchannel()->horizontalHeader()->setStretchLastSection(true);
    this->tf->setchannel()->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    for(int i = 0; i < this->tf->setchannel()->columnCount();i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem(a.at(i));
        item->setFlags(Qt::NoItemFlags);
        this->tf->setchannel()->setItem(0, i, item);
        QTableWidgetItem *item1 = new QTableWidgetItem(m_ListStrChannel.at(i));
        this->tf->setchannel()->setItem(1, i, item1);
        this->tf->setchannel()->setColumnWidth(i, (this->tf->setchannel()->width()-this->tf->setchannel()->verticalHeader()->width()-60)/this->tf->setchannel()->columnCount());
    }
}

void MainU1::StatusTableInit()
{
    this->tf->SetTlwDevSta()->setColumnCount(2);
}

