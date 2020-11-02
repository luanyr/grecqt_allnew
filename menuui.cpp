#include "menuui.h"
#include "ui_menuui.h"

MenuUI::MenuUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MenuUI)
{
    ui->setupUi(this);
    this->resize(1000, 150);
    ft = new QFont();
    ft->setPixelSize(12);
}

MenuUI::~MenuUI()
{
    delete ui;
}

startmenu_init::startmenu_init()
{
    LAipaddr = new QLabel(this);
    LAipaddr->setText(tr("IP地址:"));
    LAipaddr->setFont(*ft);
    LAipaddr->move(20, 40);

    LAport = new QLabel(this);
    LAport->setText(tr("端口:"));
    LAport->setFont(*ft);
    LAport->move(20, 70);

    LEipaddr = new QLineEdit(this);
    LEipaddr->resize(100, 20);
    LEipaddr->move(90, 40);

    LEport = new QLineEdit(this);
    LEport->resize(100, 20);
    LEport->move(90, 70);

    QIcon TBconnecticon;
    TBconnecticon.addFile(tr(":/png/png/connected-128.png"));
    TBconnect = new QToolButton(this);
    TBconnect->setIcon(TBconnecticon);
    TBconnect->setIconSize(QSize(50, 50));
    TBconnect->setStyleSheet("background-color:rgba(0,0,0,0)");
    TBconnect->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    TBconnect->setText(tr("连接"));
    TBconnect->resize(80, 80);
    TBconnect->move(190, 20);

    QIcon TBdisconnecticon;
    TBdisconnecticon.addFile(tr(":/png/png/disconnected-128.png"));
    TBdisconnect = new QToolButton(this);
    TBdisconnect->setIcon(TBdisconnecticon);
    TBdisconnect->setIconSize(QSize(50, 50));
    TBdisconnect->setStyleSheet("background-color:rgba(0,0,0,0)");
    TBdisconnect->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    TBdisconnect->setText(tr("断开"));
    TBdisconnect->resize(80, 80);
    TBdisconnect->move(240, 20);

    LAusrname = new QLabel(this);
    LAusrname->setText(tr("用户名:"));
    LAusrname->setFont(*ft);
    LAusrname->move(320, 40);

    LApswd = new QLabel(this);
    LApswd->setText(tr("密码："));
    LApswd->setFont(*ft);
    LApswd->move(320, 70);

    LEusrname = new QLineEdit(this);
    LEusrname->resize(100, 20);
    LEusrname->move(390, 40);

    LEpswd = new QLineEdit(this);
    LEpswd->resize(100, 20);
    LEpswd->move(390, 70);

    QIcon TBloginicon;
    TBloginicon.addFile(tr(":/png/png/user.png"));
    TBlogin = new QToolButton(this);
    TBlogin->setIcon(TBloginicon);
    TBlogin->setIconSize(QSize(50, 50));
    TBlogin->setStyleSheet("background-color:rgba(0,0,0,0)");
    TBlogin->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    TBlogin->setText(tr("用户登录"));
    TBlogin->resize(80, 80);
    TBlogin->move(500, 20);

    QIcon TBlogouticon;
    TBlogouticon.addFile(tr(":/png/png/talk-128.png"));
    TBlogout = new QToolButton(this);
    TBlogout->setIcon(TBlogouticon);
    TBlogout->setIconSize(QSize(50, 50));
    TBlogout->setStyleSheet("background-color:rgba(0,0,0,0)");
    TBlogout->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    TBlogout->setText(tr("退出登录"));
    TBlogout->resize(80, 80);
    TBlogout->move(580, 20);

    QIcon TBcloseappicon;
    TBcloseappicon.addFile(tr(":/png/png/cross-128.png"));
    TBcloseapp = new QToolButton(this);
    TBcloseapp->setIcon(TBcloseappicon);
    TBcloseapp->setIconSize(QSize(50, 50));
    TBcloseapp->setStyleSheet("background-color:rgba(0,0,0,0)");
    TBcloseapp->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    TBcloseapp->setText(tr("关闭程序"));
    TBcloseapp->resize(80, 80);
    TBcloseapp->move(660, 20);
    connect(this->TBconnect, SIGNAL(clicked(bool)), this, SLOT(slot_emitconnnect()));
    connect(this->TBdisconnect, SIGNAL(clicked(bool)), this, SLOT(slot_emitdisconnect()));
    connect(this->TBlogin, SIGNAL(clicked(bool)), this, SLOT(slot_emituserlogin()));
    connect(this->TBlogout, SIGNAL(clicked(bool)), this, SLOT(slot_emituserlogout()));
    connect(this->TBcloseapp, SIGNAL(clicked(bool)), this, SLOT(slot_emitturnoffapp()));
}

startmenu_init::~startmenu_init()
{

}

QString startmenu_init::Getipaddr()
{
    return this->LEipaddr->text();
}

QString startmenu_init::Getipport()
{
    return this->LEport->text();
}

QToolButton* startmenu_init::SetTbConnect()
{
    return this->TBconnect;
}

QToolButton* startmenu_init::SetTbDisconnect()
{
    return this->TBdisconnect;
}

QToolButton* startmenu_init::SetTblogin()
{
    return this->TBlogin;
}

QToolButton* startmenu_init::SetTblogout()
{
    return this->TBlogout;
}

void startmenu_init::slot_emitconnnect()
{
    emit SIGNAL_Pbtype(PBconnect);
}

void startmenu_init::slot_emitdisconnect()
{
    emit SIGNAL_Pbtype(PBdisconnect);
}

void startmenu_init::slot_emituserlogin()
{
    emit SIGNAL_Pbtype(PBuserlogin);
}

void startmenu_init::slot_emituserlogout()
{
    emit SIGNAL_Pbtype(PBuserlogout);
}

void startmenu_init::slot_emitturnoffapp()
{
    emit SIGNAL_Pbtype(PBturnoffapp);
}

QString startmenu_init::Getusrname()
{
    return this->LEusrname->text();
}

QString startmenu_init::Getusrpswd()
{
    return this->LEpswd->text();
}

QLineEdit* startmenu_init::GetAddrEdit()
{
    return this->LEipaddr;
}

QLineEdit* startmenu_init::GetPortEdit()
{
    return this->LEport;
}

QLineEdit* startmenu_init::GetUserNameEdit()
{
    return this->LEusrname;
}

QLineEdit* startmenu_init::GetUserpswdEdit()
{
    return this->LEpswd;
}

recordmenu_init::recordmenu_init()
{

    LEfilesz = new QLineEdit(this);
    LEfilesz->resize(90, 30);
    LEfilesz->move(20, 50);
    LEfilesz->setEnabled(false);

    CBfilesz = new QComboBox(this);
    CBfilesz->addItem(tr("MB"));
    CBfilesz->addItem(tr("GB"));
    CBfilesz->resize(60, 30);
    CBfilesz->move(120, 50);
    QIcon TBfileszIcon;
    TBfileszIcon.addFile(tr(":/png/png/geo_fence-128.png"));
    TBfilesz = new QToolButton(this);
    TBfilesz->setFont(*ft);
    TBfilesz->setIcon(TBfileszIcon);
    TBfilesz->setIconSize(QSize(50, 50));
    TBfilesz->setStyleSheet("background-color:rgba(0,0,0,0)");
    TBfilesz->setText(tr("设置文件大小"));
    TBfilesz->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    TBfilesz->resize(100, 100);
    TBfilesz->move(200, 30);

    QIcon TBchkRec_AIcon;
    TBchkRec_AIcon.addFile(tr(":/png/png/cross-128.png"));
    TBchkRec_A = new QPushButton(this);
    TBchkRec_A->setCheckable(true);
    //TBchkRec_A->setChecked(true);
    TBchkRec_A->setFont(*ft);
    TBchkRec_A->setIcon(TBchkRec_AIcon);
    //TBchkRec_A->setIconSize(QSize(50, 50));
    //TBchkRec_A->setStyleSheet("background-color:rgba(0,0,0,0)");
    TBchkRec_A->setText(tr("SRIO0"));
    //TBchkRec_A->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    TBchkRec_A->resize(80, 30);
    TBchkRec_A->move(320, 60);

    QIcon TBchkRec_BIcon;
    TBchkRec_BIcon.addFile(tr(":/png/png/cross-128.png"));
    TBchkRec_B = new QPushButton(this);
    TBchkRec_B->setCheckable(true);
    //TBchkRec_B->setChecked(true);
    TBchkRec_B->setFont(*ft);
    TBchkRec_B->setIcon(TBchkRec_AIcon);
    //TBchkRec_B->setIconSize(QSize(50, 50));
    //TBchkRec_B->setStyleSheet("background-color:rgba(0,0,0,0)");
    TBchkRec_B->setText(tr("SRIO1"));
    //TBchkRec_B->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    TBchkRec_B->resize(80, 30);
    TBchkRec_B->move(400, 60);

    QIcon TBRecStartIcon;
    TBRecStartIcon.addFile(tr(":/png/png/play-128.png"));
    TBRecStart = new QToolButton(this);
    TBRecStart->setFont(*ft);
    TBRecStart->setIcon(TBRecStartIcon);
    TBRecStart->setIconSize(QSize(50, 50));
    TBRecStart->setStyleSheet("background-color:rgba(0,0,0,0)");
    TBRecStart->setText(tr("开始记录"));
    TBRecStart->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    TBRecStart->resize(100, 100);
    TBRecStart->move(480, 30);

    QIcon TBRecStopIcon;
    TBRecStopIcon.addFile(tr(":/png/png/stop-128.png"));
    TBRecStop = new QToolButton(this);
    TBRecStop->setFont(*ft);
    TBRecStop->setIcon(TBRecStopIcon);
    TBRecStop->setIconSize(QSize(50, 50));
    TBRecStop->setStyleSheet("background-color:rgba(0,0,0,0)");
    TBRecStop->setText(tr("停止记录"));
    TBRecStop->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    TBRecStop->resize(100, 100);
    TBRecStop->move(560, 30);
    connect(this->TBfilesz,  SIGNAL(clicked(bool)), this,  SLOT(slot_emitSetFileSz()));
    connect(this->TBchkRec_A,SIGNAL(clicked(bool)), this,  SLOT(slot_emitchkRec_A()));
    connect(this->TBchkRec_B,SIGNAL(clicked(bool)), this,  SLOT(slot_emitchkRec_B()));
    connect(this->TBRecStart,SIGNAL(clicked(bool)), this,  SLOT(slot_emitRecStart()));
    connect(this->TBRecStop, SIGNAL(clicked(bool)), this,  SLOT(slot_emitRecStop()));
}

recordmenu_init::~recordmenu_init()
{

}

QToolButton *recordmenu_init::SetRecStart()
{
    return this->TBRecStart;
}

QToolButton *recordmenu_init::SetRecStop()
{
    return this->TBRecStop;
}

QToolButton *recordmenu_init::SetRecFilesz()
{
    return this->TBfilesz;
}

QPushButton *recordmenu_init::SetchkRec_A()
{
    return this->TBchkRec_A;
}

QPushButton *recordmenu_init::SetchkRec_B()
{
    return this->TBchkRec_B;
}

void recordmenu_init::slot_emitSetFileSz()
{
    emit SIGNAL_RcdmType(rcdmsignal_setfilesz);
}

void recordmenu_init::slot_emitchkRec_A()
{
    emit SIGNAL_RcdmType(rcdmsignal_chkRec_A);
}

void recordmenu_init::slot_emitchkRec_B()
{
    emit SIGNAL_RcdmType(rcdmsignal_chkRec_B);
}

void recordmenu_init::slot_emitRecStart()
{
    emit SIGNAL_RcdmType(rcdmsignal_recstart);
}

void recordmenu_init::slot_emitRecStop()
{
    emit SIGNAL_RcdmType(rcdmsignal_recstop);
}

UINT32 recordmenu_init::GetFilesz()
{
    return this->LEfilesz->text().toInt(0, 10);
}

QString recordmenu_init::GetFileszUint()
{
    return this->CBfilesz->currentText();
}

replaymenu_init::replaymenu_init()
{
    QIcon PBchkchnselreplayicon;
    PBchkchnselreplayicon.addFile(":/png/png/cross-128.png");
    PBchkchnselreplay = new QPushButton(this);
    PBchkchnselreplay->setCheckable(true);
    PBchkchnselreplay->setChecked(true);
    PBchkchnselreplay->resize(100, 40);
    PBchkchnselreplay->setFont(*ft);
    PBchkchnselreplay->setText(tr("通道约束"));
    //PBchkchnselreplay->setStyleSheet("background-color:rgba(0,0,0,0)");
    PBchkchnselreplay->setIcon(PBchkchnselreplayicon);

    //PBchkchnselreplay->setIconSize(QSize(30, 30));
    PBchkchnselreplay->move(20,0);

    QIcon TBchkReplayAicon;
    TBchkReplayAicon.addFile(":/png/png/cross-128.png");
    TBchkReplayA = new QPushButton(this);
    TBchkReplayA->setCheckable(true);
    TBchkReplayA->setChecked(true);
    TBchkReplayA->resize(80, 20);
    TBchkReplayA->setFont(*ft);
    TBchkReplayA->setText(tr("SRIO0"));
//  TBchkReplayA->setStyleSheet("background-color:rgba(0,0,0,0)");
    TBchkReplayA->setIcon(TBchkReplayAicon);
//    TBchkReplayA->setIconSize(QSize(50, 50));
//    TBchkReplayA->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    TBchkReplayA->move(30, 40);

    QIcon TBchkReplayBicon;
    TBchkReplayBicon.addFile(":/png/png/cross-128.png");
    TBchkReplayB = new QPushButton(this);
    TBchkReplayB->setCheckable(true);
    TBchkReplayB->setChecked(true);
    TBchkReplayB->resize(80, 20);
    TBchkReplayB->setFont(*ft);
    TBchkReplayB->setText(tr("SRIO1"));
//    TBchkReplayB->setStyleSheet("background-color:rgba(0,0,0,0)");
    TBchkReplayB->setIcon(TBchkReplayBicon);
//    TBchkReplayB->setIconSize(QSize(50, 50));
//   TBchkReplayB->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    TBchkReplayB->move(30, 80);

    QIcon PBcjkTimeSelReplayicon;
    PBcjkTimeSelReplayicon.addFile(":/png/png/cross-128.png");
    PBcjkTimeSelReplay = new QPushButton(this);
    PBcjkTimeSelReplay->setCheckable(true);
    PBcjkTimeSelReplay->setChecked(true);
    PBcjkTimeSelReplay->resize(100, 40);
    PBcjkTimeSelReplay->setFont(*ft);
    PBcjkTimeSelReplay->setText(tr("时间约束"));
    //PBchkchnselreplay->setStyleSheet("background-color:rgba(0,0,0,0)");
    PBcjkTimeSelReplay->setIcon(PBchkchnselreplayicon);
    PBcjkTimeSelReplay->setIconSize(QSize(30, 30));
    PBcjkTimeSelReplay->move(195, 0);

    LAstarttime = new QLabel(this);
    LAstarttime->setFont(*ft);
    LAstarttime->setText(tr("起始时间"));
    LAstarttime->move(150, 55);

    LAendtime = new QLabel(this);
    LAendtime->setFont(*ft);
    LAendtime->setText(tr("结束时间"));
    LAendtime->move(150, 95);

    DTEstarttime = new QDateTimeEdit(this);
    DTEstarttime->setDateTime(QDateTime::currentDateTime());
    DTEstarttime->resize(120, 30);
    DTEstarttime->move(220, 45);

    DTEendtime = new QDateTimeEdit(this);
    DTEendtime->setDateTime(QDateTime::currentDateTime());
    DTEendtime->resize(120, 30);
    DTEendtime->move(220, 85);

    QIcon PBchktypeSelReplayicon;
    PBchktypeSelReplayicon.addFile(":/png/png/cross-128.png");
    PBchktypeSelReplay = new QPushButton(this);
    PBchktypeSelReplay->setCheckable(true);
    PBchktypeSelReplay->setChecked(true);
    PBchktypeSelReplay->resize(115, 40);
    PBchktypeSelReplay->setFont(*ft);
    PBchktypeSelReplay->setText(tr("类型约束"));
    //PBchkchnselreplay->setStyleSheet("background-color:rgba(0,0,0,0)");
    PBchktypeSelReplay->setIcon(PBchkchnselreplayicon);
    PBchktypeSelReplay->setIconSize(QSize(30, 30));
    PBchktypeSelReplay->move(380, 0);

    QIcon type1icon;
    type1icon.addFile(":/png/png/cross-128.png");
    PBtype1 = new QPushButton(this);
    PBtype1->setCheckable(true);
    PBtype1->setChecked(true);
    PBtype1->resize(55, 30);
    PBtype1->setText(tr("类型1"));
    PBtype1->setIcon(type1icon);
//    PBtype1->setIconSize(QSize(15, 15));
    PBtype1->move(380, 45);

    QIcon type2icon;
    type2icon.addFile(":/png/png/cross-128.png");
    PBtype2 = new QPushButton(this);
    PBtype2->setCheckable(true);
    PBtype2->setChecked(true);
    PBtype2->resize(55, 30);
    PBtype2->setText(tr("类型2"));
    PBtype2->setIcon(type2icon);
//    PBtype2->setIconSize(QSize(15, 15));
    PBtype2->move(440, 45);

    QIcon type3icon;
    type3icon.addFile(":/png/png/cross-128.png");
    PBtype3 = new QPushButton(this);
    PBtype3->setCheckable(true);
    PBtype3->setChecked(true);
    PBtype3->resize(55, 30);
    PBtype3->setText(tr("类型3"));
    PBtype3->setIcon(type3icon);
//    PBtype3->setIconSize(QSize(15, 15));
    PBtype3->move(380, 90);

    QIcon type4icon;
    type4icon.addFile(":/png/png/cross-128.png");
    PBtype4 = new QPushButton(this);
    PBtype4->setCheckable(true);
    PBtype4->setChecked(true);
    PBtype4->resize(55, 30);
    PBtype4->setText(tr("类型4"));
    PBtype4->setIcon(type4icon);
//    PBtype4->setIconSize(QSize(15, 15));
    PBtype4->move(440, 90);

    QIcon PBchkChanSelReplayicon;
    PBchkChanSelReplayicon.addFile(":/png/png/cross-128.png");
    PBchkChanSelReplay = new QPushButton(this);
    PBchkChanSelReplay->setCheckable(true);
    PBchkChanSelReplay->setChecked(true);
    PBchkChanSelReplay->resize(100, 40);
    PBchkChanSelReplay->setFont(*ft);
    PBchkChanSelReplay->setText(tr("输出端口"));
    PBchkChanSelReplay->setIcon(PBchkChanSelReplayicon);
//    PBchkChanSelReplay->setIconSize(QSize(30, 30));
    PBchkChanSelReplay->move(550, 0);

    QStringList cbxoutcom_list;
    cbxoutcom_list << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9"\
                   <<"10" << "11" << "12" << "13" << "14";
    CBXoutcom = new QComboBox(this);
    CBXoutcom->addItems(cbxoutcom_list);
    CBXoutcom->setFont(*ft);
    CBXoutcom->resize(100, 40);
    CBXoutcom->move(550, 60);

    QIcon TBreplayicon;
    TBreplayicon.addFile(":/png/png/play-128.png");
    TBreplay = new QToolButton(this);
    TBreplay->resize(80, 80);
    TBreplay->setFont(*ft);
    TBreplay->setText(tr("回放开始"));
    TBreplay->setStyleSheet("background-color:rgba(0,0,0,0)");
    TBreplay->setIcon(TBreplayicon);
    TBreplay->setIconSize(QSize(50, 50));
    TBreplay->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    TBreplay->move(700, 40);

    QIcon TBpauseicon;
    TBpauseicon.addFile(":/png/png/pause-128.png");
    TBpause = new QToolButton(this);
    TBpause->setCheckable(true);
    TBpause->resize(80, 80);
    TBpause->setFont(*ft);
    TBpause->setText(tr("回放暂停"));
    TBpause->setStyleSheet("background-color:rgba(0,0,0,0)");
    TBpause->setIcon(TBpauseicon);
    TBpause->setIconSize(QSize(50, 50));
    TBpause->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    TBpause->move(800, 40);

    QIcon TBstopicon;
    TBstopicon.addFile(":/png/png/stop-128.png");
    TBstop = new QToolButton(this);
    TBstop->resize(80, 80);
    TBstop->setFont(*ft);
    TBstop->setText(tr("回放停止"));
    TBstop->setStyleSheet("background-color:rgba(0,0,0,0)");
    TBstop->setIcon(TBstopicon);
    TBstop->setIconSize(QSize(50, 50));
    TBstop->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    TBstop->move(900, 40);
    connect(this->PBchkchnselreplay, SIGNAL(clicked(bool)), this, SLOT(slot_emitchkchnselreplay()));
    connect(this->TBchkReplayA, SIGNAL(clicked(bool)), this, SLOT(slot_emitchkReplayA()));
    connect(this->TBchkReplayB, SIGNAL(clicked(bool)), this, SLOT(slot_emitchkReplayB()));
    connect(this->PBcjkTimeSelReplay, SIGNAL(clicked(bool)), this, SLOT(slot_emitcjkTimeSelReplay()));
    connect(this->PBchktypeSelReplay, SIGNAL(clicked(bool)), this, SLOT(slot_emitchktypeSelReplay()));
    connect(this->PBtype1, SIGNAL(clicked(bool)), this, SLOT(slot_emittype1()));
    connect(this->PBtype2, SIGNAL(clicked(bool)), this, SLOT(slot_emittype2()));
    connect(this->PBtype3, SIGNAL(clicked(bool)), this, SLOT(slot_emittype3()));
    connect(this->PBtype4, SIGNAL(clicked(bool)), this, SLOT(slot_emittype4()));
    connect(this->PBchkChanSelReplay, SIGNAL(clicked(bool)), this, SLOT(slot_emitchkChanSelReplay()));
    connect(this->TBreplay, SIGNAL(clicked(bool)), this, SLOT(slot_emitreplay()));
    connect(this->TBpause, SIGNAL(clicked(bool)), this, SLOT(slot_emitpause()));
    connect(this->TBstop, SIGNAL(clicked(bool)), this, SLOT(slot_emitstop()));
}

replaymenu_init::~replaymenu_init()
{

}

void replaymenu_init::slot_emitchkchnselreplay()
{
    emit SIGNAL_RpmType(rpmsignal_chkchnselreplay);
}

void replaymenu_init::slot_emitchkChanSelReplay()
{
    emit SIGNAL_RpmType(rpmsignal_chkChanSelReplay);
}

void replaymenu_init::slot_emitchkReplayA()
{
    emit SIGNAL_RpmType(rpmsignal_chkReplayA);
}

void replaymenu_init::slot_emitchkReplayB()
{
    emit SIGNAL_RpmType(rpmsignal_chkReplayB);
}

void replaymenu_init::slot_emitchktypeSelReplay()
{
    emit SIGNAL_RpmType(rpmsignal_chktypeSelReplay);
}

void replaymenu_init::slot_emitcjkTimeSelReplay()
{
    emit SIGNAL_RpmType(rpmsignal_cjkTimeSelReplay);
}

void replaymenu_init::slot_emitpause()
{
    emit SIGNAL_RpmType(rpmsignal_pause);
}

void replaymenu_init::slot_emitreplay()
{
    emit SIGNAL_RpmType(rpmsignal_replay);
}

void replaymenu_init::slot_emitstop()
{
    emit SIGNAL_RpmType(rpmsignal_stop);
}

void replaymenu_init::slot_emittype1()
{
    emit SIGNAL_RpmType(rpmsignal_type1);
}

void replaymenu_init::slot_emittype2()
{
    emit SIGNAL_RpmType(rpmsignal_type2);
}

void replaymenu_init::slot_emittype3()
{
    emit SIGNAL_RpmType(rpmsignal_type3);
}

void replaymenu_init::slot_emittype4()
{
    emit SIGNAL_RpmType(rpmsignal_type4);
}


QPushButton *replaymenu_init::SetchkReplayA()
{
    return this->TBchkReplayA;
}

QPushButton *replaymenu_init::SetchkReplayB()
{
    return this->TBchkReplayB;
}

QPushButton *replaymenu_init::SetType1()
{
    return this->PBtype1;
}

QPushButton *replaymenu_init::SetType2()
{
    return this->PBtype2;
}

QPushButton *replaymenu_init::SetType3()
{
    return this->PBtype3;
}

QPushButton *replaymenu_init::SetType4()
{
    return this->PBtype4;
}

QPushButton *replaymenu_init::SetchkChnSelReplay()
{
    return this->PBchkchnselreplay;
}

QPushButton *replaymenu_init::SetchkTimeSelReplay()
{
    return this->PBcjkTimeSelReplay;
}

QPushButton *replaymenu_init::SetchkChanSelReplay()
{
    return this->PBchkChanSelReplay;
}

QPushButton *replaymenu_init::SetchkTypeSelReplay()
{
    return this->PBchktypeSelReplay;
}

QComboBox *replaymenu_init::SetOutCom()
{
    return this->CBXoutcom;
}

QDateTimeEdit *replaymenu_init::SetStartTime()
{
    return this->DTEstarttime;
}

QDateTimeEdit *replaymenu_init::SetEndTime()
{
    return this->DTEendtime;
}

QToolButton* replaymenu_init::SetTbReplay()
{
    return this->TBreplay;
}

QToolButton* replaymenu_init::SetTbPause()
{
    return this->TBpause;
}

QToolButton *replaymenu_init::SetTbStop()
{
    return this->TBstop;
}

selectmenu_init::selectmenu_init()
{
    QIcon PBchkchnselreplayicon;
    PBchkchnselreplayicon.addFile(":/png/png/cross-128.png");
    PBchkchnselreplay = new QPushButton(this);
    PBchkchnselreplay->setCheckable(true);
    PBchkchnselreplay->setChecked(true);
    PBchkchnselreplay->resize(100, 40);
    PBchkchnselreplay->setFont(*ft);
    PBchkchnselreplay->setText(tr("通道约束"));
    //PBchkchnselreplay->setStyleSheet("background-color:rgba(0,0,0,0)");
    PBchkchnselreplay->setIcon(PBchkchnselreplayicon);
    PBchkchnselreplay->setIconSize(QSize(30, 30));
    PBchkchnselreplay->move(20,0);

    QIcon TBchkReplayAicon;
    TBchkReplayAicon.addFile(":/png/png/cross-128.png");
    TBchkReplayA = new QPushButton(this);
    TBchkReplayA->setCheckable(true);
    TBchkReplayA->resize(80, 20);
    TBchkReplayA->setFont(*ft);
    TBchkReplayA->setText(tr("SRIO0"));
    //TBchkReplayA->setStyleSheet("background-color:rgba(0,0,0,0)");
    TBchkReplayA->setIcon(TBchkReplayAicon);
    //TBchkReplayA->setIconSize(QSize(50, 50));
    //TBchkReplayA->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    TBchkReplayA->move(30, 40);

    QIcon TBchkReplayBicon;
    TBchkReplayBicon.addFile(":/png/png/cross-128.png");
    TBchkReplayB = new QPushButton(this);
    TBchkReplayB->setCheckable(true);
    TBchkReplayB->resize(80, 20);
    TBchkReplayB->setFont(*ft);
    TBchkReplayB->setText(tr("SRIO1"));
    //TBchkReplayB->setStyleSheet("background-color:rgba(0,0,0,0)");
    TBchkReplayB->setIcon(TBchkReplayBicon);
    //TBchkReplayB->setIconSize(QSize(50, 50));
    //TBchkReplayB->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    TBchkReplayB->move(30, 80);

    QIcon PBcjkTimeSelReplayicon;
    PBcjkTimeSelReplayicon.addFile(":/png/png/cross-128.png");
    PBcjkTimeSelReplay = new QPushButton(this);
    PBcjkTimeSelReplay->setCheckable(true);
    PBcjkTimeSelReplay->setChecked(true);
    PBcjkTimeSelReplay->resize(100, 40);
    PBcjkTimeSelReplay->setFont(*ft);
    PBcjkTimeSelReplay->setText(tr("时间约束"));
    //PBchkchnselreplay->setStyleSheet("background-color:rgba(0,0,0,0)");
    PBcjkTimeSelReplay->setIcon(PBchkchnselreplayicon);
    PBcjkTimeSelReplay->setIconSize(QSize(30, 30));
    PBcjkTimeSelReplay->move(250, 0);

    LAstarttime = new QLabel(this);
    LAstarttime->setFont(*ft);
    LAstarttime->setText(tr("起始时间"));
    LAstarttime->move(200, 55);

    LAendtime = new QLabel(this);
    LAendtime->setFont(*ft);
    LAendtime->setText(tr("结束时间"));
    LAendtime->move(200, 95);

    DTEstarttime = new QDateTimeEdit(this);
    DTEstarttime->setDateTime(QDateTime::currentDateTime());
    DTEstarttime->resize(120, 30);
    DTEstarttime->move(275, 45);

    DTEendtime = new QDateTimeEdit(this);
    DTEendtime->setDateTime(QDateTime::currentDateTime());
    DTEendtime->resize(120, 30);
    DTEendtime->move(275, 85);

    QIcon PBchktypeSelReplayicon;
    PBchktypeSelReplayicon.addFile(":/icon/icon/ygg.png");
    PBchktypeSelReplay = new QPushButton(this);
    PBchktypeSelReplay->setCheckable(true);
    PBchktypeSelReplay->setChecked(true);
    PBchktypeSelReplay->resize(125, 40);
    PBchktypeSelReplay->setFont(*ft);
    PBchktypeSelReplay->setText(tr("类型约束"));
    //PBchkchnselreplay->setStyleSheet("background-color:rgba(0,0,0,0)");
    PBchktypeSelReplay->setIcon(PBchkchnselreplayicon);
    PBchktypeSelReplay->setIconSize(QSize(30, 30));
    PBchktypeSelReplay->move(490, 0);

    QIcon type1icon;
    type1icon.addFile(":/png/png/cross-128.png");
    PBtype1 = new QPushButton(this);
    PBtype1->setCheckable(true);
    PBtype1->resize(60, 30);
    PBtype1->setText(tr("类型1"));
    PBtype1->setIcon(type1icon);
    PBtype1->setIconSize(QSize(15, 15));
    PBtype1->move(490, 45);

    QIcon type2icon;
    type2icon.addFile(":/png/png/cross-128.png");
    PBtype2 = new QPushButton(this);
    PBtype2->setCheckable(true);
    PBtype2->resize(60, 30);
    PBtype2->setText(tr("类型2"));
    PBtype2->setIcon(type2icon);
    PBtype2->setIconSize(QSize(15, 15));
    PBtype2->move(555, 45);

    QIcon type3icon;
    type3icon.addFile(":/png/png/cross-128.png");
    PBtype3 = new QPushButton(this);
    PBtype3->setCheckable(true);
    PBtype3->resize(60, 30);
    PBtype3->setText(tr("类型3"));
    PBtype3->setIcon(type3icon);
    PBtype3->setIconSize(QSize(15, 15));
    PBtype3->move(490, 90);

    QIcon type4icon;
    type4icon.addFile(":/png/png/cross-128.png");
    PBtype4 = new QPushButton(this);
    PBtype4->setCheckable(true);
    PBtype4->resize(60, 30);
    PBtype4->setText(tr("类型4"));
    PBtype4->setIcon(type4icon);
    PBtype4->setIconSize(QSize(15, 15));
    PBtype4->move(555, 90);

    QIcon TBdelfileicon;
    TBdelfileicon.addFile(":/png/png/Trash Empty.png");
    TBdelfile = new QToolButton(this);
    TBdelfile->resize(100, 100);
    TBdelfile->setFont(*ft);
    TBdelfile->setText(tr("删除文件"));
    TBdelfile->setStyleSheet("background-color:rgba(0,0,0,0)");
    TBdelfile->setIcon(TBdelfileicon);
    TBdelfile->setIconSize(QSize(50, 50));
    TBdelfile->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    TBdelfile->move(680, 20);

    QIcon TBsearchfileicon;
    TBsearchfileicon.addFile(":/png/png/opened_folder-128.png");
    TBsearchfile = new QToolButton(this);
    TBsearchfile->resize(100, 100);
    TBsearchfile->setFont(*ft);
    TBsearchfile->setText(tr("查询目录"));
    TBsearchfile->setStyleSheet("background-color:rgba(0,0,0,0)");
    TBsearchfile->setIcon(TBsearchfileicon);
    TBsearchfile->setIconSize(QSize(50, 50));
    TBsearchfile->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    TBsearchfile->move(800, 20);
    connect(this->PBchkchnselreplay, SIGNAL(clicked(bool)), this, SLOT(slot_emitchkchnselect()));
    connect(this->TBchkReplayA, SIGNAL(clicked(bool)), this, SLOT(slot_emitchkSelectA()));
    connect(this->TBchkReplayB, SIGNAL(clicked(bool)), this, SLOT(slot_emitchkSelectB()));
    connect(this->PBcjkTimeSelReplay, SIGNAL(clicked(bool)), this, SLOT(slot_emitcjkTimeSelect()));
    connect(this->PBchktypeSelReplay, SIGNAL(clicked(bool)), this, SLOT(slot_emitchktypeSelect()));
    connect(this->PBtype1, SIGNAL(clicked(bool)), this, SLOT(slot_emittype1()));
    connect(this->PBtype2, SIGNAL(clicked(bool)), this, SLOT(slot_emittype2()));
    connect(this->PBtype3, SIGNAL(clicked(bool)), this, SLOT(slot_emittype3()));
    connect(this->PBtype4, SIGNAL(clicked(bool)), this, SLOT(slot_emittype4()));
    connect(this->TBdelfile, SIGNAL(clicked(bool)), this, SLOT(slot_emitDeleteFile()));
    connect(this->TBsearchfile, SIGNAL(clicked(bool)), this, SLOT(slot_emitInqDir()));
}

selectmenu_init::~selectmenu_init()
{


}

QPushButton* selectmenu_init::SetchkReplayA()
{
    return this->TBchkReplayA;
}

QPushButton* selectmenu_init::SetchkReplayB()
{
    return this->TBchkReplayB;
}

QPushButton* selectmenu_init::SetType1()
{
    return this->PBtype1;
}
QPushButton* selectmenu_init::SetType2()
{
    return this->PBtype2;
}
QPushButton* selectmenu_init::SetType3()
{
    return this->PBtype3;
}
QPushButton* selectmenu_init::SetType4()
{
    return this->PBtype4;
}

QPushButton* selectmenu_init::SetchkChnSelReplay()
{
    return this->PBchkchnselreplay;
}

QPushButton* selectmenu_init::SetchkTimeSelReplay()
{
    return this->PBcjkTimeSelReplay;
}

QPushButton* selectmenu_init::SetchkTypeSelReplay()
{
    return this->PBchktypeSelReplay;
}

QDateTimeEdit* selectmenu_init::SetStartTime()
{
    return this->DTEstarttime;
}

QDateTimeEdit* selectmenu_init::SetEndTime()
{
    return this->DTEendtime;
}

QToolButton* selectmenu_init::SetDelFile()
{
    return this->TBdelfile;
}

QToolButton* selectmenu_init::SetInqDir()
{
    return this->TBsearchfile;
}

void selectmenu_init::slot_emitchkchnselect()
{
    emit SIGNAL_SmType(smsignal_chkchnselect);
}

void selectmenu_init::slot_emitchkSelectA()
{
    emit SIGNAL_SmType(smsignal_chkSelectTypeA);
}

void selectmenu_init::slot_emitchkSelectB()
{
    emit SIGNAL_SmType(smsignal_chkSelectTypeB);
}

void selectmenu_init::slot_emitcjkTimeSelect()
{
    emit SIGNAL_SmType(smsignal_chkTimeSelect);
}

void selectmenu_init::slot_emitchktypeSelect()
{
    emit SIGNAL_SmType(smsignal_chkTypeSelect);
}

void selectmenu_init::slot_emittype1()
{
    emit SIGNAL_SmType(smsginal_type1);
}

void selectmenu_init::slot_emittype2()
{
    emit SIGNAL_SmType(smsginal_type2);
}

void selectmenu_init::slot_emittype3()
{
    emit SIGNAL_SmType(smsginal_type3);
}

void selectmenu_init::slot_emittype4()
{
    emit SIGNAL_SmType(smsginal_type4);
}

void selectmenu_init::slot_emitDeleteFile()
{
    emit SIGNAL_SmType(smsignal_DeleteFile);
}

void selectmenu_init::slot_emitInqDir()
{
    emit SIGNAL_SmType(smsignal_InqDir);
}


managemenu_init::managemenu_init()
{
    pTime = new QTimer();
    LAsettime = new QLabel(this);
    LAsettime->setFont(*ft);
    LAsettime->setText(tr("设定时间"));
    LAsettime->resize(80, 20);
    LAsettime->move(30,25);

    QIcon PBsyncicon;
    PBsyncicon.addFile(":/icon/icon/ygg.png");
    PBsync = new QPushButton(this);
    PBsync->setFont(*ft);
    PBsync->resize(60, 30);
    PBsync->setText(tr("同步"));
    PBsync->setIcon(PBsyncicon);
    PBsync->setIconSize(QSize(20, 20));
    PBsync->move(110, 20);

    DTEtime = new QDateTimeEdit(this);
    DTEtime->setDateTime(QDateTime::currentDateTime());
    DTEtime->resize(140, 30);
    DTEtime->move(30, 80);

    QIcon TBsendtimeicon;
    TBsendtimeicon.addFile(":/png/png/time.png");
    TBsendtime = new QToolButton(this);
    TBsendtime->resize(110, 110);
    TBsendtime->setFont(*ft);
    TBsendtime->setText(tr("授时"));
    TBsendtime->setStyleSheet("background-color:rgba(0,0,0,0)");
    TBsendtime->setIcon(TBsendtimeicon);
    TBsendtime->setIconSize(QSize(50, 50));
    TBsendtime->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    TBsendtime->move(250, 10);

    QIcon TBselfcheckicon;
    TBselfcheckicon.addFile(":/png/png/settings-128.png");
    TBselfcheck = new QToolButton(this);
    TBselfcheck->resize(110, 110);
    TBselfcheck->setFont(*ft);
    TBselfcheck->setText(tr("自检"));
    TBselfcheck->setStyleSheet("background-color:rgba(0,0,0,0)");
    TBselfcheck->setIcon(TBselfcheckicon);
    TBselfcheck->setIconSize(QSize(50, 50));
    TBselfcheck->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    TBselfcheck->move(350, 10);

    connect(this->pTime, SIGNAL(timeout()), this, SLOT(slot_timesyncfunc()));
    pTime->start(1000);
    connect(this->PBsync, SIGNAL(clicked(bool)), this, SLOT(slot_emitPBsync()));
    connect(this->TBsendtime, SIGNAL(clicked(bool)), this, SLOT(slot_emitTBsendtime()));
    connect(this->TBselfcheck, SIGNAL(clicked(bool)), this, SLOT(slot_emitTBselfcheck()));
}

void managemenu_init::slot_timesyncfunc()
{
    //if(this->PBsync->isChecked())
        DTEtime->setDateTime(QDateTime::currentDateTime());
}

managemenu_init::~managemenu_init()
{

}

QPushButton* managemenu_init::SetTimeSync()
{
    return this->PBsync;
}

QDateTimeEdit* managemenu_init::SetTimeEdit()
{
    return this->DTEtime;
}

QToolButton* managemenu_init::SetSendTime()
{
    return this->TBsendtime;
}

QToolButton* managemenu_init::SetSelfCheck()
{
    return this->TBselfcheck;
}

void managemenu_init::slot_emitPBsync()
{
    emit SIGNAL_MgmType(mgmsignal_timesync);
}

void managemenu_init::slot_emitTBsendtime()
{
    emit SIGNAL_MgmType(mgmsignal_sendtime);
}

void managemenu_init::slot_emitTBselfcheck()
{
    emit SIGNAL_MgmType(mgmsignal_selfcheck);
}

usermenu_init::usermenu_init()
{
    LAusername = new QLabel(this);
    LAusername->setFont(*ft);
    LAusername->setText(tr("用户名:"));
    LAusername->resize(80, 20);
    LAusername->move(20, 35);

    LAuserpswd = new QLabel(this);
    LAuserpswd->setFont(*ft);
    LAuserpswd->setText(tr("密码:"));
    LAuserpswd->resize(80, 20);
    LAuserpswd->move(20, 75);

    LEusername = new QLineEdit(this);
    LEusername->resize(100, 20);
    LEusername->move(80, 35);

    LEuserpwsd = new QLineEdit(this);
    LEuserpwsd->resize(100, 20);
    LEuserpwsd->move(80, 75);

    QIcon TBcreatuserIcon;
    TBcreatuserIcon.addFile(tr(":/png/png/newuser.png"));
    TBcreatuser = new QToolButton(this);
    TBcreatuser->setFont(*ft);
    TBcreatuser->setIcon(TBcreatuserIcon);
    TBcreatuser->setIconSize(QSize(60, 60));
    TBcreatuser->setStyleSheet("background-color:rgba(0,0,0,0)");
    TBcreatuser->setText(tr("新建用户"));
    TBcreatuser->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    TBcreatuser->resize(85, 85);
    TBcreatuser->move(220, 20);

    QIcon TBmodifypsdwIcon;
    TBmodifypsdwIcon.addFile(tr(":/png/png/pencil-128.png"));
    TBmodifypsdw = new QToolButton(this);
    TBmodifypsdw->setFont(*ft);
    TBmodifypsdw->setIcon(TBmodifypsdwIcon);
    TBmodifypsdw->setIconSize(QSize(60, 60));
    TBmodifypsdw->setStyleSheet("background-color:rgba(0,0,0,0)");
    TBmodifypsdw->setText(tr("修改密码"));
    TBmodifypsdw->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    TBmodifypsdw->resize(85, 85);
    TBmodifypsdw->move(320, 20);

    QIcon TBdeluserIcon;
    TBdeluserIcon.addFile(tr(":/png/png/deluser.png"));
    TBdeluser = new QToolButton(this);
    TBdeluser->setFont(*ft);
    TBdeluser->setIcon(TBdeluserIcon);
    TBdeluser->setIconSize(QSize(60, 60));
    TBdeluser->setStyleSheet("background-color:rgba(0,0,0,0)");
    TBdeluser->setText(tr("删除用户"));
    TBdeluser->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    TBdeluser->resize(85, 85);
    TBdeluser->move(420, 20);

    connect(this->TBcreatuser, SIGNAL(clicked(bool)), this, SLOT(slot_emitTBcreatusr()));
    connect(this->TBmodifypsdw, SIGNAL(clicked(bool)), this, SLOT(slot_emitTBmodifypswd()));
    connect(this->TBdeluser, SIGNAL(clicked(bool)), this, SLOT(slot_emitTBdelusr()));
}

usermenu_init::~usermenu_init()
{

}

QString usermenu_init::Getusrname()
{
    return this->LEusername->text();
}

QString usermenu_init::Getusrpswd()
{
    return this->LEuserpwsd->text();
}

QToolButton* usermenu_init::SetCreatUsr()
{
    return this->TBcreatuser;
}

QToolButton* usermenu_init::SetModifyPswd()
{
    return this->TBdeluser;
}

QToolButton* usermenu_init::SetDelUsr()
{
    return this->TBdeluser;
}

void usermenu_init::slot_emitTBcreatusr()
{
    emit SIGNAL_UmType(umsignal_creatusr);
}

void usermenu_init::slot_emitTBmodifypswd()
{
    emit SIGNAL_UmType(umsignal_modifypswd);
}

void usermenu_init::slot_emitTBdelusr()
{
    emit SIGNAL_UmType(umsignal_delusr);
}

channelmenu_init::channelmenu_init()
{
    LAmdftunnel = new QLabel(this);
    LAmdftunnel->setFont(*ft);
    LAmdftunnel->resize(300, 20);
    LAmdftunnel->setText(tr("修改通道名称（下次程序打开时生效）"));
    LAmdftunnel->move(10, 20);

    PBbacktodef = new QPushButton(this);
    PBbacktodef->setText(tr("恢复默认"));
    PBbacktodef->move(320, 20);

    PBconfirm = new QPushButton(this);
    PBconfirm->setText(tr("确定"));
    PBconfirm->move(880, 20);

    TWchannel = new QTableWidget(this);
    TWchannel->resize(950, 80);
    TWchannel->setStyleSheet("background-color:rgba(0,0,0,0)");
    TWchannel->move(10, 45);
    connect(this->PBbacktodef, SIGNAL(clicked(bool)), this, SLOT(cmhandle_setchanneldefault()));
    connect(this->PBconfirm, SIGNAL(clicked(bool)), this, SLOT(cmhandle_confirm()));
}

channelmenu_init::~channelmenu_init()
{

}

void channelmenu_init::cmhandle_setchanneldefault()
{
    emit cmsignaltype(cmsignal_setchanneldefault);
}

void channelmenu_init::cmhandle_confirm()
{
    emit cmsignaltype(cmsignal_confirm);
}

QTableWidget *channelmenu_init::setchannel()
{
    return this->TWchannel;
}

specfymenu_init::specfymenu_init()
{
    QIcon TBcleardataicon;
    TBcleardataicon.addFile(":/png/png/delete-128.png");
    TBcleardata = new QToolButton(this);
    TBcleardata->resize(110, 110);
    TBcleardata->setFont(*ft);
    TBcleardata->setText(tr("清空数据"));
    TBcleardata->setStyleSheet("background-color:rgba(0,0,0,0)");
    TBcleardata->setIcon(TBcleardataicon);
    TBcleardata->setIconSize(QSize(50, 50));
    TBcleardata->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    TBcleardata->move(20, 0);

    QIcon TBsoftreseticon;
    TBsoftreseticon.addFile(":/png/png/sinchronize-128.png");
    TBsoftreset = new QToolButton(this);
    TBsoftreset->resize(110, 110);
    TBsoftreset->setFont(*ft);
    TBsoftreset->setText(tr("软复位"));
    TBsoftreset->setStyleSheet("background-color:rgba(0,0,0,0)");
    TBsoftreset->setIcon(TBsoftreseticon);
    TBsoftreset->setIconSize(QSize(50, 50));
    TBsoftreset->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    TBsoftreset->move(150, 0);

    QIcon TBpowerofficon;
    TBpowerofficon.addFile(":/png/png/nas-128.png");
    TBpoweroff = new QToolButton(this);
    TBpoweroff->resize(110, 110);
    TBpoweroff->setFont(*ft);
    TBpoweroff->setText(tr("关机"));
    TBpoweroff->setStyleSheet("background-color:rgba(0,0,0,0)");
    TBpoweroff->setIcon(TBpowerofficon);
    TBpoweroff->setIconSize(QSize(50, 50));
    TBpoweroff->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    TBpoweroff->move(280, 0);

    QIcon TBupdateicon;
    TBupdateicon.addFile(":/png/png/upload-128.png");
    TBupdate = new QToolButton(this);
    TBupdate->resize(110, 110);
    TBupdate->setFont(*ft);
    TBupdate->setText(tr("软件升级"));
    TBupdate->setStyleSheet("background-color:rgba(0,0,0,0)");
    TBupdate->setIcon(TBupdateicon);
    TBupdate->setIconSize(QSize(50, 50));
    TBupdate->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    TBupdate->move(410, 0);

    QIcon TBsoftdistoryicon;
    TBsoftdistoryicon.addFile(":/png/png/burn.png");
    TBsoftdistory = new QToolButton(this);
    TBsoftdistory->resize(110, 110);
    TBsoftdistory->setFont(*ft);
    TBsoftdistory->setText(tr("软销毁"));
    TBsoftdistory->setStyleSheet("background-color:rgba(0,0,0,0)");
    TBsoftdistory->setIcon(TBsoftdistoryicon);
    TBsoftdistory->setIconSize(QSize(50, 50));
    TBsoftdistory->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    TBsoftdistory->move(540, 0);

    LAnotice = new QLabel(this);
    LAnotice->setFont(*ft);
    LAnotice->setText(tr("这些操作对数据有一定危险性，请谨慎操作。"));
    LAnotice->move(20, 120);
    connect(this->TBcleardata, SIGNAL(clicked(bool)), this, SLOT(slot_emitcleardata()));
    connect(this->TBsoftreset, SIGNAL(clicked(bool)), this, SLOT(slot_emitsoftreset()));
    connect(this->TBpoweroff, SIGNAL(clicked(bool)), this, SLOT(slot_emitpoweroff()));
    connect(this->TBupdate, SIGNAL(clicked(bool)), this, SLOT(slot_emitupdate()));
    connect(this->TBsoftdistory, SIGNAL(clicked(bool)), this, SLOT(slot_emitsoftdistory()));
}

specfymenu_init::~specfymenu_init()
{

}

QToolButton* specfymenu_init::SetCleardata()
{
    return this->TBcleardata;
}

QToolButton* specfymenu_init::SetSoftreset()
{
    return this->TBsoftreset;
}

QToolButton* specfymenu_init::SetPoweroff()
{
    return this->TBpoweroff;
}

QToolButton* specfymenu_init::SetUpdate()
{
    return this->TBupdate;
}

QToolButton* specfymenu_init::SetSoftdistory()
{
    return this->TBsoftdistory;
}

void specfymenu_init::slot_emitcleardata()
{
    emit SIGNAL_SpmType(spmsignal_cleardata);
}

void specfymenu_init::slot_emitsoftreset()
{
    emit SIGNAL_SpmType(spmsignal_softreset);
}

void specfymenu_init::slot_emitpoweroff()
{
    emit SIGNAL_SpmType(spmsignal_poweroff);
}

void specfymenu_init::slot_emitupdate()
{
    emit SIGNAL_SpmType(spmsignal_update);
}

void specfymenu_init::slot_emitsoftdistory()
{
    emit SIGNAL_SpmType(spmsignal_spftdistory);
}
