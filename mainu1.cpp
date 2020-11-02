#include "mainu1.h"
#include "ui_mainu1.h"

#define BIT_NUM     (16)
#define STOR_NUM    (2)

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
    UiInit();
    connect(this->m_tcpclient, SIGNAL(connected()), this, SLOT(slot_havaconnected()));
    connect(this->m_tcpclient, SIGNAL(disconnected()), this, SLOT(slot_havedisconnected()));
    connect(this, SIGNAL(UImsg(QString)), this, SLOT(slot_UIstatusshow(QString)));

    connect(this, SIGNAL(HeartBeatSignal()), m_pThrdSend, SLOT(HeartBeatSlot()));
    connect(this, SIGNAL(SetTimeSignal(QDateTime)), m_pThrdSend, SLOT(SetTimeSlot(QDateTime)));
    connect(this, SIGNAL(MBitSignal()), m_pThrdSend, SLOT(BitSlot()));
    connect(this, SIGNAL(WipeSignal()), m_pThrdSend, SLOT(WipeSlot()));
    connect(this, SIGNAL(Signal_Usrlogout()), this->m_pThrdSend, SLOT(UserQuitSlot()));
    connect(this, SIGNAL(Signal_Usrlogin(QString, QString)), this->m_pThrdSend, SLOT(UserLoginSlot(QString,QString)));
    connect(this, SIGNAL(UserNewSignal(QString, QString)), m_pThrdSend, SLOT(UserNewSlot(QString, QString)));
    connect(this, SIGNAL(UserChgPwSignal(QString, QString)), m_pThrdSend, SLOT(UserChgPwSlot(QString, QString)));
    connect(this, SIGNAL(UserDelSignal(QString, QString)), m_pThrdSend, SLOT(UserDelSlot(QString, QString)));
     connect(this, SIGNAL(RecConSignal(UINT32)), m_pThrdSend, SLOT(RecConSlot(UINT32)));
    connect(this, SIGNAL(Signal_InqStatus()), this->m_pThrdSend, SLOT(InqStatusSlot()));
    connect(this, SIGNAL(Signal_Heartbeat()), this->m_pThrdSend, SLOT(HeartBeatSlot()));
    connect(this, SIGNAL(RecModeSignal(UINT32)), m_pThrdSend, SLOT(RecModeSlot(UINT32)));
    connect(this, SIGNAL(WriteProtectSignal(QVariant)), m_pThrdSend, SLOT(WriteProtectSlot(QVariant)));
    connect(this, SIGNAL(WriteProtectOffSignal(QVariant)), m_pThrdSend, SLOT(WriteProtectOffSlot(QVariant)));
    connect(this, SIGNAL(UnInstStartSignal(QVariant)), m_pThrdSend, SLOT(UnInstStartSlot(QVariant)));
    connect(this, SIGNAL(UnInstStopSignal()), m_pThrdSend, SLOT(UnInstStopSlot()));
    connect(this, SIGNAL(PlaybackPauseSignal(UINT32)), m_pThrdSend, SLOT(PlaybackPauseSlot(UINT32)));
    connect(this, SIGNAL(PlaybackStartSignal(QVariant)), m_pThrdSend, SLOT(PlaybackStartSlot(QVariant)));
    connect(this, SIGNAL(PlaybackStopSignal()), m_pThrdSend, SLOT(PlaybackStopSlot()));
    connect(this, SIGNAL(DirSignal(QVariant)), m_pThrdSend, SLOT(DirSlot(QVariant)), Qt::DirectConnection);
    connect(this, SIGNAL(FileDelSignal(QVariant)), m_pThrdSend, SLOT(FileDelSlot(QVariant)));
    connect(this, SIGNAL(UpdateSignal()), m_pThrdSend, SLOT(UpdateSlot()));
    connect(this, SIGNAL(ResetSignal()), m_pThrdSend, SLOT(ResetSlot()));
    connect(this, SIGNAL(SelfDestorySignal()), m_pThrdSend, SLOT(SelfDestorySlot()));
    connect(this, SIGNAL(PowerDownSignal()), m_pThrdSend, SLOT(PowerDownSlot()));
    connect(this, SIGNAL(UnmountSignal(bool)), m_pThrdSend, SLOT(UnmountSlot(bool)));
    connect(this, SIGNAL(MountSignal(bool)), m_pThrdSend, SLOT(MountSlot(bool)));
    connect(this, SIGNAL(SetfileszSignal(UINT32)), m_pThrdSend, SLOT(SetfileSzSlot(UINT32)));

    connect(this->m_pThrdRecv, SIGNAL(DirSignal(QList<CFileAttrib>)), this, SLOT(DirSlot(QList<CFileAttrib>)), Qt::DirectConnection);
    connect(this->m_pThrdRecv, SIGNAL(UserLoginSignal(QByteArray)), this, SLOT(slot_userlogin(QByteArray)));
    connect(this->m_pThrdRecv, SIGNAL(InqStatusSignal(QByteArray)), this, SLOT(slot_inqstatus(QByteArray)));
    connect(this->m_pThrdRecv, SIGNAL(RecvMsgToui(QString)),this,SLOT(slot_UIstatusshow(QString)));
    connect(this->m_pThrdRecv, SIGNAL(AllAckSignal(QByteArray)),this,SLOT(AllAckSlot(QByteArray)));
    connect(this->m_pThrdRecv, SIGNAL(AllCmdUiSignal(QByteArray, QByteArray)), this, SLOT(AllCmdUiSlot(QByteArray, QByteArray)));
    connect(this->m_pThrdRecv, SIGNAL(BitSignal(QByteArray)), this, SLOT(BitSlot(QByteArray)));
    connect(this->m_pThrdRecv, SIGNAL(WipeSignal(QByteArray)), this, SLOT(WipeSlot(QByteArray)));
    connect(this->m_pThrdSend, SIGNAL(SendMsgToui(QString)), this, SLOT(slot_UIstatusshow(QString)));
    connect(this->tf, SIGNAL(signals_pbtype(int)), this, SLOT(slot_handlesignal(int)));
    connect(this->tf, SIGNAL(signals_cmtype(int)), this, SLOT(slot_handlecmsignals(int)));
    connect(this->tf, SIGNAL(signals_rcdmtype(int)), this, SLOT(slot_handlercdmsignal(int)));
    connect(this->tf, SIGNAL(signals_rpmtype(int)), this, SLOT(slot_handlerpmsignals(int)));
    connect(this->tf, SIGNAL(signals_smtype(int)), this, SLOT(slot_handlesmsignals(int)));
    connect(this->tf, SIGNAL(signals_mgmtype(int)), this, SLOT(slot_handlemgmsignals(int)));
    connect(this->tf, SIGNAL(signals_umtype(int)), this, SLOT(slot_handleumsignals(int)));
    connect(this->tf, SIGNAL(signals_spmtype(int)), this, SLOT(slot_handlespmsignals(int)));
    connect(this->tf, SIGNAL(signals_cutype(int)), this, SLOT(slot_handlecusignals(int)));


}

MainU1::~MainU1()
{
    delete ui;
}

myTabWidget::myTabWidget(QWidget* parent):QTabWidget(parent)
{

}

void myTabWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(e->button() == Qt::RightButton)
        emit signal_addwidget(this->count());
}

void MainU1::UiInit()
{
    m_ListStrChannel = m_infoStore.m_listStrChannel;
    m_listStrLink << m_ListStrChannel << tr("磁盘箱");
    m_listStrBit << m_ListStrChannel << tr("磁盘箱") << tr("板载电子盘") << tr("扩展电子盘");;

    StatusTableInit();
    BitListInit();
    StartPageInit();
    ReplayPageInit();
    SelectPageInit();
    OtherInit();
    ChannelPageInit();
    RecPageInit();
    uiChgToolEnable(false);
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
    m_connectstatus = true;
    m_bLogined = false;
    uiChgToolEnable(m_connectstatus);
    m_bAlive = true;
    m_bNeedRecMode = true;
    QTimer *pTimer = new QTimer();
    connect(pTimer, SIGNAL(timeout()), this, SLOT(TimerHeartBeatFunction()));
    pTimer->start(1000);
    userlogin();//连接上后自动登陆
}

void MainU1::TimerHeartBeatFunction()
{
    emit HeartBeatSignal();
}

void MainU1::slot_havedisconnected()
{
    m_connectstatus = false;
    this->tf->CuStatusNet(statusnet_disconnected);
    uiChgToolEnable(false);
    emit UImsg(tr("断开连接"));
    if(!m_bManual)
    {
        emit UImsg(tr("TCP连接失败,正在重试")+QString::number(m_uRetry,10)+tr("次"));
        Mysleep(5000);
        m_uRetry++;
        connectserver();
    }
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
    m_bManual = true;
    if(m_connectstatus)
        m_tcpclient->disconnectFromHost();
}

void MainU1::displayError()
{
    emit UImsg(tr("网络错误原因：") + m_tcpclient->errorString());
    emit UImsg(tr("TCP连接失败,正在重试")+QString::number(m_uRetry,10)+tr("次"));
    Mysleep(5000);
    connectserver();
}

void MainU1::slot_handlesignal(int type)
{
    switch (type) {
    case PBconnect:
        m_bManual = false;
        m_uRetry = 1;
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
    uiChgToolEnable(false);
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
        uiChgToolEnable(true);

    } else {
        m_bLogined = false;
        emit UImsg(tr("用户")+m_infoStore.m_strCurUser+tr("登录失败！错误代码：%1。").arg(pMsg->result));
        uiChgToolEnable(false);
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

void MainU1::UpdateProgressSlot(bool bShow, UINT32 percent)
{
    if(bShow) {
        this->tf->SetCuProgress()->show();
        this->tf->SetCuProgress()->setValue(percent);  // 当前进度
    } else {
        this->tf->SetCuProgress()->hide();
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
    this->tf->SetTlwDevSta()->setRowCount(11+m_listStrLink.count());

    this->tf->SetTlwDevSta()->setColumnWidth(0, this->tf->SetTlwDevSta()->width()/this->tf->SetTlwDevSta()->columnCount()-30);
    this->tf->SetTlwDevSta()->setColumnWidth(1, this->tf->SetTlwDevSta()->width()/this->tf->SetTlwDevSta()->columnCount()-30);
    QStringList heads;
    heads << tr("状态项") << tr("明细");
    this->tf->SetTlwDevSta()->setHorizontalHeaderLabels(heads);
    this->tf->SetTlwDevSta()->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->tf->SetTlwDevSta()->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->tf->SetTlwDevSta()->setSelectionMode(QAbstractItemView::SingleSelection);
    this->tf->SetTlwDevSta()->setAlternatingRowColors(true);

    for(int i = 0; i < m_listStrLink.count(); i++)
    {
        QLabel *pLabel = new QLabel;
        pLabel->setMinimumSize(pLabel->sizeHint());
        pLabel->setPixmap(QPixmap(":/png/png/ledyellow.png"));
        pLabel->setToolTip(m_listStrLink.at(i)+ ": " + tr("未定义"));
        m_listStatLink << pLabel;
    }
}

void MainU1::BitListInit()
{
    this->tf->SetLwListBit()->setViewMode(QListWidget::IconMode);
    this->tf->SetLwListBit()->setContentsMargins(50, 5, 5, 5);
    for(int i = 0; i < m_listStrBit.count(); i++)
    {
        QListWidgetItem *pItem = new QListWidgetItem(this->tf->SetLwListBit());
        pItem->setIcon(QIcon(":/png/png/yellow.png"));
        pItem->setToolTip(m_listStrBit.at(i)+": "+tr("未定义"));
        pItem->setSizeHint(QSize(40,40));
    }
    this->tf->SetLwListBit()->setMovement(QListView::Static);
}

void MainU1::StartPageInit()
{
    QRegExp ipRx("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-4]|[01]?\\d\\d?)");
    this->tf->SetTcpAddr()->setValidator(new QRegExpValidator(ipRx, this->tf->SetTcpAddr()));
    this->tf->SetTcport()->setValidator(new QIntValidator(0, 65535,this->tf->SetTcport()));
    if(!m_infoStore.m_szIP.isEmpty()) this->tf->SetTcpAddr()->setText(m_infoStore.m_szIP);
    if(m_infoStore.m_uPort != 0)  this->tf->SetTcport()->setText(QString::number(m_infoStore.m_uPort));
    if(!m_infoStore.m_strCurUser.isEmpty()) this->tf->SetUserName()->setText(m_infoStore.m_strCurUser);
    if(!m_infoStore.m_strCurPw.isEmpty()) this->tf->SetUserPswd()->setText(m_infoStore.m_strCurPw);
}

void MainU1::RecPageInit()
{
    m_listChkRec << this->tf->SetRcdmChkA() << this->tf->SetRcdmChkB();

    for(int i = 0; i < m_listChkRec.count(); i++)
    {
        m_listChkRec.at(i)->setChecked((m_infoStore.m_uRecCon&(1<<i)) != 0);
        m_listChkRec.at(i)->setToolTip(m_ListStrChannel.at(i));
    }
}

void MainU1::ReplayPageInit()
{
    m_listChkReplay << this->tf->SetRpmChkA() << this->tf->SetRpmChkB();
    m_listChkTypeReplay << this->tf->SetRpmType1() << this->tf->SetRpmType2() << this->tf->SetRpmType3() << this->tf->SetRpmType4();
    this->tf->SetRpmChkchnselreplay()->setChecked((m_infoStore.m_filterReplay.uValid&FIELD__CHAN) == FIELD__CHAN);
    rpmchkChnSelReplay();
    for(int i = 0; i < m_listChkReplay.count(); i++)
    {
        m_listChkReplay.at(i)->setChecked((m_infoStore.m_filterReplay.uChannel&(1<<i)) != 0);
        m_listChkReplay.at(i)->setToolTip(m_ListStrChannel.at(i));
    }
    this->tf->SetRpmchktimesetreplay()->setChecked((m_infoStore.m_filterReplay.uValid&FIELD__TIME_BEGIN) == FIELD__TIME_BEGIN);
    rpmchkTimeSelReplay();
    this->tf->SetRpmStartTime()->setDateTime(m_infoStore.m_filterReplay.timeBeg);
    this->tf->SetRpmEndTime()->setDateTime(m_infoStore.m_filterReplay.timeEnd);
    rpmchkTypeSelReplay();
    for(int i = 0; i < m_listChkTypeReplay.count(); i++)
        m_listChkTypeReplay.at(i)->setChecked((m_infoStore.m_filterReplay.uRange&(1<<i)) != 0);
    rpmchkChanSelReplay();
}

void MainU1::slot_handlerpmsignals(int type)
{
    switch (type) {
    case rpmsignal_chkchnselreplay:
        rpmchkChnSelReplay();
        break;
    case rpmsignal_chkChanSelReplay:
        rpmchkChanSelReplay();
        break;
    case rpmsignal_cjkTimeSelReplay:
        rpmchkTimeSelReplay();
        break;
    case rpmsignal_chktypeSelReplay:
        rpmchkTypeSelReplay();
        break;
    case rpmsignal_type1:
        rpmtype1();
        break;
    case rpmsignal_type2:
        rpmtype2();
        break;
    case rpmsignal_type3:
        rpmtype3();
        break;
    case rpmsignal_type4:
        rpmtype4();
        break;
    case rpmsignal_chkReplayA:
        rpmchkReplayA();
        break;
    case rpmsignal_chkReplayB:
        rpmchkReplayB();
        break;
    case rpmsignal_replay:
        rpmreplaystart();
        break;
    case rpmsignal_pause:
        rpmreplaypause();
        break;
    case rpmsignal_stop:
        rpmreplaystop();
        break;
    default:
        break;
    }
}

void MainU1::slot_handlercdmsignal(int type)
{
    switch (type) {
    case rcdmsignal_setfilesz:
        slot_rcdmSetFilesz();
        break;
    case rcdmsignal_chkRec_A:
        slot_rcdmReplayA();
        break;
    case rcdmsignal_chkRec_B:
        slot_rcdmReplayB();
        break;
    case rcdmsignal_recstart:
        slot_rcdmRecordStart();
        break;
    case rcdmsignal_recstop:
        slot_rcdmRecordEnd();
        break;
    default:
        break;
    }
}

void MainU1::slot_handlesmsignals(int type)
{
    switch (type) {
    case smsignal_chkchnselect:
        slot_slcmChkchnSelSelect();
        break;
    case smsignal_chkSelectTypeA:
        slot_slcmSelectA();
        break;
    case smsignal_chkSelectTypeB:
        slot_slcmSelectB();
        break;
    case smsignal_chkTimeSelect:
        slot_slcmChkTimeSelSelect();
        break;
    case smsignal_chkTypeSelect:
        slot_slcmChkTypeSelSelect();
        break;
    case smsginal_type1:
        slot_slcmType1();
        break;
    case smsginal_type2:
        slot_slcmType2();
        break;
    case smsginal_type3:
        slot_slcmType3();
        break;
    case smsginal_type4:
        slot_slcmType4();
        break;
    case smsignal_DeleteFile:
        slot_slcmdeletefile();
        break;
    case smsignal_InqDir:
        slot_slcminqdir();
        break;
    default:
        break;
    }
}

void MainU1::slot_handlemgmsignals(int type)
{
    switch (type) {
    case mgmsignal_sendtime:
        slot_mgmsendtime();
        break;
    case mgmsignal_selfcheck:
        slot_mgmselfcheck();
        break;
    default:
        break;
    }
}

void MainU1::slot_handleumsignals(int type)
{
    switch (type) {
    case umsignal_creatusr:
        slot_umcreatusr();
        break;
    case umsignal_modifypswd:
        slot_ummodifypswd();
        break;
    case umsignal_delusr:
        slot_umdelusr();
        break;
    default:
        break;
    }
}

void MainU1::slot_handlespmsignals(int type)
{
    switch (type) {
    case spmsignal_cleardata:
        slot_spmcleardata();
        break;
    case spmsignal_softreset:
        slot_spmsoftreset();
        break;
    case spmsignal_poweroff:
        slot_spmpoweroff();
        break;
    case spmsignal_update:
        slot_spmupdate();
        break;
    case spmsignal_spftdistory:
        slot_spmsoftdistory();
        break;
    default:
        break;
    }
}

void MainU1::slot_handlecusignals(int type)
{
    switch (type) {
    case cusignal_trwitemselectionchange:
        slot_cutrwitemselectionchange();
        break;
    case cusignal_pbpbbitsmall:
        slot_cupbbitsmall();
        break;
    default:
        break;
    }
}
void MainU1::slot_rcdmSetFilesz()
{
    UINT32 filesz = this->tf->GetRcdmFilesz();
    if(this->tf->GetRcdmFileszUint() == "GB")
        filesz = filesz * 1024;
    emit SetfileszSignal(filesz);
}

void MainU1::slot_rcdmReplayA()
{
    bool bChecked = this->tf->SetRcdmChkA()->isChecked();
    QIcon PBrcdmReplayA;
    if(bChecked)
    {
        PBrcdmReplayA.addFile(":/png/png/checkmark-128.png");
        this->tf->SetRcdmChkA()->setIcon(PBrcdmReplayA);
    } else {
        PBrcdmReplayA.addFile(":/png/png/cross-128.png");
        this->tf->SetRcdmChkA()->setIcon(PBrcdmReplayA);
    }
}

void MainU1::slot_rcdmReplayB()
{
    bool bChecked = this->tf->SetRcdmChkB()->isChecked();
    QIcon PBrcdmReplayB;
    if(bChecked)
    {
        PBrcdmReplayB.addFile(":/png/png/checkmark-128.png");
        this->tf->SetRcdmChkB()->setIcon(PBrcdmReplayB);
    } else {
        PBrcdmReplayB.addFile(":/png/png/cross-128.png");
        this->tf->SetRcdmChkB()->setIcon(PBrcdmReplayB);
    }
}

void MainU1::slot_rcdmRecordStart()
{
    if(m_bNeedRecMode) {}
    UINT32 uRecCon = 0;
    for(int i = 0; i < m_listChkRec.count(); i++)
    {
        if(m_listChkRec.at(i)->isChecked()) uRecCon |= 1 << i;
    }

    this->tf->SetRcdmRecStop()->setEnabled(true);
    this->tf->SetRcdmRecStart()->setEnabled(false);
    emit RecConSignal(uRecCon);
    Mysleep(1000);
    this->tf->SetRcdmRecStart()->setEnabled(true);
}

void MainU1::slot_rcdmRecordEnd()
{
    UINT32 uRecCon = 0;
    for(int i = 0; i < m_listChkRec.count(); i++)
    {
        if(m_listChkRec.at(i)->isChecked()) uRecCon &= ~(1 << i);
    }
    this->tf->SetRcdmRecStop()->setEnabled(false);
    emit RecConSignal(uRecCon);
}


void MainU1::OtherInit()
{
    QRegExp double_rx180("^-?(180(\\.0{1,5})?|(1?[0-7]?\\d|\\d?\\d?)(\\.\\d{1,5})?)$");
    QRegExp double_rx90("^-?(90(\\.0{1,5})?|[1-8]?\\d(\\.\\d{1,5})?)$");
    QRegExp double_rx360("^((([0-2])?\\d{1,2}|3[0-5]\\d)(\\.\\d{1})?|360(\\.0{1})?)$");
}

void MainU1::AllAckSlot(QByteArray head)
{
    m_bAlive = true;
}

void MainU1::BitSlot(QByteArray data)
{
    RW_MSGR__ADM_SELFDIAG *pData = (RW_MSGR__ADM_SELFDIAG *)data.data();
    int idx = 0;
    this->tf->SetCuDevStaCap()->setCurrentIndex(1);
    for(int i = 0; i < BIT_NUM;i++)
    {
        UINT32 *pU = (UINT32*)&pData->chan_stat;
        QListWidgetItem *pItem = this->tf->SetCuListBit()->item(idx);
        QString sz;
        switch ((*pU>>(i*2)) & 0x3) {
        case LINKSTAT__DOWN:
             sz = tr("无连接");
             pItem->setIcon(QIcon(":/png/png/blue.png"));
             break;
         case LINKSTAT__ERROR:
             sz = tr("有故障");
             pItem->setIcon(QIcon(":/png/png/red.png"));
             break;
         case LINKSTAT__UP:
             sz = tr("正常");
             pItem->setIcon(QIcon(":/png/png/green.png"));
             break;
         case LINKSTAT__RSVD:
             sz = tr("未定义");
             pItem->setIcon(QIcon(":/png/png/yellow.png"));
             break;
        }
        pItem->setToolTip(m_listStrBit.at(idx)+": "+sz);
        idx++;
    }
    for(int i=0; i<STOR_NUM; i++)
    {
     QListWidgetItem *pItem = this->tf->SetCuListBit()->item(idx);

     UINT32 *pU = (UINT32*)&pData->stor_stat;
     QString sz;
     switch((*pU>>(i*2)) & 0x3)
     {
     case LINKSTAT__DOWN:
         sz = tr("无连接");
         pItem->setIcon(QIcon(":/png/png/blue.png"));
         break;
     case LINKSTAT__ERROR:
         sz = tr("有故障");
         pItem->setIcon(QIcon(":/png/png/red.png"));
         break;
     case LINKSTAT__UP:
         sz = tr("正常");
         pItem->setIcon(QIcon(":/png/png/green.png"));
         break;
     case LINKSTAT__RSVD:
         sz = tr("未定义");
         pItem->setIcon(QIcon(":/png/png/yellow.png"));
         break;
     }
     pItem->setToolTip(m_listStrBit.at(idx)+": "+sz);
     idx++;
    }
}

void MainU1::AllCmdUiSlot(QByteArray head, QByteArray data)
{
    cmi_msgr_all_cmdresult *pMsgr = (cmi_msgr_all_cmdresult *)data.data();
    QString sz;
    if(pMsgr->flag & CMI_CMDRESULT_FLAG__DONE)
    {
        switch(pMsgr->cmd)
        {
         case CMI_CMD__STOR_OPERFILE:
             sz = QString(tr("文件操作完成，返回值:%1")).arg(pMsgr->result);
             break;
         case CMI_CMD__STOR_OPERMED:
         {
             static int iCount = 0;
             iCount++;
             m_bMounted = (pMsgr->result==0);
             if((!m_bMounted) && (iCount<4))
             {
                 Mysleep(5000);
                 if(m_uMountMode == CMI_MEDOP__MOUNT)
                     emit MountSignal(false);
                 else
                     emit UnmountSignal(false);
             }
             else
             {
                 iCount = 0;
                 sz = QString(tr("磁盘操作完成，返回值:%1")).arg(pMsgr->result);
             }
         }
             break;
         case CMI_CMD__BIZ_CTRLPLAY:
             sz = QString(tr("回放控制完成，返回值:%1")).arg(pMsgr->result);
             break;
         case CMI_CMD__STOR_LISTFILE:
             sz = QString(tr("目录返回完成，返回值:%1")).arg(pMsgr->result);
             break;
         case CMI_CMD__BIZ_CTRLMIG:
             sz = QString(tr("卸载控制完成，返回值:%1")).arg(pMsgr->result);
             break;
         case RW_CMD__ADM_SELFDIAG:
             sz = QString(tr("静态自检完成，返回值:%1")).arg(pMsgr->result);
             break;
         case RW_CMD__ADM_SOFTRESET:
             sz = QString(tr("软复位完成，返回值:%1")).arg(pMsgr->result);
             break;
         case RW_CMD__ADM_SELFDEST:
             sz = QString(tr("软销毁完成，返回值:%1")).arg(pMsgr->result);
             break;
        }
    }
        else
        {
            UpdateProgressSlot(true, pMsgr->progress);
            switch(pMsgr->cmd)
             {
             case CMI_CMD__STOR_OPERFILE:
                 sz = QString(tr("操作文件中(%1%)...")).arg(pMsgr->progress);
                 break;
             case CMI_CMD__BIZ_CTRLPLAY:
                 sz = QString(tr("回放进行中(%1%)...")).arg(pMsgr->progress);
                 break;
             case CMI_CMD__STOR_LISTFILE:
                 sz = QString(tr("目录返回进行中(%1%)...")).arg(pMsgr->progress);
                 break;
             case CMI_CMD__BIZ_CTRLMIG:
                 sz = QString(tr("文件卸载进行中(%1%)...")).arg(pMsgr->progress);
                 break;
             case RW_CMD__ADM_SELFDIAG:
                 sz = QString(tr("静态自检进行中(%1%)...")).arg(pMsgr->progress);
                 break;
             case RW_CMD__ADM_SOFTRESET:
                 sz = QString(tr("软复位进行中(%1%)...")).arg(pMsgr->progress);
                 break;
             case RW_CMD__ADM_SELFDEST:
                 sz = QString(tr("软销毁进行中(%1%)...")).arg(pMsgr->progress);
                 break;
             }
                if(!sz.isEmpty()) emit UImsg(sz);
        }
    }

void MainU1::rpmchkChnSelReplay()
{
    bool bChecked = this->tf->SetRpmChkchnselreplay()->isChecked();
    if(bChecked)
    {
        QIcon PBchkchnselreplayicon;
        PBchkchnselreplayicon.addFile(":/png/png/checkmark-128.png");
        this->tf->SetRpmChkchnselreplay()->setIcon(PBchkchnselreplayicon);
    } else {
        QIcon PBchkchnselreplayicon;
        PBchkchnselreplayicon.addFile(":/png/png/cross-128.png");
        this->tf->SetRpmChkchnselreplay()->setIcon(PBchkchnselreplayicon);
    }
    for(int i=0; i<m_listChkReplay.count(); i++)
    {
        m_listChkReplay.at(i)->setEnabled(bChecked);
    }
}

void MainU1::rpmchkChanSelReplay()
{
    bool bChecked = this->tf->SetRpmchkchansetreplay()->isChecked();
    this->tf->SetRpmOutCom()->setEnabled(bChecked);
}

void MainU1::rpmchkTimeSelReplay()
{
    bool bChecked = this->tf->SetRpmchktimesetreplay()->isChecked();
    this->tf->SetRpmStartTime()->setEnabled(bChecked);
    this->tf->SetRpmEndTime()->setEnabled(bChecked);
}

void MainU1::rpmchkTypeSelReplay()
{
    bool bChecked  = this->tf->SetRpmchktypesetreplay()->isChecked();
    for(INT32 i = 0; i < m_listChkTypeReplay.count(); i++)
    {
        m_listChkTypeReplay.at(i)->setEnabled(bChecked);
    }
}

void MainU1::rpmtype1()
{
    bool bChecked = this->tf->SetRpmType1()->isChecked();
    QIcon PBrpmtype1;
    if(bChecked)
    {
        PBrpmtype1.addFile(":/png/png/checkmark-128.png");
        this->tf->SetRpmType1()->setIcon(PBrpmtype1);
    } else {
        PBrpmtype1.addFile(":/png/png/cross-128.png");
        this->tf->SetRpmType1()->setIcon(PBrpmtype1);
    }
}

void MainU1::rpmtype2()
{
    bool bChecked = this->tf->SetRpmType2()->isChecked();
    QIcon PBrpmtype2;
    if(bChecked)
    {
        PBrpmtype2.addFile(":/png/png/checkmark-128.png");
        this->tf->SetRpmType2()->setIcon(PBrpmtype2);
    } else {
        PBrpmtype2.addFile(":/png/png/cross-128.png");
        this->tf->SetRpmType2()->setIcon(PBrpmtype2);
    }
}

void MainU1::rpmtype3()
{
    bool bChecked = this->tf->SetRpmType3()->isChecked();
    QIcon PBrpmtype3;
    if(bChecked)
    {
        PBrpmtype3.addFile(":/png/png/checkmark-128.png");
        this->tf->SetRpmType3()->setIcon(PBrpmtype3);
    } else {
        PBrpmtype3.addFile(":/png/png/cross-128.png");
        this->tf->SetRpmType3()->setIcon(PBrpmtype3);
    }
}

void MainU1::rpmtype4()
{
    bool bChecked = this->tf->SetRpmType4()->isChecked();
    QIcon PBrpmtype4;
    if(bChecked)
    {
        PBrpmtype4.addFile(":/png/png/checkmark-128.png");
        this->tf->SetRpmType4()->setIcon(PBrpmtype4);
    } else {

        PBrpmtype4.addFile(":/png/png/cross-128.png");
        this->tf->SetRpmType4()->setIcon(PBrpmtype4);
    }
}

void MainU1::rpmchkReplayA()
{
    bool bChecked = this->tf->SetRpmChkA()->isChecked();
    QIcon PBrpmchkReplayA;
    if(bChecked)
    {
        PBrpmchkReplayA.addFile(":/png/png/checkmark-128.png");
        this->tf->SetRpmChkA()->setIcon(PBrpmchkReplayA);
    } else {

        PBrpmchkReplayA.addFile(":/png/png/cross-128.png");
        this->tf->SetRpmChkA()->setIcon(PBrpmchkReplayA);
    }
}

void MainU1::rpmchkReplayB()
{
    bool bChecked = this->tf->SetRpmChkB()->isChecked();
    QIcon PBrpmchkReplayB;
    if(bChecked)
    {
        PBrpmchkReplayB.addFile(":/png/png/checkmark-128.png");
        this->tf->SetRpmChkB()->setIcon(PBrpmchkReplayB);
    } else {
        PBrpmchkReplayB.addFile(":/png/png/cross-128.png");
        this->tf->SetRpmChkB()->setIcon(PBrpmchkReplayB);
    }
}

QVariant MainU1::ReadLimitReplay()
{
    T_FILTER filter;
    filter.uFileFlags = 0;
    filter.uValid = 0;
    if(this->tf->SetRpmchkchansetreplay()->isChecked()) filter.uValid |= FIELD__CHAN;
    filter.uChannel = 0;
    for(int i=0; i<2; i++)
    {
        if(m_listChkReplay.at(i)->isChecked())
            filter.uChannel |= 1<<(i);
    }
    if(this->tf->SetRpmchkchansetreplay()->isChecked())
    {
        filter.uFileFlags = filter.uChannel;
        filter.uChannel = 0;
        filter.uChannel |= 1<<(this->tf->SetRpmOutCom()->currentIndex());
        emit UImsg(tr("current index: ") +QString::number(filter.uFileFlags, 10));
    }
    if(this->tf->SetRpmchktimesetreplay()->isChecked()) filter.uValid |= FIELD__TIME_BEGIN | FIELD__TIME_END;
    filter.timeBeg = this->tf->SetRpmStartTime()->dateTime();
    filter.timeEnd = this->tf->SetRpmEndTime()->dateTime();

    if(this->tf->SetRpmchktypesetreplay()->isChecked()) filter.uValid |= FIELD__DATA_TYPE;
    filter.uRange = 0;
    for(int i=0; i<m_listChkTypeReplay.count(); i++)
    {
        if(m_listChkTypeReplay.at(i)->isChecked())
            filter.uRange |= 1<<i;
    }
    QVariant a;
    a.setValue(filter);
    return a;
}

void MainU1::rpmreplaystart()
{
    QVariant pb = ReadLimitReplay();
    T_FILTER filterPb;
    filterPb = pb.value<T_FILTER>();
    UINT32 uTypeNum = 0;
    for(int i=0; i<32; i++)
    {
        if(((filterPb.uRange>>i)&1)==1) uTypeNum++;
    }
    CString sz;
    if((uTypeNum!=1) || ((filterPb.uValid&FIELD__DATA_TYPE)!=FIELD__DATA_TYPE))
    {
        emit UImsg(tr("错误：回放时必须打开类型约束，且只可选择一种数据类型。"));
    }
    else
    {
        this->tf->SetRpmPause()->setChecked(false);
        this->tf->SetRpmPause()->setChecked(true);
        this->tf->SetRpmStop()->setChecked(true);
        emit PlaybackPauseSignal(0);
        emit PlaybackStartSignal(pb);
    }
}

void MainU1::rpmreplaypause()
{
    UINT32 uChannel = 0;
    if(this->tf->SetRpmPause()->isChecked())
    {
        for(int i;i < m_listChkReplay.count(); i++)
        {
            if(m_listChkReplay.at(i)->isChecked())
                uChannel |= 1<<i;
        }
        this->tf->SetRpmPause()->setChecked(true);
        this->tf->SetRpmPause()->setText(tr("回放继续"));
    } else {
        for(int i=0; i<m_listChkReplay.count(); i++)
        {
            if(m_listChkReplay.at(i)->isChecked())
                uChannel &= ~(1<<i);
        }
        this->tf->SetRpmPause()->setChecked(false);
        this->tf->SetRpmPause()->setText(tr("回放暂停"));
    }
    this->tf->SetRpmPause()->setEnabled(true);
    this->tf->SetRpmStop()->setEnabled(true);
    emit PlaybackPauseSignal(uChannel);
}

void MainU1::rpmreplaystop()
{
    this->tf->SetRpmPause()->setChecked(false);
    this->tf->SetRpmStop()->setChecked(false);
    this->tf->SetRpmStop()->setText(tr("回放暂停"));
    emit PlaybackPauseSignal(0);
    emit PlaybackStopSignal();
}

void MainU1::SelectPageInit()
{
    m_listChkSelect << this->tf->SetSlcmChkA() << this->tf->SetSlcmChkB();
    m_listChkTypeSelect << this->tf->SetSlcmType1() << this->tf->SetSlcmType2() << this->tf->SetSlcmType3()\
                        << this->tf->SetSlcmType4();
    this->tf->SetSlcmChnSelect()->setChecked((m_infoStore.m_filterSelect.uValid&FIELD__CHAN) == FIELD__CHAN);
    slot_slcmChkchnSelSelect();
    for(int i = 0; i < m_listChkSelect.count(); i++)
    {
        m_listChkSelect.at(i)->setChecked((m_infoStore.m_filterSelect.uChannel&(1<<i)) != 0);
        m_listChkSelect.at(i)->setToolTip(m_ListStrChannel.at(i));
    }
    this->tf->SetSlcmTimeSelect()->setChecked((m_infoStore.m_filterSelect.uValid&FIELD__TIME_BEGIN) == FIELD__TIME_BEGIN);
    slot_slcmChkTimeSelSelect();
    this->tf->SetSlcmStartTime()->setDateTime(m_infoStore.m_filterSelect.timeBeg);
    this->tf->SetSlcmEndTime()->setDateTime(m_infoStore.m_filterSelect.timeEnd);
    slot_slcmChkTypeSelSelect();
    for(int i = 0; i< m_listChkTypeSelect.count(); i++)
        m_listChkTypeSelect.at(i)->setChecked((m_infoStore.m_filterSelect.uRange&(1<<i)) != 0);
}

void MainU1::slot_slcmChkchnSelSelect()
{
    bool bChecked = this->tf->SetSlcmChnSelect()->isChecked();
    QIcon PBSlcmChnSelect;
    if(bChecked)
    {
        PBSlcmChnSelect.addFile(":/png/png/checkmark-128.png");
        this->tf->SetSlcmChnSelect()->setIcon(PBSlcmChnSelect);
    } else {

        PBSlcmChnSelect.addFile(":/png/png/cross-128.png");
        this->tf->SetSlcmChnSelect()->setIcon(PBSlcmChnSelect);
    }
    for(int i = 0 ; i < m_listChkSelect.count(); i++)
        m_listChkSelect.at(i)->setEnabled(bChecked);
}

void MainU1:: slot_slcmChkTimeSelSelect()
{
    bool bChecked = this->tf->SetSlcmTimeSelect()->isChecked();
    this->tf->SetSlcmStartTime()->setEnabled(bChecked);
    this->tf->SetSlcmEndTime()->setEnabled(bChecked);
}

void MainU1::slot_slcmChkTypeSelSelect()
{
    bool bChecked = this->tf->SetSlcmTypeSelect()->isChecked();
    QIcon PBSlcmTypeSelect;
    if(bChecked)
    {
        PBSlcmTypeSelect.addFile(":/png/png/checkmark-128.png");
        this->tf->SetSlcmTypeSelect()->setIcon(PBSlcmTypeSelect);
    } else {

        PBSlcmTypeSelect.addFile(":/png/png/cross-128.png");
        this->tf->SetSlcmTypeSelect()->setIcon(PBSlcmTypeSelect);
    }
    for(INT32 i = 0; i < m_listChkTypeSelect.count(); i++)
        m_listChkTypeSelect.at(i)->setEnabled(bChecked);
}

void MainU1::slot_slcmSelectA()
{
    bool bChecked = this->tf->SetSlcmChkA()->isChecked();
    QIcon PBslcmSelectA;
    if(bChecked)
    {
        PBslcmSelectA.addFile(":/png/png/checkmark-128.png");
        this->tf->SetSlcmChkA()->setIcon(PBslcmSelectA);
    } else {
        PBslcmSelectA.addFile(":/png/png/cross-128.png");
        this->tf->SetSlcmChkA()->setIcon(PBslcmSelectA);
    }
}


void MainU1::slot_slcmSelectB()
{
    bool bChecked = this->tf->SetSlcmChkB()->isChecked();
    QIcon PBslcmSelectB;
    if(bChecked)
    {
        PBslcmSelectB.addFile(":/png/png/checkmark-128.png");
        this->tf->SetSlcmChkB()->setIcon(PBslcmSelectB);
    } else {
        PBslcmSelectB.addFile(":/png/png/cross-128.png");
        this->tf->SetSlcmChkB()->setIcon(PBslcmSelectB);
    }
}

void MainU1::slot_slcmType1()
{
    bool bChecked = this->tf->SetSlcmType1()->isChecked();
    QIcon PBslcmtype1;
    if(bChecked)
    {
        PBslcmtype1.addFile(":/png/png/checkmark-128.png");
        this->tf->SetSlcmType1()->setIcon(PBslcmtype1);
    } else {
        PBslcmtype1.addFile(":/png/png/cross-128.png");
        this->tf->SetSlcmType1()->setIcon(PBslcmtype1);
    }
}
void MainU1::slot_slcmType2()
{
    bool bChecked = this->tf->SetSlcmType2()->isChecked();
    QIcon PBslcmtype2;
    if(bChecked)
    {
        PBslcmtype2.addFile(":/png/png/checkmark-128.png");
        this->tf->SetSlcmType2()->setIcon(PBslcmtype2);
    } else {
        PBslcmtype2.addFile(":/png/png/cross-128.png");
        this->tf->SetSlcmType2()->setIcon(PBslcmtype2);
    }
}
void MainU1::slot_slcmType3()
{
    bool bChecked = this->tf->SetSlcmType3()->isChecked();
    QIcon PBslcmtype3;
    if(bChecked)
    {
        PBslcmtype3.addFile(":/png/png/checkmark-128.png");
        this->tf->SetSlcmType3()->setIcon(PBslcmtype3);
    } else {
        PBslcmtype3.addFile(":/png/png/cross-128.png");
        this->tf->SetSlcmType3()->setIcon(PBslcmtype3);
    }
}
void MainU1::slot_slcmType4()
{
    bool bChecked = this->tf->SetSlcmType4()->isChecked();
    QIcon PBslcmtype4;
    if(bChecked)
    {
        PBslcmtype4.addFile(":/png/png/checkmark-128.png");
        this->tf->SetSlcmType4()->setIcon(PBslcmtype4);
    } else {
        PBslcmtype4.addFile(":/png/png/cross-128.png");
        this->tf->SetSlcmType4()->setIcon(PBslcmtype4);
    }
}

QVariant MainU1::ReadLimitSelect()
{
    T_FILTER filter;
    filter.uFileFlags = 0;
    filter.uValid = 0;
    if(this->tf->SetSlcmChnSelect()->isChecked()) filter.uValid |= FIELD__CHAN;
    filter.uChannel = 0;
    for(int i=0; i<2; i++)
    {
        if(m_listChkSelect.at(i)->isChecked())
            filter.uChannel |= 1<<(i);
    }

    if(this->tf->SetSlcmChnSelect()->isChecked()) filter.uValid |= FIELD__TIME_BEGIN | FIELD__TIME_END;
    filter.timeBeg = this->tf->SetSlcmStartTime()->dateTime();
    filter.timeEnd = this->tf->SetSlcmEndTime()->dateTime();

    if(this->tf->SetSlcmTypeSelect()->isChecked()) filter.uValid |= FIELD__DATA_TYPE;
    filter.uRange = 0;
    for(int i=0; i<m_listChkTypeSelect.count(); i++)
    {
        if(m_listChkTypeSelect.at(i)->isChecked())
            filter.uRange |= 1<<i;
    }

    QVariant a;
    a.setValue(filter);
    return a;
}

void MainU1::slot_slcmdeletefile()
{
    this->tf->SetSlcmDelFile()->setEnabled(false);
    emit FileDelSignal(ReadLimitSelect());
    Mysleep(1000);
    this->tf->SetSlcmDelFile()->setEnabled(m_connectstatus && m_bLogined);
}

void MainU1::slot_slcminqdir()
{
    this->tf->SetCuTableFile()->clear();
    this->tf->SetCuTableFile()->setRowCount(0);
    this->tf->SetCuTreeFile()->clear();
    m_listFile.clear();

    this->tf->SetSlcmInqDir()->setEnabled(true);
    emit DirSignal(ReadLimitSelect());
    Mysleep(3000);
    m_mutexFile.lock();
    m_mutexFile.unlock();
    this->tf->SetSlcmInqDir()->setEnabled(m_connectstatus && m_bLogined);
}

void MainU1::TimerSyncTimeFunction()
{
    if(this->tf->SetMgmTimeSync()->isChecked())
        this->tf->SetMgmTimeEdit()->setDateTime(QDateTime::currentDateTime());
}

void MainU1::slot_mgmtimesync()
{

}

void MainU1::slot_mgmsendtime()
{
    this->tf->SetMgmSendTime()->setEnabled(false);
    emit SetTimeSignal( this->tf->SetMgmTimeEdit()->dateTime());
    Mysleep(500);
    this->tf->SetMgmSendTime()->setEnabled(true);
}

void MainU1::slot_mgmselfcheck()
{
    this->tf->SetMgmSelfCheck()->setEnabled(false);
    this->tf->SetCuBitsmall()->setEnabled(false);
    for(int i = 0; i < this->tf->SetCuListBit()->count(); i++)
    {
        this->tf->SetCuListBit()->item(i)->setIcon(QIcon(":/png/png/yellow.png"));
    }
    emit MBitSignal();

    Mysleep(500);
    this->tf->SetMgmSelfCheck()->setEnabled(true);
    this->tf->SetCuBitsmall()->setEnabled(true);
}

void MainU1::slot_umcreatusr()
{
    this->tf->SetUmCreatUsr()->setEnabled(false);
    emit UserNewSignal(this->tf->GetUmUsrname(), this->tf->GetUmUsrpswd());
    Mysleep(500);
    this->tf->SetUmCreatUsr()->setEnabled(true);
}

void MainU1::slot_ummodifypswd()
{
    this->tf->SetUmModifyPswd()->setEnabled(false);
    emit UserChgPwSignal(this->tf->GetUmUsrname(), this->tf->GetUmUsrpswd());
    Mysleep(500);
    this->tf->SetUmModifyPswd()->setEnabled(true);
}

void MainU1::slot_umdelusr()
{
    this->tf->SetUmDelusr()->setEnabled(false);
    emit UserDelSignal(this->tf->GetUmUsrname(), this->tf->GetUmUsrpswd());
    Mysleep(500);
    this->tf->SetUmDelusr()->setEnabled(true);
}

void MainU1::slot_spmcleardata()
{
    this->tf->SetSpmClearData()->setEnabled(false);
    emit WipeSignal();
    Mysleep(5000);
    this->tf->SetSpmClearData()->setEnabled(true);
}

void MainU1::slot_spmpoweroff()
{
    this->tf->SetSpmPoweroff()->setEnabled(false);
    emit PowerDownSignal();
    m_bLogined = false;
    uiChgToolEnable(false);
    Mysleep(5000);
    m_bManual = false;
    m_tcpclient->disconnectFromHost();
    emit UImsg(tr("网络已断开"));
}

void MainU1::slot_spmsoftdistory()
{
    this->tf->SetSpmSoftdistory()->setEnabled(false);
    emit SelfDestorySignal();
    m_bLogined = false;
    //uiChgToolEnable(false);
    Mysleep(5000);
}

void MainU1::slot_spmsoftreset()
{
    this->tf->SetSpmSoftReset()->setEnabled(false);
    emit ResetSignal();
    //uiChgToolEnable(false);
    m_bLogined = false;
    Mysleep(5000);
    m_bManual = false;
    m_tcpclient->disconnectFromHost();
    emit UImsg(tr("网络已断开"));
}

void MainU1::slot_spmupdate()
{
    this->tf->SetSpmUpdate()->setEnabled(false);
    emit UpdateSignal();
    Mysleep(5000);
    this->tf->SetSpmUpdate()->setEnabled(true);
}

void MainU1::slot_cutrwitemselectionchange()
{
    QList<QTreeWidgetItem *> listItemSelected =this->tf->SetCuTreeFile()->selectedItems();
    if(listItemSelected.count() == 0) return;
    CFileAttrib fSrc;
    fSrc.ConvertFromFilename(listItemSelected.at(0)->data(0, Qt::UserRole).toString());
    INT32 nTier = listItemSelected.at(0)->data(1, Qt::UserRole).toInt();
    this->tf->SetCuTableFile()->clearContents();
    this->tf->SetCuTableFile()->setRowCount(65536);
    this->tf->SetCuTableFile()->setDragEnabled(false);
    m_mutexFile.lock();
    UINT32 idx = 0;
    QString strTooltip;
    for (int i = 0; i < (int)m_listFile.count(); i++)
    {
        QString strFlag = "";
        CFileAttrib fCompare = m_listFile.at(i);
        switch (nTier)
        {
        case 2:
            if (!fSrc.IsSameDtype(&fCompare))
                break;
        case 1:
            if (!fSrc.IsSameChannel(&fCompare))
                break;
        case 0:
            // date
            if (!fSrc.IsSameDate(&fCompare))
                break;
        default:
            this->tf->SetCuTableFile()->setItem(idx, 0, new QTableWidgetItem(fCompare.GetFullFilename()));
            this->tf->SetCuTableFile()->setItem(idx, 1, new QTableWidgetItem(fCompare.GetCreateDateTime()));
            this->tf->SetCuTableFile()->setItem(idx, 2, new QTableWidgetItem(fCompare.GetDataType()));
            this->tf->SetCuTableFile()->setItem(idx, 3, new QTableWidgetItem(fCompare.GetFileLength()));
            this->tf->SetCuTableFile()->item(idx, 0)->setIcon(QIcon(":/ico/icon/file.ico"));
            this->tf->SetCuTableFile()->item(idx, 0)->setToolTip(QString("文件名： %1\n创建时间： %2\n文件大小: %3").arg(fCompare.GetFullFilename())
                                                                 .arg(fCompare.GetCreateDateTime()).arg(fCompare.GetFileLength()));
            this->tf->SetCuTableFile()->item(idx, 3)->setTextAlignment(Qt::AlignVCenter|Qt::AlignRight);
            if (fCompare.bProtected)	strFlag += "W";
            if (fCompare.bCompressed)	strFlag += "C";
            if (fCompare.bEncryped)	strFlag += "E";
            if (fCompare.bMarked)	strFlag += "M";
            this->tf->SetCuTableFile()->setItem(idx, 4, new QTableWidgetItem(strFlag,10));
            idx++;
            break;
        }

    }
    this->tf->SetCuTableFile()->setRowCount(idx);
    m_mutexFile.unlock();

}

void MainU1::slot_cupbbitsmall()
{
    slot_mgmselfcheck();
}

void MainU1::ChartUpdate(int idx, double dblUsed, double dblUsable)
{
    if(idx<0 || idx>=noOfChartViewers) return;

    // Create a PieChart object of size 320 x 300 pixels
    int w = 320;
    int h = 300;
    PieChart *c = new PieChart(w, h);

    // Set the center of the pie at (150, 140) and the radius to 100 pixels
    c->setPieSize(150, 140, 100);

    // Add a title to the pie chart
    if(idx==0)
        c->addTitle(QTextCodec::codecForName("UTF-8")->fromUnicode("设备容量").constData(), "simsun.ttc", 12)->setMargin(0, 0, 4, 0);
    else
        c->addTitle(QTextCodec::codecForName("UTF-8")->fromUnicode("磁盘箱容量").constData(), "simsun.ttc", 12)->setMargin(0, 0, 4, 0);

    c->setLabelStyle( "simsun.ttc",12,0x20000000);
    //c->setDefaultFonts("simsun.ttc");

    // Draw the pie in 3D
    c->set3D();

    //dblUsed和dblUsable的单位都是MB
    //当没有磁盘箱时，传过来的数可能为0，可能有异常，做一点保护，当异常时不做图
    if((dblUsable+dblUsed)>100.0)
    {
        // The data for the pie chart
        //double data[] = {(pData->capboard+pData->capextboard-pData->capusable)/1024.0*CAP_UNIT, pData->capusable/1024.0*CAP_UNIT};
        double data[] = {dblUsed, dblUsable};
        // The labels for the pie chart
        const char *labels[] = {"已用","空闲"};
        // Set the pie data and the pie labels
        c->setData(DoubleArray(data, (int)(sizeof(data) / sizeof(data[0]))), StringArray(labels, (int)(
            sizeof(labels) / sizeof(labels[0]))));
    }
    // Output the chart
    c->makeChart();

    // Output the chart
    m_chartViewers[idx]->setChart(c);

    // Include tool tip for the chart
    m_chartViewers[idx]->setImageMap(
        c->getHTMLImageMap("", "", "title='{label}: {value}MB ({percent}%)'"));

    // In this sample project, we do not need to chart object any more, so we
    // delete it now.

    m_chartViewers[idx]->show();
    delete c;
}

void MainU1::slot_inqstatus(QByteArray data)
{
    RW_MSGR__ADM_INQSTATUS *pData = (RW_MSGR__ADM_INQSTATUS *)data.data();

    int idx = 0;
    if((pData->capboard+pData->capextboard >= pData->capusable)
                && ((pData->capusable>1)   //卸载时有可能会卸载磁盘箱而使得可用容量为0，但是磁盘箱容量有数，此时不更新磁盘箱状态
                || (pData->capboard+pData->capextboard+pData->capusable<100)))    //磁盘箱卸载后可用容量和总容量均为0，此时不作图（在chartupdate中会根据此判断）
    {
        this->tf->SetTlwDevSta()->setItem(idx, 0, new QTableWidgetItem(tr("设备总容量")));
        this->tf->SetTlwDevSta()->setItem(idx++,1,new QTableWidgetItem(QString::number(pData->capboard*1.024*1.024/1000.0*CAP_UNIT, 'f', 2)+tr(" MB")));
        this->tf->SetTlwDevSta()->setItem(idx,0,new QTableWidgetItem(tr("设备可用容量")));
        this->tf->SetTlwDevSta()->setItem(idx++,1,new QTableWidgetItem(QString::number(pData->capusable*1.024/1000.0*CAP_UNIT, 'f', 2)+tr(" MB")));
        ChartUpdate(0, (pData->capboard+pData->capextboard-pData->capusable)*1.3/1000.0*CAP_UNIT, pData->capusable*1.3/1000.0*CAP_UNIT);
    } else {
        this->tf->SetTlwDevSta()->setItem(idx,0,new QTableWidgetItem(tr("设备总容量")));
        this->tf->SetTlwDevSta()->setItem(idx++,1,new QTableWidgetItem(QString::number(pData->capboard*1.024*1.024/1000.0*CAP_UNIT, 'f', 2)+tr(" MB")));
        this->tf->SetTlwDevSta()->setItem(idx,0,new QTableWidgetItem(tr("设备可用容量")));
        this->tf->SetTlwDevSta()->setItem(idx++,1,new QTableWidgetItem(QString::number(pData->capusable*1.024/1000.0*CAP_UNIT, 'f', 2)+tr(" MB")));
    }

     QLabel *pLabel;
     for(int i=0; i<2; i++)
     {
         this->tf->SetTlwDevSta()->setItem(idx, 0, new QTableWidgetItem(m_listStrLink.at(i) + tr("链路状态")));
         pLabel = m_listStatLink.at(i);
         UINT32 *pU = (UINT32*)&pData->chan_stat;
         QString sz;
         int k = i;
         if (i>2) {
             k = i-3;
         }
         switch((*pU>>(k*2)) & 0x3)
         {
            case LINKSTAT__DOWN:
              sz = tr("无连接");
              pLabel->setPixmap(QPixmap(":/png/png/ledblue.png"));
              break;
            case LINKSTAT__ERROR:
              sz = tr("有故障");
              pLabel->setPixmap(QPixmap(":/png/png/ledred.png"));
              break;
            case LINKSTAT__UP:
              sz = tr("正常");
              pLabel->setPixmap(QPixmap(":/png/png/ledgreen.png"));
              break;
            case LINKSTAT__RSVD:
              sz = tr("未定义");
              pLabel->setPixmap(QPixmap(":/png/png/ledyellow.png"));
              break;
         }

        pLabel->setToolTip(m_listStrLink.at(i)+": "+sz);
        this->tf->SetTlwDevSta()->setItem(idx++, 1, new QTableWidgetItem(sz));
     }
     this->tf->SetTlwDevSta()->setItem(idx,0,new QTableWidgetItem(tr("总记录速度")));
     pData->fc_speed = pData->fc_speed * 1.024 * 1.024;
     this->tf->SetTlwDevSta()->setItem(idx++, 1, new QTableWidgetItem(
                QString::number(pData->fc_speed, 10)+get_speed_unit_str(pData->fc_speed_unit)+tr("/s")));
     this->tf->SetTlwDevSta()->setItem(idx,0,new QTableWidgetItem(tr("设备工作状态")));
     QString szWork;
     switch(pData->dev_runstat)
     {
     case RUNSTAT__INIT:
         szWork = tr("初始化");
         break;
     case RUNSTAT__READY:
         szWork = tr("准备");
         break;
     case RUNSTAT__RECORD:
         szWork = tr("记录");
         break;
     case RUNSTAT__DOWNLOAD:
         szWork = tr("卸载");
         break;
     case RUNSTAT__PLAYBACK:
         szWork = tr("回放");
         break;
     }

     this->tf->SetTlwDevSta()->setItem(idx++,1,new QTableWidgetItem(szWork));

     uint64_t val = pData->caphd;
     uint32_t temp = ((val >> 32) & 0xffffffff)/10;
     float vol_int = (val & 0xffffffff)/100.0;

     this->tf->SetTlwDevSta()->setItem(idx,0,new QTableWidgetItem(tr("设备温度")));
     this->tf->SetTlwDevSta()->setItem(idx++,1,new QTableWidgetItem(QString::number(temp, 10).toUpper()+tr(" 度")));
     this->tf->SetTlwDevSta()->setItem(idx,0,new QTableWidgetItem(tr("设备电压INT"),10));
     this->tf->SetTlwDevSta()->setItem(idx++,1,new QTableWidgetItem(QString("%1").arg(vol_int) + tr(" V")));
     val = pData->caphdusable;
     float vol_ddr= ((val >> 32) & 0xffffffff)/100.0;
     float vol_aux = (val & 0xffffffff)/100.0;
     this->tf->SetTlwDevSta()->setItem(idx,0,new QTableWidgetItem(tr("设备电压DDR"),10));
     this->tf->SetTlwDevSta()->setItem(idx++,1,new QTableWidgetItem(QString("%1").arg(vol_ddr)+ tr(" V")));
     this->tf->SetTlwDevSta()->setItem(idx,0,new QTableWidgetItem(tr("设备电压AUX"),10));
     this->tf->SetTlwDevSta()->setItem(idx++,1,new QTableWidgetItem(QString("%1").arg(vol_aux) + tr(" V")));

}

void MainU1::WipeSlot(QByteArray data)
{
    RW_MSGR__MED_WIPESTOR *pMsgr = (RW_MSGR__MED_WIPESTOR*)data.data();

     QString sz;
     if(pMsgr->done_flag != WIPE_DONE)
         sz = QString("文件清除中(%1%)...").arg(pMsgr->progress);
     else {
         sz = tr("文件清除已完成。");
     }
     emit UImsg(sz);

     if(pMsgr->done_flag == WIPE_DONE)
     {
         UpdateProgressSlot(false);
     } else {
         UpdateProgressSlot(true, pMsgr->progress);
     }
}

void MainU1::WorkStatusSlot(QByteArray data)
{
     cmi_msgr_heartbeat_t* hb = (cmi_msgr_heartbeat_t*)data.data();
     BIZ_TYPE biztype = hb->sys_biztype;
     static bool bFormatting = false;
     QString sz;
     if (biztype & BIZTYPE__REC)
     {
         sz += tr("记录");
         this->tf->SetRpmStop()->setEnabled(true);
         uiChgBizEnable(false);
     } else {
         this->tf->SetRpmStop()->setEnabled(false);
     }
     if (biztype & BIZTYPE__PLAY)
     {
         sz += tr("回放");
         this->tf->SetRpmPause()->setEnabled(true);
         this->tf->SetRpmStop()->setEnabled(true);
         uiChgBizEnable(false);
     } else {
         this->tf->SetRpmPause()->setEnabled(false);
         this->tf->SetRpmStop()->setEnabled(false);
     }
     if(biztype & BIZTYPE__FORMAT)
     {
        sz += tr("格式化 ");
        uiChgToolEnable(false);
        this->tf->SetSmDisconnect()->setEnabled(false);
        bFormatting = true;
     }
     if(sz.isEmpty())
     {
         sz = tr("空闲");
         FunInIdleEnable(true);
         uiChgBizEnable(true);
         if(bFormatting)
         {
            bFormatting = false;
            uiChgToolEnable(true);
         }
     }
     QString strWorkStatus = tr("工作状态: ") + sz;
     m_labelStatUser->setText(strWorkStatus);
}

void MainU1::uiChgToolEnable(bool bEnable)
{
#if 0
    this->tf->SetSmConnect()->setEnabled(!m_connectstatus);
    this->tf->SetSmDisconnect()->setEnabled(m_connectstatus);
    this->tf->SetSmLogin()->setEnabled(m_connectstatus && !m_bLogined && bEnable);
    this->tf->SetSmLogout()->setEnabled(m_connectstatus && m_bLogined && bEnable);
    this->tf->SetUmCreatUsr()->setEnabled(m_connectstatus && m_bLogined && bEnable);
    this->tf->SetUmDelusr()->setEnabled(m_connectstatus && m_bLogined && bEnable);
    this->tf->SetUmModifyPswd()->setEnabled(m_connectstatus && m_bLogined && bEnable);
    this->tf->SetSpmClearData()->setEnabled(m_connectstatus && m_bLogined && bEnable);
    this->tf->SetSpmSoftdistory()->setEnabled(m_connectstatus && m_bLogined && bEnable);
    this->tf->SetSpmPoweroff()->setEnabled(m_connectstatus && m_bLogined && bEnable);
    this->tf->SetSpmSoftReset()->setEnabled(m_connectstatus && m_bLogined && bEnable);
    this->tf->SetSpmUpdate()->setEnabled(m_connectstatus && m_bLogined && bEnable);
    this->tf->SetRcdmRecStart()->setEnabled(m_connectstatus && m_bLogined && bEnable);
    this->tf->SetRcdmRecStop()->setEnabled(false);
    this->tf->SetRcdmFilesz()->setEnabled(m_connectstatus && m_bLogined && bEnable);
    this->tf->SetRpmReplay()->setEnabled(m_connectstatus && m_bLogined && bEnable);
    this->tf->SetRpmPause()->setEnabled(false);
    this->tf->SetRpmStop()->setEnabled(false);
    this->tf->SetSlcmInqDir()->setEnabled(m_connectstatus && m_bLogined && bEnable);
    this->tf->SetSlcmDelFile()->setEnabled(m_connectstatus && m_bLogined && bEnable);
    this->tf->SetMgmSelfCheck()->setEnabled(m_connectstatus && m_bLogined && bEnable);
    this->tf->SetMgmSendTime()->setEnabled(m_connectstatus && m_bLogined && bEnable);
    this->tf->SetCuBitsmall()->setEnabled(m_connectstatus && m_bLogined && bEnable);
#endif
}

void MainU1::uiChgBizEnable(bool bEnable)
{
    this->tf->SetSlcmDelFile()->setEnabled(bEnable && m_bLogined);
    this->tf->SetSlcmInqDir()->setEnabled(bEnable && m_bLogined);
    this->tf->SetSpmClearData()->setEnabled(bEnable && m_bLogined);
    this->tf->SetSmDisconnect()->setEnabled(bEnable);
    this->tf->SetSmLogout()->setEnabled(bEnable && m_bLogined);
}

void MainU1::FunInIdleEnable(bool bEnable)
{
    this->tf->SetRcdmRecStart()->setEnabled(m_connectstatus && m_bLogined && bEnable);
    this->tf->SetRcdmRecStop()->setEnabled(false);
    this->tf->SetRpmReplay()->setEnabled(m_connectstatus && m_bLogined && bEnable);
    this->tf->SetRpmPause()->setEnabled(false);
    this->tf->SetRpmStop()->setEnabled(false);
}

void MainU1::DirSlot(QList<CFileAttrib> list)
{
    this->tf->SetCuTableFile()->clearContents();
    this->tf->SetCuTreeFile()->clear();

    if(list.isEmpty()) return;
    m_mutexFile.lock();
    m_listFile = list;
    for (int i = 0; i < (int)m_listFile.count(); i++)
    {
        QTreeWidgetItem *pParent = NULL;
        CFileAttrib fSrc = m_listFile.at(i);
        UINT32 nTier = 0;
        BOOL bNeedAdd = true;
        for(int j = 0; j < this->tf->SetCuTreeFile()->topLevelItemCount(); j++)
        {
            CFileAttrib fDest;
            if(!fDest.ConvertFromFilename(this->tf->SetCuTreeFile()->topLevelItem(j)->data(0, Qt::UserRole).toString()))
            {
                continue;
            }
            if(fSrc.IsSameDate(&fDest))
            {
                nTier = 1;
                pParent = this->tf->SetCuTreeFile()->topLevelItem(j);
                for(int m = 0; m < this->tf->SetCuTreeFile()->topLevelItem(j)->childCount();m++)
                {
                    if(!fDest.ConvertFromFilename(this->tf->SetCuTreeFile()->topLevelItem(j)->child(m)->data(0, Qt::UserRole).toString()))
                    {
                        continue;
                    }
                    if(fSrc.IsSameChannel(&fDest))
                    {
                        nTier = 2;
                        pParent = this->tf->SetCuTreeFile()->topLevelItem(j)->child(m);
                        for(int n=0; this->tf->SetCuTreeFile()->topLevelItem(j)->child(m)->childCount(); n++)
                        {
                            if(!fDest.ConvertFromFilename(this->tf->SetCuTreeFile()->topLevelItem(j)->child(m)->child(n)->data(0, Qt::UserRole).toString()))
                            {
                                continue;
                            }
                            if(fSrc.IsSameDtype(&fDest))
                            {
                                bNeedAdd = false;
                                break;
                            }
                        }
                        break;
                    }
                }
                break;
            }
        }
        if(bNeedAdd)
        {
            InsertTreeItem(&fSrc, nTier, pParent);
        }
    }
    m_mutexFile.unlock();
}

void MainU1::InsertTreeItem(CFileAttrib *pfileAttribSrc, INT32 nTier, QTreeWidgetItem *pParent)
{
    QTreeWidgetItem *A, *B, *C;
    switch(nTier)
    {
    case 0:
        A = new QTreeWidgetItem(QStringList()<<pfileAttribSrc->GetCreateDate());
        A->setData (0, Qt::UserRole, pfileAttribSrc->GetFullFilename());
        A->setData (1, Qt::UserRole, 0);
        this->tf->SetCuTreeFile()->addTopLevelItem(A);
        Q_ASSERT(pParent==NULL);
        pParent = A;
    case 1:
        B = new QTreeWidgetItem(QStringList()<<pfileAttribSrc->GetChannelName());
        B->setData (0, Qt::UserRole, pfileAttribSrc->GetFullFilename());
        B->setData (1, Qt::UserRole, 1);
        pParent->addChild(B);
        pParent = B;
    case 2:
        C = new QTreeWidgetItem(QStringList()<<pfileAttribSrc->GetDataType());
        C->setData (0, Qt::UserRole, pfileAttribSrc->GetFullFilename());
        C->setData (1, Qt::UserRole, 2);
        pParent->addChild(C);
        break;
    default:
        Q_ASSERT(0);
        break;
    }
}
