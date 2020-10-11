#if 0
#include <QTableWidgetItem>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "JlCompress.h"

#define BIT_NUM     (16)
#define STOR_NUM    (2)
#define WINDOW_TITLE "存储设备管理软件_J991_V1.0_200819"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowIcon(QIcon("icon/RecCtl.ico"));

    if (UIinit()) {
        UIStatusShow(QString(tr("界面初始化成功！")));
    } else {
        UIStatusShow(QString(tr("界面初始化失败！")));
        this->close();
    }

    this->setWindowTitle(WINDOW_TITLE);
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background,QColor(173,173,173));
    this ->setPalette(palette);

    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    QTimer *pTimer = new QTimer();
    //将定时器超时信号与槽(功能函数)联系起来
    connect( pTimer,SIGNAL(timeout()), this, SLOT(TimerSyncTimeFunction()) );
    //开始运行定时器，定时时间间隔为1000ms
    pTimer->start(1000);
    /**********线程初始化*******/
    m_pThrdSend = new SendThread(this);
    m_pThrdRecv = new RecvThread(this);

    m_tcpClient = new QTcpSocket(this);
    m_tcpClient->setReadBufferSize(16384);
    connect(m_tcpClient, SIGNAL(connected()), this, SLOT(HaveConnected())); //当tcp连接成功发出信号；
    connect(m_tcpClient, SIGNAL(disconnected()),this,SLOT(NotConnect()));
    connect(m_tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));

    connect(this,SIGNAL(Uimsg(QString)),this,SLOT(UIStatusShow(QString)));
    //自动连接用异步调用，避免无网络时卡死在连接过程中导致界面不能弹出
    connect(this,SIGNAL(AutoConnectSignal()),this,SLOT(on_PbConnect_clicked()),Qt::QueuedConnection);

    /************连接发送和接收线程的信号槽*****************/
    connect(m_pThrdSend,SIGNAL(SendMsgToui(QString)),this,SLOT(UIStatusShow(QString)));
    connect(m_pThrdRecv,SIGNAL(RecvMsgToui(QString)),this,SLOT(UIStatusShow(QString)));
    connect(m_pThrdRecv,SIGNAL(AllAckSignal(QByteArray)),this,SLOT(AllAckSlot(QByteArray)));
    //connect(m_pThrdRecv,SIGNAL(AllAckSignal(QByteArray)),m_pThrdSend,SLOT(AllAckSlot(QByteArray)));
    connect(m_pThrdRecv, SIGNAL(AllCmdUiSignal(QByteArray, QByteArray)), this, SLOT(AllCmdUiSlot(QByteArray, QByteArray)));
    connect(m_pThrdRecv, SIGNAL(AllCmdSendSignal(QByteArray, QByteArray)), m_pThrdSend, SLOT(AllCmdSendSlot(QByteArray, QByteArray)));
    connect(m_pThrdRecv, SIGNAL(SetSysBizTypeSignal(QByteArray)), m_pThrdSend, SLOT(SetSysBizTypeSlot(QByteArray)));

    connect(m_pThrdRecv,SIGNAL(RecvProcessToui(bool,UINT32)),this,SLOT(UpdateProgressSlot(bool,UINT32)));
    connect(m_pThrdSend,SIGNAL(SendProcessToui(bool,UINT32)),this,SLOT(UpdateProgressSlot(bool,UINT32)));

    //connect(m_tcpClient, SIGNAL(readyRead()), m_pThrdRecv, SLOT(RecvStartSlot()), Qt::DirectConnection);     //接收到数据后出发的信号

    connect(m_pThrdRecv, SIGNAL(UserLoginSignal(QByteArray)), this, SLOT(UserLoginSlot(QByteArray)));
    connect(m_pThrdRecv, SIGNAL(UserOperSignal(QByteArray)), this, SLOT(UserOperSlot(QByteArray)));
    connect(m_pThrdRecv, SIGNAL(BitSignal(QByteArray)), this, SLOT(BitSlot(QByteArray)));
    connect(m_pThrdRecv, SIGNAL(InqStatusSignal(QByteArray)), this, SLOT(InqStatusSlot(QByteArray)));
    connect(m_pThrdRecv, SIGNAL(WipeSignal(QByteArray)), this, SLOT(WipeSlot(QByteArray)));
    connect(m_pThrdRecv, SIGNAL(DirSignal(QList<CFileAttrib>)), this, SLOT(DirSlot(QList<CFileAttrib>)), Qt::DirectConnection);
    connect(m_pThrdRecv, SIGNAL(WorkStatusSignal(QByteArray)), this, SLOT(WorkStatusSlot(QByteArray)));

    connect(this, SIGNAL(HeartBeatSignal()), m_pThrdSend, SLOT(HeartBeatSlot()));
    connect(this, SIGNAL(SetTimeSignal(QDateTime)), m_pThrdSend, SLOT(SetTimeSlot(QDateTime)));
    connect(this, SIGNAL(MBitSignal()), m_pThrdSend, SLOT(BitSlot()));
    connect(this, SIGNAL(InqStatusSignal()), m_pThrdSend, SLOT(InqStatusSlot()));
    connect(this, SIGNAL(WipeSignal()), m_pThrdSend, SLOT(WipeSlot()));
    connect(this, SIGNAL(UserLoginSignal(QString, QString)), m_pThrdSend, SLOT(UserLoginSlot(QString,QString)));
    connect(this, SIGNAL(UserQuitSignal()), m_pThrdSend, SLOT(UserQuitSlot()));
    connect(this, SIGNAL(UserNewSignal(QString, QString)), m_pThrdSend, SLOT(UserNewSlot(QString, QString)));
    connect(this, SIGNAL(UserDelSignal(QString, QString)), m_pThrdSend, SLOT(UserDelSlot(QString, QString)));
    connect(this, SIGNAL(UserChgPwSignal(QString, QString)), m_pThrdSend, SLOT(UserChgPwSlot(QString, QString)));
    connect(this, SIGNAL(RecConSignal(UINT32)), m_pThrdSend, SLOT(RecConSlot(UINT32)));
    connect(this, SIGNAL(RecModeSignal(UINT32)), m_pThrdSend, SLOT(RecModeSlot(UINT32)));
    connect(this, SIGNAL(WriteProtectSignal(QVariant)), m_pThrdSend, SLOT(WriteProtectSlot(QVariant)));
    connect(this, SIGNAL(WriteProtectOffSignal(QVariant)), m_pThrdSend, SLOT(WriteProtectOffSlot(QVariant)));
    connect(this, SIGNAL(UnInstStartSignal(QVariant)), m_pThrdSend, SLOT(UnInstStartSlot(QVariant)));
    connect(this, SIGNAL(UnInstStopSignal()), m_pThrdSend, SLOT(UnInstStopSlot()));
    connect(this, SIGNAL(PlaybackStartSignal(QVariant)), m_pThrdSend, SLOT(PlaybackStartSlot(QVariant)));
    connect(this, SIGNAL(PlaybackStopSignal()), m_pThrdSend, SLOT(PlaybackStopSlot()));
    connect(this, SIGNAL(PlaybackPauseSignal(UINT32)), m_pThrdSend, SLOT(PlaybackPauseSlot(UINT32)));
    connect(this, SIGNAL(DirSignal(QVariant)), m_pThrdSend, SLOT(DirSlot(QVariant)), Qt::DirectConnection);
    connect(this, SIGNAL(FileDelSignal(QVariant)), m_pThrdSend, SLOT(FileDelSlot(QVariant)));
    connect(this, SIGNAL(SelfDestorySignal()), m_pThrdSend, SLOT(SelfDestorySlot()));
    connect(this, SIGNAL(UnmountSignal(bool)), m_pThrdSend, SLOT(UnmountSlot(bool)));
    connect(this, SIGNAL(MountSignal(bool)), m_pThrdSend, SLOT(MountSlot(bool)));
    connect(this, SIGNAL(UpdateSignal()), m_pThrdSend, SLOT(UpdateSlot()));
    connect(this, SIGNAL(ResetSignal()), m_pThrdSend, SLOT(ResetSlot()));
    connect(this, SIGNAL(PowerDownSignal()), m_pThrdSend, SLOT(PowerDownSlot()));
    connect(this, SIGNAL(SetfileszSignal(UINT32)), m_pThrdSend, SLOT(SetfileSzSlot(UINT32)));

    connect(this, SIGNAL(SetCurVecSignal(double, double, double)), m_pThrdSend, SLOT(SetCurVecSlot(double, double, double)));
    connect(this, SIGNAL(MenuActionSignal(UINT32, QStringList)), m_pThrdSend, SLOT(MenuActionSlot(UINT32, QStringList)));

    /***********发送线程和接收线程之间通信****************/
    //connect(m_pThrdRecv, SIGNAL(TransBulkSignal(QByteArray data)), m_pThrdRecv, SLOT(TransBulkSlot(QByteArray data)));

    //打开软件后自动开始链接
    emit AutoConnectSignal();
}

MainWindow::~MainWindow()
{
    m_infoStore.m_uRecCon = 0;
    for(int i=0; i<m_listChkRec.count(); i++)
    {
        if(m_listChkRec.at(i)->isChecked()) m_infoStore.m_uRecCon |= 1<<i;
    }
    SelectFilterSave();
    ReplayFilterSave();

    m_pThrdSend->quit();
    m_pThrdRecv->stop();
    m_pThrdRecv->quit();

    delete ui;

}

bool MainWindow::UIinit()
{
    bool bRes = true;

    QToolTip::setFont(QFont("宋体",12));

    m_bConnected = false;
    m_bLogined = false;

    m_listStrChannel = m_infoStore.m_listStrChannel;
    m_listStrLink << m_listStrChannel << tr("磁盘箱");
    m_listStrBit << m_listStrChannel << tr("磁盘箱") << tr("板载电子盘") << tr("扩展电子盘");;
    uiChgToolEnable();

    StatusbarInit();
    BitListInit();
    StatusTableInit();
    StatusChartInit();

    StartPageInit();
    RecPageInit();
    ReplayPageInit();
    SelectPageInit();
    OtherPageInit();
    ChannelPageInit();

    FilePageInit();

    return bRes;
}

void MainWindow::TimerSyncTimeFunction()
{
    if(ui->PbSync->isChecked())
        ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
}

void MainWindow::StartPageInit()
{
    //QRegExp regxIP("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");//IP格式正则表达式，只能按照IP地址格式输入。
    QRegExp ipRx("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-4]|[01]?\\d\\d?)");
    ui->TcpAddrEdit->setValidator(new QRegExpValidator(ipRx,ui->TcpAddrEdit));
    //ui->TcpAddrEdit->setInputMask("000.000.000.000");
    ui->TcpPortEdit->setValidator(new QIntValidator(0,65535,ui->TcpPortEdit));
    if(!m_infoStore.m_szIP.isEmpty()) ui->TcpAddrEdit->setText(m_infoStore.m_szIP);
    if(m_infoStore.m_uPort != 0) ui->TcpPortEdit->setText(QString::number(m_infoStore.m_uPort));
    if(!m_infoStore.m_strCurUser.isEmpty()) ui->UserNameEdit->setText(m_infoStore.m_strCurUser);
    if(!m_infoStore.m_strCurPw.isEmpty()) ui->PasswordEdit->setText(m_infoStore.m_strCurPw);
}

void MainWindow::FilePageInit()
{
    m_pMenu = NULL;

    ui->treeFile->setHeaderLabels(QStringList()<<tr("目录项"));
    ui->treeFile->setColumnCount(1);
    ui->treeFile->setSortingEnabled(true);  //自动排序
    ui->treeFile->setSelectionMode(QAbstractItemView::SingleSelection);  //单选

    m_listStrFile << tr("名称") << tr("修改时间") << tr("类型") << tr("大小")
                 << tr("标志位");
    ui->tableFile->setColumnCount(m_listStrFile.count());
    ui->tableFile->setHorizontalHeaderLabels(m_listStrFile);
    ui->tableFile->setSortingEnabled(false);
    ui->tableFile->setColumnWidth(0,420);
    ui->tableFile->setColumnWidth(1,160);
    ui->tableFile->setColumnWidth(2,60);
    ui->tableFile->setColumnWidth(3,120);
    ui->tableFile->setColumnWidth(4,80);

    ui->tableFile->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableFile->setSelectionBehavior(QAbstractItemView::SelectRows);  //单击选择一行
    ui->tableFile->setSelectionMode(QAbstractItemView::ExtendedSelection); //多行选中
    ui->tableFile->setAlternatingRowColors(true);

    setAcceptDrops(true);
    ui->tableFile->setDragDropMode(QAbstractItemView::DragDrop);
    ui->tableFile->setDragEnabled(true);
    ui->tableFile->setDropIndicatorShown(true);

    ui->tableFile->setContextMenuPolicy(Qt::CustomContextMenu);
    connect (ui->tableFile, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(CustomMenuRequested(QPoint)));   // right click connection
}

void MainWindow::CustomMenuRequested(QPoint pos)
{
    if(m_pMenu == NULL)
    {
        m_pMenu = new QMenu(this);
        QFont font("宋体",12,QFont::Normal);
        m_pMenu->setFont(font);

        QAction* actionPlay = new QAction(tr("回放(&P)"), this);
        actionPlay->setIcon(QIcon(":/png/png/play-128.png"));
        actionPlay->setData(QVariant::fromValue(TMCMD_PB));
        m_pMenu->addAction(actionPlay);

        m_pMenu->addSeparator();

        QAction* actionDel = new QAction(tr("删除文件(&D)"), this);
        actionDel->setIcon(QIcon(":/png/png/Trash Empty.png"));
        actionDel->setData(QVariant::fromValue(TMCMD_DEL));
        m_pMenu->addAction(actionDel);

        m_pMenu->addSeparator();

        QAction* actionWpOn = new QAction(tr("打开写保护(&W)"), this);
        actionWpOn->setIcon(QIcon(":/png/png/lock-128.png"));
        actionWpOn->setData(QVariant::fromValue(TMCMD_WPON));
        m_pMenu->addAction(actionWpOn);

        QAction* actionWpOff = new QAction(tr("关闭写保护(&Q)"), this);
        actionWpOff->setIcon(QIcon(":/png/png/unlock-128.png"));
        actionWpOff->setData(QVariant::fromValue(TMCMD_WPOFF));
        m_pMenu->addAction(actionWpOff);

        m_pMenu->addSeparator();

        QAction* actionRefresh = new QAction(tr("刷新(&R)"), this);
        actionRefresh->setIcon(QIcon(":/png/png/sinchronize-128.png"));
        actionRefresh->setData(QVariant::fromValue(TMCMD_DIR));
        m_pMenu->addAction(actionRefresh);

        connect (actionPlay, SIGNAL(triggered()), this, SLOT(MenuFileSlot()));
        connect (actionDel, SIGNAL(triggered()), this, SLOT(MenuFileSlot()));
        connect (actionWpOn, SIGNAL(triggered()), this, SLOT(MenuFileSlot()));
        connect (actionWpOff, SIGNAL(triggered()), this, SLOT(MenuFileSlot()));
        connect (actionRefresh, SIGNAL(triggered()), this, SLOT(MenuFileSlot()));
    }
    m_pMenu->exec(ui->tableFile->viewport()->mapToGlobal(pos));
}

void MainWindow::MenuFileSlot()
{
    QAction* action = qobject_cast<QAction*> (sender());
    if (action == 0) return;

    UINT32 uCmdTm = action->data().toUInt();
    if(uCmdTm == TMCMD_DIR)
    {
        on_PbInqDir_clicked();
    }
    else
    {
        QList<QTableWidgetItem *> listItem = ui->tableFile->selectedItems();
        QStringList listFile;
        for(int i=0; i<listItem.count(); i++)
        {
            INT32 col = ui->tableFile->column(listItem.at(i));//获取选中的列
            if(col == 0) listFile << listItem.at(i)->text();
        }
        emit MenuActionSignal(uCmdTm, listFile);
    }
}

void MainWindow::BitListInit()
{
    ui->listBit->setViewMode(QListWidget::IconMode);
    ui->listBit->setContentsMargins(50,5,5,5);
    for(int i=0; i<m_listStrBit.count(); i++)
    {
        QListWidgetItem *pItem = new QListWidgetItem(ui->listBit);
        pItem->setIcon(QIcon(":/png/png/yellow.png"));
        pItem->setToolTip(m_listStrBit.at(i)+": "+tr("未定义"));
        pItem->setSizeHint(QSize(40,40));
    }
    ui->listBit->setMovement(QListView::Static);
}

void MainWindow::StatusTableInit()
{
    ui->tableStatus->setColumnCount(2);
    ui->tableStatus->setRowCount(11+m_listStrLink.count());

    ui->tableStatus->setColumnWidth(0,ui->tableStatus->width()/ui->tableStatus->columnCount()-30);
    ui->tableStatus->setColumnWidth(1,ui->tableStatus->width()/ui->tableStatus->columnCount()-20);

    QStringList heads;
    heads <<tr("状态项")<<tr("明细");
    ui->tableStatus->setHorizontalHeaderLabels(heads);

    ui->tableStatus->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableStatus->setSelectionBehavior(QAbstractItemView::SelectRows);  //单击选择一行
    ui->tableStatus->setSelectionMode(QAbstractItemView::SingleSelection); //设置只能选择一行，不能多行选中
    ui->tableStatus->setAlternatingRowColors(true);
}

void MainWindow::StatusChartInit()
{
    QScrollArea *scrollArea1 = new QScrollArea(this);
    scrollArea1->setPalette(QColor(221, 221, 221));
    scrollArea1->setWidgetResizable(true);
    ui->verticalLayout_Chart->addWidget(scrollArea1);
    QWidget *scrollAreaContent1 = new QWidget();
    scrollArea1->setWidget(scrollAreaContent1);

    m_chartViewers[0] = new QChartViewer(scrollAreaContent1);

    QScrollArea *scrollArea2 = new QScrollArea(this);
    scrollArea2->setPalette(QColor(221, 221, 221));
    scrollArea2->setWidgetResizable(true);
    ui->verticalLayout_Chart->addWidget(scrollArea2);
    QWidget *scrollAreaContent2 = new QWidget();
    scrollArea2->setWidget(scrollAreaContent2);

    m_chartViewers[1] = new QChartViewer(scrollAreaContent2);
}

void MainWindow::RecPageInit()
{
    m_listChkRec << ui->chkRec_A << ui->chkRec_B;

    for(int i=0; i<m_listChkRec.count(); i++)
    {
        m_listChkRec.at(i)->setChecked((m_infoStore.m_uRecCon&(1<<i)) != 0);
        m_listChkRec.at(i)->setToolTip(m_listStrChannel.at(i));
    }
}
void MainWindow::on_chkTypeSelReplay_clicked()
{
    bool bChecked = ui->chkTypeSelReplay->isChecked();
    for(INT32 i=0; i<m_listChkTypeReplay.count(); i++)
    {
        m_listChkTypeReplay.at(i)->setEnabled(bChecked);
    }
}

void MainWindow::ReplayPageInit()
{
    m_listChkReplay << ui->chkReplay_A << ui->chkReplay_B;
    m_listChkTypeReplay<<ui->chkTypeReplay1<<ui->chkTypeReplay2<<ui->chkTypeReplay3<<ui->chkTypeReplay4;
    ui->chkChnSelReplay->setChecked((m_infoStore.m_filterReplay.uValid&FIELD__CHAN) == FIELD__CHAN);
    on_chkChnSelReplay_clicked();
    for(int i=0; i<m_listChkReplay.count(); i++)
    {
        m_listChkReplay.at(i)->setChecked((m_infoStore.m_filterReplay.uChannel&(1<<i)) != 0);
        m_listChkReplay.at(i)->setToolTip(m_listStrChannel.at(i));
    }
    ui->chkTimeSelReplay->setChecked((m_infoStore.m_filterReplay.uValid&FIELD__TIME_BEGIN) == FIELD__TIME_BEGIN);
    on_chkTimeSelReplay_clicked();
    ui->dateTimeEditSelBegReplay->setDateTime(m_infoStore.m_filterReplay.timeBeg);
    ui->dateTimeEditSelEndReplay->setDateTime(m_infoStore.m_filterReplay.timeEnd);
    on_chkTypeSelReplay_clicked();
    for(int i=0; i<m_listChkTypeReplay.count(); i++)
        m_listChkTypeReplay.at(i)->setChecked((m_infoStore.m_filterReplay.uRange&(1<<i)) != 0);
    on_chkChanSelReplay_clicked();
}

void MainWindow::on_chkTypeSelSelect_clicked()
{
    bool bChecked = ui->chkTypeSelSelect->isChecked();
    for(INT32 i=0; i<m_listChkTypeSelect.count(); i++)
    {
        m_listChkTypeSelect.at(i)->setEnabled(bChecked);
    }
}

void MainWindow::SelectPageInit()
{
    m_listChkSelect << ui->chkSelect_A << ui->chkSelect_B;
    m_listChkTypeSelect<<ui->chkTypeSelect1<<ui->chkTypeSelect2<<ui->chkTypeSelect3<<ui->chkTypeSelect4;

    ui->chkChnSelSelect->setChecked((m_infoStore.m_filterSelect.uValid&FIELD__CHAN) == FIELD__CHAN);
    on_chkChnSelSelect_clicked();
    for(int i=0; i<m_listChkSelect.count(); i++)
    {
        m_listChkSelect.at(i)->setChecked((m_infoStore.m_filterSelect.uChannel&(1<<i)) != 0);
        m_listChkSelect.at(i)->setToolTip(m_listStrChannel.at(i));
    }

    ui->chkTimeSelSelect->setChecked((m_infoStore.m_filterSelect.uValid&FIELD__TIME_BEGIN) == FIELD__TIME_BEGIN);
    on_chkTimeSelSelect_clicked();
    ui->dateTimeEditSelBegSelect->setDateTime(m_infoStore.m_filterSelect.timeBeg);
    ui->dateTimeEditSelEndSelect->setDateTime(m_infoStore.m_filterSelect.timeEnd);

    on_chkTypeSelSelect_clicked();
    for(int i=0; i<m_listChkTypeSelect.count(); i++)
        m_listChkTypeSelect.at(i)->setChecked((m_infoStore.m_filterSelect.uRange&(1<<i)) != 0);
}

void MainWindow::ReplayFilterSave()
{
    m_infoStore.m_filterReplay.uValid = 0;
    if(ui->chkChnSelReplay->isChecked()) m_infoStore.m_filterReplay.uValid |= FIELD__CHAN;
    m_infoStore.m_filterReplay.uChannel = 0;
    for(int i=0; i<m_listChkReplay.count(); i++)
        if(m_listChkReplay.at(i)->isChecked()) m_infoStore.m_filterReplay.uChannel |= (1<<i);

    if(ui->chkTimeSelReplay->isChecked()) m_infoStore.m_filterReplay.uValid |= FIELD__TIME_BEGIN | FIELD__TIME_END;
    m_infoStore.m_filterReplay.timeBeg = ui->dateTimeEditSelBegReplay->dateTime();
    m_infoStore.m_filterReplay.timeEnd = ui->dateTimeEditSelEndReplay->dateTime();
    if(ui->chkTypeSelReplay->isChecked()) m_infoStore.m_filterReplay.uValid |= FIELD__DATA_TYPE | FIELD__DATA_TYPE;
    m_infoStore.m_filterReplay.uRange = 0;
    for(int i=0; i<m_listChkTypeReplay.count(); i++)
        if(m_listChkTypeReplay.at(i)->isChecked()) m_infoStore.m_filterReplay.uRange |= (1<<i);
}

void MainWindow::SelectFilterSave()
{
    m_infoStore.m_filterSelect.uValid = 0;
    if(ui->chkChnSelSelect->isChecked()) m_infoStore.m_filterSelect.uValid |= FIELD__CHAN;
    m_infoStore.m_filterSelect.uChannel = 0;
    for(int i=0; i<m_listChkSelect.count(); i++)
        if(m_listChkSelect.at(i)->isChecked()) m_infoStore.m_filterSelect.uChannel |= (1<<i);

    if(ui->chkTimeSelSelect->isChecked()) m_infoStore.m_filterSelect.uValid |= FIELD__TIME_BEGIN | FIELD__TIME_END;
    m_infoStore.m_filterSelect.timeBeg = ui->dateTimeEditSelBegSelect->dateTime();
    m_infoStore.m_filterSelect.timeEnd = ui->dateTimeEditSelEndSelect->dateTime();
    m_infoStore.m_filterSelect.uValid |= FIELD__DATA_TYPE | FIELD__DATA_TYPE;
    m_infoStore.m_filterSelect.uRange = 0;
    m_infoStore.m_filterSelect.uRange |= (1<<0);
    if(ui->chkTypeSelSelect->isChecked()) m_infoStore.m_filterSelect.uValid |= FIELD__DATA_TYPE | FIELD__DATA_TYPE;
    m_infoStore.m_filterSelect.uRange = 0;
    for(int i=0; i<m_listChkTypeSelect.count(); i++)
        if(m_listChkTypeSelect.at(i)->isChecked()) m_infoStore.m_filterSelect.uRange |= (1<<i);
}

void MainWindow::OtherPageInit()
{
    QRegExp double_rx180("^-?(180(\\.0{1,5})?|(1?[0-7]?\\d|\\d?\\d?)(\\.\\d{1,5})?)$");
    QRegExp double_rx90("^-?(90(\\.0{1,5})?|[1-8]?\\d(\\.\\d{1,5})?)$");
    QRegExp double_rx360("^((([0-2])?\\d{1,2}|3[0-5]\\d)(\\.\\d{1})?|360(\\.0{1})?)$");
}

void MainWindow::ChannelPageInit()
{
    QStringList a;
    a << "A" << "B" << "C" << "D" << "E" << "F" << "G"
      << "H" << "I" << "J" << "K" << "L" << "M" << "N" << "O";
    ui->tableChannel->setColumnCount(a.count());
    ui->tableChannel->setRowCount(2);

    QStringList heads;
    heads << tr("标识") << tr("通道名");
    ui->tableChannel->setVerticalHeaderLabels(heads);

    QHeaderView *headerView = ui->tableChannel->horizontalHeader();
    headerView->setHidden(true);

    ui->tableChannel->setEditTriggers(QAbstractItemView::AllEditTriggers);
    ui->tableChannel->setSelectionBehavior(QAbstractItemView::SelectColumns);  //单击选择一列
    ui->tableChannel->setSelectionMode(QAbstractItemView::SingleSelection); //设置只能选择一列，不能多行选中
    ui->tableChannel->setAlternatingRowColors(true);

    ui->tableChannel->horizontalHeader()->setStretchLastSection(true);
    ui->tableChannel->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    for(int i=0; i<ui->tableChannel->columnCount(); i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem(a.at(i));
        item->setFlags(Qt::NoItemFlags);
        ui->tableChannel->setItem(0, i, item);
        QTableWidgetItem *item1 = new QTableWidgetItem(m_listStrChannel.at(i));
        ui->tableChannel->setItem(1, i, item1);

        ui->tableChannel->setColumnWidth(i,(ui->tableChannel->width()-ui->tableChannel->verticalHeader()->width()-60)/ui->tableChannel->columnCount());
    }
}

void MainWindow::StatusbarInit()
{
    m_pbStatNet = new QPushButton;
    m_pbStatNet->setMinimumSize(m_pbStatNet->sizeHint());
    m_pbStatNet->setToolTip(tr("显示与设备的连接状态"));
    m_pbStatNet->setFlat(true);
    QFont font("宋体",12,QFont::Bold);
    m_pbStatNet->setFont(font);
    m_pbStatNet->setIcon(QIcon(":/ico/icon/dead.ico"));
    m_pbStatNet->setText(tr("未连接"));
    ui->statusBar->addWidget(m_pbStatNet);

    for(int i=0; i<m_listStrLink.count(); i++)
    {
        QLabel *pLable = new QLabel;
        pLable->setMinimumSize(pLable->sizeHint());
        pLable->setPixmap(QPixmap(":/png/png/ledyellow.png"));
        pLable->setToolTip(m_listStrLink.at(i)+": "+tr("未定义"));

        ui->statusBar->addPermanentWidget(pLable);
        m_listStatLink << pLable;
    }

    QFrame *separator1 = new QFrame;
    separator1->setFrameShape(QFrame::VLine);
    ui->statusBar->addWidget(separator1);

    m_labelStatUser = new QLabel;
    m_labelStatUser->setMinimumSize(m_labelStatUser->sizeHint());
    m_labelStatUser->setAlignment(Qt::AlignLeft);
    m_labelStatUser->setText(tr("当前用户:         "));
    ui->statusBar->addWidget(m_labelStatUser);

    QFrame *separator2 = new QFrame;
    separator2->setFrameShape(QFrame::VLine);
    ui->statusBar->addWidget(separator2);

    m_labelStatWork = new QLabel;
    m_labelStatWork->setMinimumSize(m_labelStatWork->sizeHint());
    m_labelStatWork->setAlignment(Qt::AlignLeft);
    m_labelStatWork->setText(tr("工作状态: 空闲"));
    ui->statusBar->addWidget(m_labelStatWork);

    QFrame *separator3 = new QFrame;
    separator3->setFrameShape(QFrame::VLine);
    ui->statusBar->addWidget(separator3);

    m_pProgress = new QProgressBar;
    m_pProgress->setOrientation(Qt::Horizontal);  // 水平方向
    m_pProgress->setMinimum(0);  // 最小值
    m_pProgress->setMaximum(100);  // 最大值
    m_pProgress->hide();

    ui->statusBar->addWidget(m_pProgress);

    ui->statusBar->setStyleSheet(QString("QStatusBar::item{border: 0px}"));
}

void MainWindow::uiChgToolEnable(bool bEnable)
{
    ui->PbDisConnect->setEnabled(m_bConnected);
    ui->PbConnect->setEnabled(!m_bConnected);

    ui->toolUserLogin->setEnabled(m_bConnected && !m_bLogined && bEnable);
    ui->toolUserQuit->setEnabled(m_bConnected && m_bLogined && bEnable);
    ui->toolUserNew->setEnabled(m_bConnected && m_bLogined && bEnable);
    ui->toolUserDel->setEnabled(m_bConnected && m_bLogined && bEnable);
    ui->toolUserChgPw->setEnabled(m_bConnected && m_bLogined && bEnable);
    ui->toolFormat->setEnabled(m_bConnected && m_bLogined && bEnable);
    ui->toolSelfDest->setEnabled(m_bConnected && m_bLogined && bEnable);
    ui->toolDelFile->setEnabled(m_bConnected && m_bLogined && bEnable);
    ui->toolUpdate->setEnabled(m_bConnected && m_bLogined && bEnable);
    ui->PbRec->setEnabled(m_bConnected && m_bLogined && bEnable);
    ui->PbRecStop->setEnabled(false);
    ui->SetFilesz->setEnabled(m_bConnected && m_bLogined && bEnable);
    ui->filesz->setEnabled(m_bConnected && m_bLogined && bEnable);
    ui->szlogo->setEnabled(m_bConnected && m_bLogined && bEnable);
    ui->PbReplay->setEnabled(m_bConnected && m_bLogined && bEnable);
    ui->PbReplayPause->setEnabled(false);
    ui->PbReplayStop->setEnabled(false);
    ui->PbReset->setEnabled(m_bConnected && m_bLogined && bEnable);
    ui->PbPowerDown->setEnabled(m_bConnected && m_bLogined && bEnable);
    ui->PbInqDir->setEnabled(m_bConnected && m_bLogined && bEnable);
    ui->PbBit->setEnabled(m_bConnected && m_bLogined && bEnable);
    ui->PbBitSmall->setEnabled(m_bConnected && m_bLogined && bEnable);
    ui->PbSetTime->setEnabled(m_bConnected && m_bLogined && bEnable);
}

void MainWindow::uiChgBizEnable(bool bEnable)
{
    ui->toolDelFile->setEnabled(bEnable && m_bLogined);
    ui->PbInqDir->setEnabled(bEnable && m_bLogined);
    ui->toolFormat->setEnabled(bEnable && m_bLogined);
    ui->PbDisConnect->setEnabled(bEnable);
    ui->toolUserQuit->setEnabled(m_bLogined && bEnable);
}

void MainWindow::FunInIdleEnable(bool bEnable)
{
    ui->PbRec->setEnabled(m_bConnected && m_bLogined && bEnable);
    ui->PbRecStop->setEnabled(false);
    ui->PbReplay->setEnabled(m_bConnected && m_bLogined && bEnable);
    ui->PbReplayPause->setEnabled(false);
    ui->PbReplayStop->setEnabled(false);
}

void MainWindow::UIStatusShow(QString msg)  //ui 上的窗口显示状态
{
    ui->listStatus->append(tr("[")+DATETIME+tr("] ")+msg);
}

void MainWindow::UpdateProgressSlot(bool bShow, UINT32 percent)
{
    if(bShow) {
        m_pProgress->show();
        m_pProgress->setValue(percent);  // 当前进度
    } else {
            m_pProgress->hide();
    }

}

void MainWindow::HaveConnected()
{
    emit Uimsg(tr("网络连接成功"));

    m_pThrdRecv->SetSocketWord(m_tcpClient);
    m_pThrdSend->SetSocketWord(m_tcpClient);
    m_pThrdRecv->start(); //开始接收线程
    m_pThrdRecv->setPriority(QThread::HighPriority);
    //m_pThrdSend->start(); //开始发送线程

    m_bConnected = true;
    m_bLogined = false;
    uiChgToolEnable();

    m_pbStatNet->setIcon(QIcon(":/ico/icon/alive.ico"));
    m_pbStatNet->setText(tr("已连接"));

    m_bAlive = true;

    m_bNeedRecMode = true;

    QTimer *pTimer = new QTimer();

    //将定时器超时信号与槽(功能函数)联系起来
    connect( pTimer,SIGNAL(timeout()), this, SLOT(TimerHeartBeatFunction()) );

    //开始运行心跳定时器，定时时间间隔为1000ms
    pTimer->start(1000);

    //连接成功后自动登录
    on_toolUserLogin_clicked();
}

void MainWindow::NotConnect()
{
    m_bConnected = false;
    uiChgToolEnable();

    m_pbStatNet->setIcon(QIcon(":/ico/icon/dead.ico"));
    m_pbStatNet->setText(tr("未连接"));

    if(!m_bManual)
    {
        emit Uimsg(tr("TCP连接失败,正在重试")+QString::number(m_uRetry,10)+tr("次"));
        Mysleep(5000);
        m_uRetry++;
        ConnectServer();
    }
}

void MainWindow::ConnectServer()
{
    if(!m_bConnected) {
        m_infoStore.m_uPort = ui->TcpPortEdit->text().toUInt(0,10);
        m_infoStore.m_szIP = ui->TcpAddrEdit->text().toLatin1();
        m_tcpClient->connectToHost(m_infoStore.m_szIP,m_infoStore.m_uPort);
        emit Uimsg(tr("正在连接")+m_infoStore.m_szIP+tr(":")+QString::number(m_infoStore.m_uPort,10)+tr("..."));
    }
}

void MainWindow::displayError(QAbstractSocket::SocketError)
{
    emit Uimsg(tr("网络错误原因：")+ m_tcpClient->errorString());
    emit Uimsg(tr("TCP连接错误，正在重试"));
    Mysleep(5000);
    ConnectServer();
}

void MainWindow::on_PbConnect_clicked()
{
    m_bManual = false;
    m_uRetry = 1;
    ConnectServer();
}

void MainWindow::on_PbDisConnect_clicked()
{
    m_bManual = true;
    m_tcpClient->disconnectFromHost();
    emit Uimsg(tr("网络已断开"));
}

void MainWindow::AllAckSlot(QByteArray head)
{
    //有ack表示设备还有应答
    m_bAlive = true;
}

void MainWindow::TimerHeartBeatFunction()
{
    emit HeartBeatSignal();
}

//void MainWindow::on_PbInqStatus_clicked()
//{
//    emit InqStatusSignal();
//}

void MainWindow::on_PbBit_clicked()
{
    ui->PbBit->setEnabled(false);
    ui->PbBitSmall->setEnabled(false);

    //置状态为初始
    for(int i=0; i<ui->listBit->count(); i++)
    {
        ui->listBit->item(i)->setIcon(QIcon(":/png/png/yellow.png"));
    }

    emit MBitSignal();

    Mysleep(500);
    ui->PbBit->setEnabled(true);
    ui->PbBitSmall->setEnabled(true);
}

void MainWindow::BitSlot(QByteArray data)
{
    RW_MSGR__ADM_SELFDIAG *pData = (RW_MSGR__ADM_SELFDIAG *)data.data();

    int idx = 0;
    ui->tabWidget_left->setCurrentIndex(1);
    for(int i=0; i<BIT_NUM; i++)
    {
        UINT32 *pU = (UINT32*)&pData->chan_stat;

        QListWidgetItem *pItem = ui->listBit->item(idx);
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
    for(int i=0; i<STOR_NUM; i++)
    {
        QListWidgetItem *pItem = ui->listBit->item(idx);

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

void MainWindow::ChartUpdate(int idx, double dblUsed, double dblUsable)
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

void MainWindow::InqStatusSlot(QByteArray data)
{
    RW_MSGR__ADM_INQSTATUS *pData = (RW_MSGR__ADM_INQSTATUS *)data.data();

    int idx = 0;
    if((pData->capboard+pData->capextboard >= pData->capusable)
            && ((pData->capusable>1)   //卸载时有可能会卸载磁盘箱而使得可用容量为0，但是磁盘箱容量有数，此时不更新磁盘箱状态
            || (pData->capboard+pData->capextboard+pData->capusable<100)))    //磁盘箱卸载后可用容量和总容量均为0，此时不作图（在chartupdate中会根据此判断）
    {
        ui->tableStatus->setItem(idx,0,new QTableWidgetItem(tr("设备总容量")));
        ui->tableStatus->setItem(idx++,1,new QTableWidgetItem(QString::number(pData->capboard*1.024*1.024/1000.0*CAP_UNIT, 'f', 2)+tr(" MB")));

        ui->tableStatus->setItem(idx,0,new QTableWidgetItem(tr("设备可用容量")));
        ui->tableStatus->setItem(idx++,1,new QTableWidgetItem(QString::number(pData->capusable*1.024/1000.0*CAP_UNIT, 'f', 2)+tr(" MB")));

        ChartUpdate(0, (pData->capboard+pData->capextboard-pData->capusable)*1.3/1000.0*CAP_UNIT, pData->capusable*1.3/1000.0*CAP_UNIT);
    }
    else
    {
        ui->tableStatus->setItem(idx,0,new QTableWidgetItem(tr("设备总容量")));
        ui->tableStatus->setItem(idx++,1,new QTableWidgetItem(QString::number(pData->capboard*1.024*1.024/1000.0*CAP_UNIT, 'f', 2)+tr(" MB")));

        ui->tableStatus->setItem(idx,0,new QTableWidgetItem(tr("设备可用容量")));
        ui->tableStatus->setItem(idx++,1,new QTableWidgetItem(QString::number(pData->capusable*1.024/1000.0*CAP_UNIT, 'f', 2)+tr(" MB")));
    }

    QLabel *pLabel;
    for(int i=0; i<2; i++)
    {
        ui->tableStatus->setItem(idx,0,new QTableWidgetItem(m_listStrLink.at(i)+tr("链路状态")));

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
        ui->tableStatus->setItem(idx++,1,new QTableWidgetItem(sz));
    }

    ui->tableStatus->setItem(idx,0,new QTableWidgetItem(tr("总记录速度")));
    pData->fc_speed = pData->fc_speed * 1.024 * 1.024;
    ui->tableStatus->setItem(idx++, 1, new QTableWidgetItem(
        QString::number(pData->fc_speed, 10)+get_speed_unit_str(pData->fc_speed_unit)+tr("/s")));
    //strSpeedFC = sz;
    ui->tableStatus->setItem(idx,0,new QTableWidgetItem(tr("设备工作状态")));
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
    ui->tableStatus->setItem(idx++,1,new QTableWidgetItem(szWork));

    uint64_t val = pData->caphd;
    uint32_t temp = ((val >> 32) & 0xffffffff)/10;
    float vol_int = (val & 0xffffffff)/100.0;

    ui->tableStatus->setItem(idx,0,new QTableWidgetItem(tr("设备温度")));
    ui->tableStatus->setItem(idx++,1,new QTableWidgetItem(QString::number(temp, 10).toUpper()+tr(" 度")));

    ui->tableStatus->setItem(idx,0,new QTableWidgetItem(tr("设备电压INT"),10));
    ui->tableStatus->setItem(idx++,1,new QTableWidgetItem(QString("%1").arg(vol_int) + tr(" V")));
    val = pData->caphdusable;
    float vol_ddr= ((val >> 32) & 0xffffffff)/100.0;
    float vol_aux = (val & 0xffffffff)/100.0;
    ui->tableStatus->setItem(idx,0,new QTableWidgetItem(tr("设备电压DDR"),10));
    ui->tableStatus->setItem(idx++,1,new QTableWidgetItem(QString("%1").arg(vol_ddr)+ tr(" V")));
    ui->tableStatus->setItem(idx,0,new QTableWidgetItem(tr("设备电压AUX"),10));
    ui->tableStatus->setItem(idx++,1,new QTableWidgetItem(QString("%1").arg(vol_aux) + tr(" V")));
}

void MainWindow::WipeSlot(QByteArray data)
{
    RW_MSGR__MED_WIPESTOR *pMsgr = (RW_MSGR__MED_WIPESTOR*)data.data();

    QString sz;
    if(pMsgr->done_flag != WIPE_DONE)
        sz = QString("文件清除中(%1%)...").arg(pMsgr->progress);
    else {
        sz = tr("文件清除已完成。");
    }
    emit Uimsg(sz);

    if(pMsgr->done_flag == WIPE_DONE)
    {
        UpdateProgressSlot(false);
    } else {
        UpdateProgressSlot(true, pMsgr->progress);
    }
}

void MainWindow::UserLoginSlot(QByteArray data)
{
    RW_MSGR__USR_LOGIN *pMsg = (RW_MSGR__USR_LOGIN *)data.data();
    if((pMsg->result == LOGIN_OK) || (m_bLogined))
    {
        m_labelStatUser->setText(tr("当前用户: ")+m_infoStore.m_strCurUser);
        emit Uimsg(tr("用户")+m_infoStore.m_strCurUser+tr("登录成功。"));
        m_bLogined = true;
        uiChgToolEnable();
    }
    else
    {
        m_bLogined = false;

        uiChgToolEnable();

        emit Uimsg(tr("用户")+m_infoStore.m_strCurUser+tr("登录失败！错误代码：%1。").arg(pMsg->result));
    }
}

void MainWindow::UserOperSlot(QByteArray data)
{
    RW_MSGR__USR_LOGIN *pMsg = (RW_MSGR__USR_LOGIN *)data.data();
    if(pMsg->result == 0)
    {
        emit Uimsg(tr("操作用户成功。"));
    }
    else
    {
        emit Uimsg(tr("操作用户失败。"));
    }
}

void MainWindow::AllCmdUiSlot(QByteArray head, QByteArray data)
{
    cmi_msgr_all_cmdresult *pMsgr = (cmi_msgr_all_cmdresult *)data.data();
    QString sz;
    if(pMsgr->flag & CMI_CMDRESULT_FLAG__DONE)
    {
        //命令结束
        //UpdateProgressSlot(true, 100);

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
        //result是进度
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
    }
    if(!sz.isEmpty()) emit Uimsg(sz);
}

void MainWindow::on_PbRec_clicked()
{
    //连接后第一次点击记录时，应保证先发一次记录模式
    if(m_bNeedRecMode)
    {
    }

    UINT32 uRecCon = 0;
    for(int i=0; i<m_listChkRec.count(); i++)
    {
        if(m_listChkRec.at(i)->isChecked()) uRecCon |= 1<<i;
    }

    //ui->PbRec->setChecked(true);
    //ui->PbRecStop->setChecked(false);

    //ui->PbRec->setEnabled(false);
    ui->PbRecStop->setEnabled(true);

    //for(int i=0; i<m_listChkRec.count(); i++)
    //{
    //    m_listChkRec.at(i)->setEnabled(false);
    //}

    ui->PbRec->setEnabled(false);
    emit RecConSignal(uRecCon);
    Mysleep(1000);
    ui->PbRec->setEnabled(true);
}

void MainWindow::on_PbRecStop_clicked()
{
    UINT32 uRecCon = 0;
    for(int i=0; i<m_listChkRec.count(); i++)
    {
        if(m_listChkRec.at(i)->isChecked()) uRecCon &= ~(1<<i);
    }

    //ui->PbRec->setChecked(false);
    //ui->PbRecStop->setChecked(true);

    //ui->PbRec->setEnabled(true);
    ui->PbRecStop->setEnabled(false);

    //for(int i=0; i<m_listChkRec.count(); i++)
    //{
    //    m_listChkRec.at(i)->setEnabled(true);
    //}

    emit RecConSignal(uRecCon);
}

void MainWindow::on_toolUserLogin_clicked()
{
    if(m_bLogined)
    {
        emit Uimsg(tr("警告：必须退出当前用户才能重新登录！"));
        return;
    }

    m_infoStore.m_strCurUser = ui->UserNameEdit->text();
    m_infoStore.m_strCurPw = ui->PasswordEdit->text();
    m_infoStore.m_bAutoLogin = true;
    m_infoStore.m_bRememberPw = true;

    emit UserLoginSignal(m_infoStore.m_strCurUser, m_infoStore.m_strCurPw);

#ifdef MY_DEBUG
    m_bLogined = true;
    uiChgToolEnable();
#endif
}

void MainWindow::on_toolUserQuit_clicked()
{
    m_bLogined = false;
    uiChgToolEnable();
    m_labelStatUser->setText(tr("当前用户:         "));

    emit UserQuitSignal();
}

void MainWindow::on_toolUserNew_clicked()
{
    ui->toolUserNew->setEnabled(false);
    emit UserNewSignal(ui->UserNameAltEdit->text(), ui->PasswordAltEdit->text());
    Mysleep(500);
    ui->toolUserNew->setEnabled(true);
}

void MainWindow::on_toolUserChgPw_clicked()
{
    ui->toolUserChgPw->setEnabled(false);
    emit UserChgPwSignal(ui->UserNameAltEdit->text(), ui->PasswordAltEdit->text());
    Mysleep(500);
    ui->toolUserChgPw->setEnabled(true);
}

void MainWindow::on_toolUserDel_clicked()
{
    ui->toolUserDel->setEnabled(false);
    emit UserDelSignal(ui->UserNameAltEdit->text(), ui->PasswordAltEdit->text());
    Mysleep(500);
    ui->toolUserDel->setEnabled(true);
}

void MainWindow::on_PbSetTime_clicked()
{
    ui->PbSetTime->setEnabled(false);
    emit SetTimeSignal( ui->dateTimeEdit->dateTime());
    Mysleep(500);
    ui->PbSetTime->setEnabled(true);
}

void MainWindow::on_toolFormat_clicked()
{
    ui->toolFormat->setEnabled(false);
    emit WipeSignal();
    Mysleep(5000);
    ui->toolFormat->setEnabled(true);
}

void MainWindow::on_toolSelfDest_clicked()
{
    ui->toolSelfDest->setEnabled(false);
    emit SelfDestorySignal();
    m_bLogined = false;
    uiChgToolEnable(false);
    Mysleep(5000);
}

void MainWindow::on_toolUpdate_clicked()
{
    ui->toolUpdate->setEnabled(false);
    emit UpdateSignal();
    Mysleep(500);
    ui->toolUpdate->setEnabled(true);
}

void MainWindow::on_PbReset_clicked()
{
    ui->PbReset->setEnabled(false);
    emit ResetSignal();
    m_bLogined = false;
    uiChgToolEnable(false);
    Mysleep(5000);
    m_bManual = false;
    m_tcpClient->disconnectFromHost();
    emit Uimsg(tr("网络已断开"));
}


void MainWindow::DirSlot(QList<CFileAttrib> list)
{
    ui->tableFile->clearContents();
    ui->treeFile->clear();

    if(list.isEmpty()) return;

    m_mutexFile.lock();

    //文件列表存一个副本，后面的文件列表和气泡会用
    m_listFile = list;

    //建立并编历树，确定该放哪个位置
    //这里有4重循环，因为要遍历3级节点
    //树里面data第0列存的是文件名QString(只能存QT的元素类型，其它类型不安全)
    //     data第1列存的是层级
    //如果以后有扩展，还可以增加data的列数
    for (int i = 0; i < (int)m_listFile.count(); i++)
    {
        QTreeWidgetItem *pParent = NULL;
        CFileAttrib fSrc = m_listFile.at(i);
        UINT32 nTier = 0;
        BOOL bNeedAdd = true;
        for(int j=0; j<ui->treeFile->topLevelItemCount(); j++)
        {
            CFileAttrib fDest;
            if(!fDest.ConvertFromFilename(
                ui->treeFile->topLevelItem(j)->data(0,Qt::UserRole).toString()))
            {   //无效，继续下一个
                continue;
            }
            if(fSrc.IsSameDate(&fDest))
            {
                nTier = 1;
                pParent = ui->treeFile->topLevelItem(j);
                //找到同一天，继续向下找
                for(int m=0; m<ui->treeFile->topLevelItem(j)->childCount(); m++)
                {
                    if(!fDest.ConvertFromFilename(ui->treeFile->topLevelItem(j)->child(m)->data(0,Qt::UserRole).toString()))
                    {
                        //无效，继续下一个
                        continue;
                    }
                    if(fSrc.IsSameChannel(&fDest))
                    {
                        nTier = 2;
                        pParent = ui->treeFile->topLevelItem(j)->child(m);
                        //找到同一通道，继续向下找
                        for(int n=0; n<ui->treeFile->topLevelItem(j)->child(m)->childCount(); n++)
                        {
                            if(!fDest.ConvertFromFilename(ui->treeFile->topLevelItem(j)->child(m)->child(n)->data(0,Qt::UserRole).toString()))
                            {
                                //无效，继续下一个
                                continue;
                            }
                            if(fSrc.IsSameDtype(&fDest))
                            {
                                //找到同一数据类型，跳出，不添加
                                bNeedAdd = false;
                                break;
                            }
                        }
                        //有发现，跳出本层循环
                        break;
                    }
                }
                //有发现，跳出本层循环
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

void MainWindow::InsertTreeItem(CFileAttrib *pfileAttribSrc, INT32 nTier, QTreeWidgetItem *pParent)
{
    QTreeWidgetItem *A, *B, *C;
    //nTier表示是第几级目录
    switch(nTier)
    {//以下0、1没有break，因为等于0时需要加3层，等于1时需要加2层
    case 0:
        A = new QTreeWidgetItem(QStringList()<<pfileAttribSrc->GetCreateDate());
        //data使用自定义数据，只能放QT的数据类型，这里放了文件名，下同
        //QTreeWidgetItem的data可以放多列数据，目前这里只有2列
        A->setData (0, Qt::UserRole, pfileAttribSrc->GetFullFilename());
        A->setData (1, Qt::UserRole, 0);
        ui->treeFile->addTopLevelItem(A);
        Q_ASSERT(pParent==NULL);    //到这个分支的时候pParent必然是空，因为顶层节点没有父节点
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

void MainWindow::on_treeFile_itemSelectionChanged()
{
    QList<QTreeWidgetItem *> listItemSelected = ui->treeFile->selectedItems();
    if(listItemSelected.count()==0) return;

    //前面设置了单选，所以这里选中的只会有1个，参数直接用0
    CFileAttrib fSrc;
    fSrc.ConvertFromFilename(listItemSelected.at(0)->data(0, Qt::UserRole).toString());
    INT32 nTier = listItemSelected.at(0)->data(1, Qt::UserRole).toInt();

    ui->tableFile->clearContents();

    //先设置足够大，否则超过部分显示不出来，后面按实际行数设置
    ui->tableFile->setRowCount(65536);
    ui->tableFile->setDragEnabled(false);

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
            // dtype
            if (!fSrc.IsSameDtype(&fCompare))
                break;
        case 1:
            // channel name
            if (!fSrc.IsSameChannel(&fCompare))
                break;
        case 0:
            // date
            if (!fSrc.IsSameDate(&fCompare))
                break;
        default:
            //tr("名称") << tr("修改时间") << tr("类型") << tr("大小") << tr("标志位");
            ui->tableFile->setItem(idx, 0, new QTableWidgetItem(fCompare.GetFullFilename()));
            ui->tableFile->setItem(idx, 1, new QTableWidgetItem(fCompare.GetCreateDateTime()));
            ui->tableFile->setItem(idx, 2, new QTableWidgetItem(fCompare.GetDataType()));
            ui->tableFile->setItem(idx, 3, new QTableWidgetItem(fCompare.GetFileLength()));
            ui->tableFile->item(idx, 0)->setIcon(QIcon(":/ico/icon/file.ico"));
            ui->tableFile->item(idx, 0)->setToolTip(
                        QString("文件名： %1\n创建时间： %2\n文件大小: %3").arg(fCompare.GetFullFilename())
                        .arg(fCompare.GetCreateDateTime()).arg(fCompare.GetFileLength()));
            ui->tableFile->item(idx, 3)->setTextAlignment(Qt::AlignVCenter|Qt::AlignRight);
            if (fCompare.bProtected)	strFlag += "W";
            if (fCompare.bCompressed)	strFlag += "C";
            if (fCompare.bEncryped)	strFlag += "E";
            if (fCompare.bMarked)	strFlag += "M";
            ui->tableFile->setItem(idx, 4, new QTableWidgetItem(strFlag,10));
            idx++;
            break;
        }
    }
    ui->tableFile->setRowCount(idx);

    m_mutexFile.unlock();
}

void MainWindow::on_tabWidget_main_currentChanged(int index)
{
    if(index==0)
    {
        ui->tabWidget_main->setBaseSize(ui->tabWidget_main->baseSize().width(),120);
    }
    else if(index==3)
    {
        ui->tabWidget_main->setBaseSize(ui->tabWidget_main->baseSize().width(),160);
    }

}

void MainWindow::on_chkChnSelSelect_clicked()
{
    bool bChecked = ui->chkChnSelSelect->isChecked();
    for(int i=0; i<m_listChkSelect.count(); i++)
    {
        m_listChkSelect.at(i)->setEnabled(bChecked);
    }
}

void MainWindow::on_chkTimeSelSelect_clicked()
{
    bool bChecked = ui->chkTimeSelSelect->isChecked();
    ui->dateTimeEditSelBegSelect->setEnabled(bChecked);
    ui->dateTimeEditSelEndSelect->setEnabled(bChecked);
}


QVariant MainWindow::ReadLimitSelect()
{
    T_FILTER filter;
    filter.uFileFlags = 0;
    filter.uValid = 0;
    if(ui->chkChnSelSelect->isChecked()) filter.uValid |= FIELD__CHAN;
    filter.uChannel = 0;
    for(int i=0; i<2; i++)
    {
        if(m_listChkSelect.at(i)->isChecked())
            filter.uChannel |= 1<<(i);
    }
/*
    for(int i=3; i<5; i++)
    {
        if(m_listChkSelect.at(i)->isChecked())
            filter.uChannel |= 1<<(i-3);
    }
*/
    if(ui->chkTimeSelSelect->isChecked()) filter.uValid |= FIELD__TIME_BEGIN | FIELD__TIME_END;
    filter.timeBeg = ui->dateTimeEditSelBegSelect->dateTime();
    filter.timeEnd = ui->dateTimeEditSelEndSelect->dateTime();

    if(ui->chkTypeSelSelect->isChecked()) filter.uValid |= FIELD__DATA_TYPE;
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

void MainWindow::on_toolDelFile_clicked()
{
    ui->toolDelFile->setEnabled(false);
    emit FileDelSignal(ReadLimitSelect());
    Mysleep(1000);
    ui->toolDelFile->setEnabled(m_bConnected && m_bLogined);
}

void MainWindow::on_PbInqDir_clicked()
{
    ui->tableFile->clearContents();
    ui->tableFile->setRowCount(0);
    ui->treeFile->clear();
    m_listFile.clear();

    ui->PbInqDir->setEnabled(false);
    emit DirSignal(ReadLimitSelect());
    Mysleep(3000);
    m_mutexFile.lock();
    m_mutexFile.unlock();
    ui->PbInqDir->setEnabled(m_bConnected && m_bLogined);
}

void MainWindow::WorkStatusSlot(QByteArray data)
{
    cmi_msgr_heartbeat_t* hb = (cmi_msgr_heartbeat_t*)data.data();
    BIZ_TYPE biztype = hb->sys_biztype;

    static bool bFormatting = false;
    QString sz;
    if (biztype & BIZTYPE__REC)
    {
        sz += tr("记录 ");
        //ui->PbRec->setEnabled(false);
        ui->PbRecStop->setEnabled(true);
        uiChgBizEnable(false);
    }
    else
    {
        ui->PbRecStop->setEnabled(false);
    }

    if (biztype & BIZTYPE__PLAY)
    {
        sz += tr("回放 ");
        //ui->PbReplay->setEnabled(false);
        ui->PbReplayPause->setEnabled(true);
        ui->PbReplayStop->setEnabled(true);
        uiChgBizEnable(false);
    }
    else
    {
        ui->PbReplayPause->setEnabled(false);
        ui->PbReplayStop->setEnabled(false);
    }

    if (biztype & BIZTYPE__FORMAT)
    {
        sz += tr("格式化 ");
        uiChgToolEnable(false);
        ui->PbDisConnect->setEnabled(false);
        bFormatting = true;
    }
    if (sz.isEmpty())
    {
        sz = tr("空闲");
        FunInIdleEnable();
        uiChgBizEnable(true);
        if(bFormatting)
        {
            bFormatting = false;
            uiChgToolEnable();
        }
    }

    QString strWorkStatus = tr("工作状态: ") + sz;
    m_labelStatWork->setText(strWorkStatus);
}

void MainWindow::on_PbBitSmall_clicked()
{
    on_PbBit_clicked();
}

void MainWindow::on_pbChannel_clicked()
{
    m_listStrChannel.clear();
    for(int i=0; i<ui->tableChannel->columnCount(); i++)
    {
        m_listStrChannel << ui->tableChannel->item(1, i)->text();
    }

    UpdateChannelName();
}

void MainWindow::on_pbChannelDefault_clicked()
{
    m_listStrChannel = SetDefaultChannelName();

    for(int i=0; i<ui->tableChannel->columnCount(); i++)
    {
        QTableWidgetItem *item1 = new QTableWidgetItem(m_listStrChannel.at(i));
        ui->tableChannel->setItem(1, i, item1);
    }

    UpdateChannelName();
}

void MainWindow::UpdateChannelName()
{
    m_infoStore.m_listStrChannel = m_listStrChannel;

    for(int i=0; i<m_listStrChannel.count(); i++)
    {
        QString t = m_listStrChannel.at(i);
        m_listStrLink.replace(i, t);
        m_listStrBit.replace(i, t);
    }

    for(int i=0; i<m_listChkRec.count(); i++)
    {
        m_listChkRec.at(i)->setToolTip(m_listStrChannel.at(i));
    }
    for(int i=0; i<m_listChkReplay.count(); i++)
    {
        m_listChkReplay.at(i)->setToolTip(m_listStrChannel.at(i));
    }
    for(int i=0; i<m_listChkDownload.count(); i++)
    {
        m_listChkDownload.at(i)->setToolTip(m_listStrChannel.at(i));
    }
    for(int i=0; i<m_listChkSelect.count(); i++)
    {
        m_listChkSelect.at(i)->setToolTip(m_listStrChannel.at(i));
    }
}


void MainWindow::on_toolUserQuit_2_clicked()
{
    this->close();
    QApplication::quit();
}

void MainWindow::on_PbPowerDown_clicked()
{
    ui->PbPowerDown->setEnabled(false);
    emit PowerDownSignal();
    m_bLogined = false;
    uiChgToolEnable(false);
    Mysleep(5000);
    m_bManual = false;
    m_tcpClient->disconnectFromHost();
    emit Uimsg(tr("网络已断开"));
}

QVariant MainWindow::ReadLimitReplay()
{
    T_FILTER filter;
    filter.uFileFlags = 0;
    filter.uValid = 0;
    if(ui->chkChnSelReplay->isChecked()) filter.uValid |= FIELD__CHAN;
    filter.uChannel = 0;
    for(int i=0; i<2; i++)
    {
        if(m_listChkReplay.at(i)->isChecked())
            filter.uChannel |= 1<<(i);
    }

    if (ui->chkChanSelReplay->isChecked()) {
        filter.uFileFlags = filter.uChannel;
        filter.uChannel = 0;
        filter.uChannel |= 1<<(ui->outcom->currentIndex());
        emit Uimsg(tr("current index: ") +QString::number(filter.uFileFlags, 10));
    }

/*
    for(int i=3; i<5; i++)
    {
        if(m_listChkSelect.at(i)->isChecked())
            filter.uChannel |= 1<<(i-3);
    }
*/
    if(ui->chkTimeSelReplay->isChecked()) filter.uValid |= FIELD__TIME_BEGIN | FIELD__TIME_END;
    filter.timeBeg = ui->dateTimeEditSelBegReplay->dateTime();
    filter.timeEnd = ui->dateTimeEditSelEndReplay->dateTime();

    if(ui->chkTypeSelReplay->isChecked()) filter.uValid |= FIELD__DATA_TYPE;
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

void MainWindow::on_PbReplay_clicked()
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
        //ui->PbReplay->setChecked(false);
        emit Uimsg(tr("错误：回放时必须打开类型约束，且只可选择一种数据类型。"));
    }
    else
    {
        //ui->PbReplay->setChecked(true);
        ui->PbReplayPause->setChecked(false);
        //ui->PbReplayStop->setChecked(false);

        //ui->PbReplay->setEnabled(false);
        ui->PbReplayPause->setEnabled(true);
        ui->PbReplayStop->setEnabled(true);

        //EnableReplaySel(false);

        //先发一个回放继续指令
        emit PlaybackPauseSignal(0);

        emit PlaybackStartSignal(pb);
    }
}

void MainWindow::on_PbReplayPause_clicked()
{
    UINT32 uChannel = 0;
    if(ui->PbReplayPause->isChecked())
    {   //暂停
        for(int i=0; i<m_listChkReplay.count(); i++)
        {
            if(m_listChkReplay.at(i)->isChecked())
                uChannel |= 1<<i;
        }
        //ui->PbReplay->setChecked(false);
        ui->PbReplayPause->setChecked(true);
        ui->PbReplayPause->setText("回放继续");
        //ui->PbReplayStop->setChecked(false);
    }
    else
    {   //继续
        for(int i=0; i<m_listChkReplay.count(); i++)
        {
            if(m_listChkReplay.at(i)->isChecked())
                uChannel &= ~(1<<i);
        }
        //ui->PbReplay->setChecked(true);
        ui->PbReplayPause->setChecked(false);
        ui->PbReplayPause->setText("回放暂停");
        //ui->PbReplayStop->setChecked(false);
    }

    //ui->PbReplay->setEnabled(false);
    ui->PbReplayPause->setEnabled(true);
    ui->PbReplayStop->setEnabled(true);

    emit PlaybackPauseSignal(uChannel);
}

void MainWindow::on_PbReplayStop_clicked()
{
    //ui->PbReplay->setChecked(false);
    ui->PbReplayPause->setChecked(false);
    //ui->PbReplayStop->setChecked(true);

    //ui->PbReplay->setEnabled(true);
    ui->PbReplayPause->setEnabled(false);
    ui->PbReplayStop->setEnabled(false);
    ui->PbReplayPause->setText("回放暂停");
    //EnableReplaySel(true);

    //先发一个回放继续指令
    emit PlaybackPauseSignal(0);
    //发回放停止指令
    emit PlaybackStopSignal();
}

void MainWindow::on_chkChnSelReplay_clicked()
{
    bool bChecked = ui->chkChnSelReplay->isChecked();
    for(int i=0; i<m_listChkReplay.count(); i++)
    {
        m_listChkReplay.at(i)->setEnabled(bChecked);
    }
}

void MainWindow::on_chkTimeSelReplay_clicked()
{
    bool bChecked = ui->chkTimeSelReplay->isChecked();
    ui->dateTimeEditSelBegReplay->setEnabled(bChecked);
    ui->dateTimeEditSelEndReplay->setEnabled(bChecked);
}

void MainWindow::on_chkChanSelReplay_clicked()
{
    bool bChecked = ui->chkChanSelReplay->isChecked();
    ui->outcom->setEnabled(bChecked);
}

void MainWindow::on_SetFilesz_clicked()
{
    UINT32 filesz = ui->filesz->text().toInt(0, 10);

    if (ui->szlogo->currentText() == "GB") {
        filesz = filesz * 1024;
    }

    emit SetfileszSignal(filesz);
}
#endif
