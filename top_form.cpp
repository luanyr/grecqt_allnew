#include "top_form.h"
#include "ui_top_form.h"

Top_Form::Top_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Top_Form)
{
    ui->setupUi(this);
    this->resize(2000, 1000);
    Startmenu_Init();
    Content_Init();
    connect(this->CBXmenu, SIGNAL(currentIndexChanged(QString)), this, SLOT(menu_change(QString)));
    connect(this->sm, SIGNAL(SIGNAL_Pbtype(int)), this, SLOT(slot_handlepbsignals(int)));

}

Top_Form::~Top_Form()
{
    delete ui;
}

void Top_Form::Startmenu_Init()
{
    ft.setPixelSize(12);
    menulist << tr("开始") << tr("记录") << tr("回放") << tr("筛选") << tr("管理")\
             << tr("用户") <<tr("通道") << tr("特殊");
    CBXmenu = new QComboBox(this);
    CBXmenu->resize(100, 20);
    CBXmenu->setFont(ft);
    CBXmenu->addItems(menulist);
    CBXmenu->move(10,40);
    sm = new startmenu_init();
    sm->setParent(this);
    sm->move(120, 0);
    rcdm = new recordmenu_init();
    rcdm->setParent(this);
    rcdm->setHidden(true);
    rpm = new replaymenu_init();
    rpm->setParent(this);
    rpm->setHidden(true);
    slcm = new selectmenu_init();
    slcm->setParent(this);
    slcm->setHidden(true);
    mgm = new managemenu_init();
    mgm->setParent(this);
    mgm->setHidden(true);
    um = new usermenu_init();
    um->setParent(this);
    um->setHidden(true);
    cm = new channelmenu_init();
    cm->setParent(this);
    cm->setHidden(true);
    spm = new specfymenu_init();
    spm->setParent(this);
    spm->setHidden(true);
}

void Top_Form::menu_distory()
{
    if(sm != NULL)  delete sm;
    else if (rcdm != NULL)   delete rcdm;
    else if (rpm  != NULL)   delete rpm;
    else if (slcm != NULL)   delete slcm;
    else if (mgm  != NULL)   delete mgm;
    else if (um   != NULL)   delete um;
    else if (cm   != NULL)   delete cm;
    else if (spm  != NULL)   delete spm;
    sm   = NULL;
    rcdm = NULL;
    rpm  = NULL;
    slcm = NULL;
    mgm  = NULL;
    um   = NULL;
    cm   = NULL;
    spm  = NULL;
}

void Top_Form::menu_hidden()
{
    sm  ->setHidden(true);
    rcdm->setHidden(true);
    rpm ->setHidden(true);
    slcm->setHidden(true);
    mgm ->setHidden(true);
    um  ->setHidden(true);
    cm  ->setHidden(true);
    spm ->setHidden(true);
}
void Top_Form::menu_change(QString text)
{
    menu_hidden();
    //menu_distory();
    if(text == tr("开始"))
    {
        sm->move(120, 0);
        sm->show();
        connect(this->sm, SIGNAL(SIGNAL_Pbtype(int)), this, SLOT(slot_handlepbsignals(int)));
    } else if(text == tr("记录"))
    {
        rcdm->move(120, 0);
        rcdm->show();
    } else if(text == tr("回放"))
    {
        rpm->move(120, 0);
        rpm->show();
    } else if(text == tr("筛选"))
    {
        slcm->move(120, 0);
        slcm->show();
    } else if(text == tr("管理"))
    {
        mgm->move(120, 0);
        mgm->show();
    } else if(text == tr("用户"))
    {
        um->move(120, 0);
        um->show();
    } else if(text == tr("通道"))
    {
        cm->move(120, 0);
        cm->show();
        connect(this->cm, SIGNAL(cmsignaltype(int)), this, SLOT(slot_cmhandlesignals(int)));
    } else if(text == tr("特殊"))
    {
        spm->move(120, 0);
        spm->show();
    }
}

void Top_Form::Content_Init()
{
    cu = new ContentUI(this);
    cu->move(5, 200);
}

void Top_Form::UIShowMsg(QString msg)
{
    this->cu->ListShowStatus(msg);
}

QString Top_Form::Getipaddr()
{
    return this->sm->Getipaddr();
}

QString Top_Form::Getipport()
{
    return this->sm->Getipport();
}

QString Top_Form::Getusrname()
{
    return this->sm->Getusrname();
}

QString Top_Form::Getuserpswd()
{
    return this->sm->Getusrpswd();
}

void Top_Form::slot_handlepbsignals(int type)
{
    switch (type) {
    case PBconnect:
        emit signals_pbtype(PBconnect);
        break;
    case PBdisconnect:
        emit signals_pbtype(PBdisconnect);
        break;
    case PBuserlogin:
        emit signals_pbtype(PBuserlogin);
        break;
    case PBuserlogout:
        emit signals_pbtype(PBuserlogout);
        break;
    case PBturnoffapp:
        emit signals_pbtype(PBturnoffapp);
        break;
    default:
        break;
    }
}

void Top_Form::slot_cmhandlesignals(int type)
{
    switch (type) {
    case cmsignal_setchanneldefault:
        signals_cmtype(cmsignal_setchanneldefault);
        break;
    case cmsignal_confirm:
        signals_cmtype(cmsignal_confirm);
        break;
    default:
        break;
    }
}
void Top_Form::CuStatusNet(int status)
{
    this->cu->Statusnet_Change(status);
}

void Top_Form::CuStatusUsr(QString usrname, int status)
{
    this->cu->Statususr_Change(usrname, status);
}

QTableWidget *Top_Form::setchannel()
{
    return this->cm->setchannel();
}

QTableWidget* Top_Form::SetTlwDevSta()
{
    return this->cu->SetTLWDevSta();
}
