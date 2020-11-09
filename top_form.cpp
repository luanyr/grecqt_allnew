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
    connect(this->rcdm, SIGNAL(SIGNAL_RcdmType(int)), this, SLOT(slot_rcdmhandlesignal(int)));
    connect(this->sm, SIGNAL(SIGNAL_Pbtype(int)), this, SLOT(slot_handlepbsignals(int)));
    connect(this->rpm, SIGNAL(SIGNAL_RpmType(int)), this, SLOT(slot_rpmhandlesignals(int)));
    connect(this->slcm, SIGNAL(SIGNAL_SmType(int)), this, SLOT(slot_slcmhandlesignal(int)));
    connect(this->mgm, SIGNAL(SIGNAL_MgmType(int)), this, SLOT(slot_mgmhandlesignal(int)));
    connect(this->um, SIGNAL(SIGNAL_UmType(int)), this, SLOT(slot_umhandlesignal(int)));
    connect(this->spm, SIGNAL(SIGNAL_SpmType(int)), this, SLOT(slot_spmhandlesignal(int)));
    connect(this->cu, SIGNAL(SIGNAL_CuType(int)), this, SLOT(slot_cuhandlesignal(int)));
}

Top_Form::~Top_Form()
{
    delete ui;
}

void Top_Form::Startmenu_Init()
{
    ft.setPixelSize(16);
    menulist << tr("开始") << tr("记录") << tr("回放") << tr("筛选") << tr("管理")\
             << tr("用户") <<tr("通道") << tr("特殊");
    CBXmenu = new QComboBox(this);
    CBXmenu->resize(100, 30);
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

void Top_Form::slot_rcdmhandlesignal(int type)
{
    switch (type) {
    case rcdmsignal_setfilesz:
        emit signals_rcdmtype(rcdmsignal_setfilesz);
        break;
    case rcdmsignal_chkRec_A:
        emit signals_rcdmtype(rcdmsignal_chkRec_A);
        break;
    case rcdmsignal_chkRec_B:
        emit signals_rcdmtype(rcdmsignal_chkRec_B);
        break;
    case rcdmsignal_recstart:
        emit signals_rcdmtype(rcdmsignal_recstart);
        break;
    case rcdmsignal_recstop:
        emit signals_rcdmtype(rcdmsignal_recstop);
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

void Top_Form::slot_rpmhandlesignals(int type)
{
    switch (type) {
    case rpmsignal_chkchnselreplay:
        emit signals_rpmtype(rpmsignal_chkchnselreplay);
        break;
    case rpmsignal_chkReplayA:
        emit signals_rpmtype(rpmsignal_chkReplayA);
        break;
    case rpmsignal_chkReplayB:
        emit signals_rpmtype(rpmsignal_chkReplayB);
        break;
    case rpmsignal_cjkTimeSelReplay:
        emit signals_rpmtype(rpmsignal_cjkTimeSelReplay);
        break;
    case rpmsignal_chktypeSelReplay:
        emit signals_rpmtype(rpmsignal_chktypeSelReplay);
        break;
    case rpmsignal_type1:
        emit signals_rpmtype(rpmsignal_type1);
        break;
    case rpmsignal_type2:
        emit signals_rpmtype(rpmsignal_type2);
        break;
    case rpmsignal_type3:
        emit signals_rpmtype(rpmsignal_type3);
        break;
    case rpmsignal_type4:
        emit signals_rpmtype(rpmsignal_type4);
        break;
    case rpmsignal_chkChanSelReplay:
        emit signals_rpmtype(rpmsignal_chkChanSelReplay);
        break;
    case rpmsignal_replay:
        emit signals_rpmtype(rpmsignal_replay);
        break;
    case rpmsignal_pause:
        emit signals_rpmtype(rpmsignal_pause);
        break;
    case rpmsignal_stop:
        emit signals_rpmtype(rpmsignal_stop);
        break;
    default:
        break;
    }
}

void Top_Form::slot_slcmhandlesignal(int type)
{
    switch (type) {
    case smsignal_chkchnselect:
        emit signals_smtype(smsignal_chkchnselect);
        break;
    case smsignal_chkSelectTypeA:
        emit signals_smtype(smsignal_chkSelectTypeA);
        break;
    case smsignal_chkSelectTypeB:
        emit signals_smtype(smsignal_chkSelectTypeB);
        break;
    case smsignal_chkTimeSelect:
        emit signals_smtype(smsignal_chkTimeSelect);
        break;
    case smsignal_chkTypeSelect:
        emit signals_smtype(smsignal_chkTypeSelect);
        break;
    case smsginal_type1:
        emit signals_smtype(smsginal_type1);
        break;
    case smsginal_type2:
        emit signals_smtype(smsginal_type2);
        break;
    case smsginal_type3:
        emit signals_smtype(smsginal_type3);
        break;
    case smsginal_type4:
        emit signals_smtype(smsginal_type4);
        break;
    case smsignal_DeleteFile:
        emit signals_smtype(smsignal_DeleteFile);
        break;
    case smsignal_InqDir:
        emit signals_smtype(smsignal_InqDir);
        break;
    default:
        break;
    }
}

void Top_Form::slot_mgmhandlesignal(int type)
{
    switch (type) {
    case mgmsignal_timesync:
        emit signals_mgmtype(mgmsignal_timesync);
        break;
    case mgmsignal_sendtime:
        emit signals_mgmtype(mgmsignal_sendtime);
        break;
    case mgmsignal_selfcheck:
        emit signals_mgmtype(mgmsignal_selfcheck);
        break;
    default:
        break;
    }
}

void Top_Form::slot_umhandlesignal(int type)
{
    switch (type) {
    case umsignal_creatusr:
        emit signals_umtype(umsignal_creatusr);
        break;
    case umsignal_modifypswd:
        emit signals_umtype(umsignal_modifypswd);
        break;
    case umsignal_delusr:
        emit signals_umtype(umsignal_delusr);
        break;
    default:
        break;
    }
}

void Top_Form::slot_spmhandlesignal(int type)
{
    switch (type) {
    case spmsignal_cleardata:
        emit signals_spmtype(spmsignal_cleardata);
        break;
    case spmsignal_softreset:
        emit signals_spmtype(spmsignal_softreset);
        break;
    case spmsignal_poweroff:
        emit signals_spmtype(spmsignal_poweroff);
        break;
    case spmsignal_update:
        emit signals_spmtype(spmsignal_update);
        break;
    case spmsignal_spftdistory:
        emit signals_spmtype(spmsignal_spftdistory);
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

void Top_Form::CuStatusWork(QString workstatus)
{
    this->cu->Statuswork_Change(workstatus);
}

QToolButton *Top_Form::SetSmConnect()
{
    return this->sm->SetTbConnect();
}

QToolButton *Top_Form::SetSmDisconnect()
{
    return this->sm->SetTbDisconnect();
}

QToolButton *Top_Form::SetSmLogin()
{
    return this->sm->SetTblogin();
}

QToolButton *Top_Form::SetSmLogout()
{
    return this->sm->SetTblogout();
}

QTableWidget *Top_Form::setchannel()
{
    return this->cm->setchannel();
}

QTableWidget* Top_Form::SetTlwDevSta()
{
    return this->cu->SetTLWDevSta();
}

QListWidget* Top_Form::SetLwListBit()
{
    return this->cu->SetLWListBit();
}

QPushButton* Top_Form::SetCuBitsmall()
{
    return this->cu->Setbitsmall();
}

QListWidget* Top_Form::SetCuListBit()
{
    return this->cu->Setlistbit();
}

QLineEdit* Top_Form::SetTcpAddr()
{
    return this->sm->GetAddrEdit();
}

QLineEdit* Top_Form::SetTcport()
{
    return this->sm->GetPortEdit();
}

QLineEdit* Top_Form::SetUserName()
{
    return this->sm->GetUserNameEdit();
}

QLineEdit* Top_Form::SetUserPswd()
{
    return this->sm->GetUserpswdEdit();
}

QPushButton* Top_Form::SetRcdmChkA()
{
    return this->rcdm->SetchkRec_A();
}

QPushButton* Top_Form::SetRcdmChkB()
{
    return this->rcdm->SetchkRec_B();
}

UINT32 Top_Form::GetRcdmFilesz()
{
    return this->rcdm->GetFilesz();
}

QString Top_Form::GetRcdmFileszUint()
{
    return this->rcdm->GetFileszUint();
}

QToolButton* Top_Form::SetRcdmRecStart()
{
    return this->rcdm->SetRecStart();
}

QToolButton* Top_Form::SetRcdmRecStop()
{
    return this->rcdm->SetRecStop();
}

QToolButton* Top_Form::SetRcdmFilesz()
{
    return this->rcdm->SetRecFilesz();
}

QPushButton* Top_Form::SetRpmChkA()
{
    return this->rpm->SetchkReplayA();
}

QPushButton* Top_Form::SetRpmChkB()
{
    return this->rpm->SetchkReplayB();
}

QPushButton* Top_Form::SetRpmType1()
{
    return this->rpm->SetType1();
}

QPushButton* Top_Form::SetRpmType2()
{
    return this->rpm->SetType2();
}

QPushButton* Top_Form::SetRpmType3()
{
    return this->rpm->SetType3();
}

QPushButton* Top_Form::SetRpmType4()
{
    return this->rpm->SetType4();
}

QPushButton* Top_Form::SetRpmChkchnselreplay()
{
    return this->rpm->SetchkChnSelReplay();
}

QPushButton* Top_Form::SetRpmchkchansetreplay()
{
    return this->rpm->SetchkChanSelReplay();
}

QPushButton* Top_Form::SetRpmchktimesetreplay()
{
    return this->rpm->SetchkTimeSelReplay();
}

QPushButton* Top_Form::SetRpmchktypesetreplay()
{
    return this->rpm->SetchkTypeSelReplay();
}

QComboBox* Top_Form::SetRpmOutCom()
{
    return this->rpm->SetOutCom();
}

QDateTimeEdit* Top_Form::SetRpmStartTime()
{
    return this->rpm->SetStartTime();
}

QDateTimeEdit* Top_Form::SetRpmEndTime()
{
    return this->rpm->SetEndTime();
}

QToolButton* Top_Form::SetRpmReplay()
{
    return this->rpm->SetTbReplay();
}

QToolButton* Top_Form::SetRpmPause()
{
    return this->rpm->SetTbPause();
}

QToolButton* Top_Form::SetRpmStop()
{
    return this->rpm->SetTbStop();
}

QPushButton* Top_Form::SetSlcmChkA()
{
    return this->slcm->SetchkReplayA();
}

QPushButton* Top_Form::SetSlcmChkB()
{
    return this->slcm->SetchkReplayB();
}

QPushButton* Top_Form::SetSlcmChnSelect()
{
    return this->slcm->SetchkChnSelReplay();
}

QPushButton* Top_Form::SetSlcmTimeSelect()
{
    return this->slcm->SetchkTimeSelReplay();
}

QPushButton* Top_Form::SetSlcmTypeSelect()
{
    return this->slcm->SetchkTypeSelReplay();
}

QDateTimeEdit* Top_Form::SetSlcmStartTime()
{
    return this->slcm->SetStartTime();
}

QDateTimeEdit* Top_Form::SetSlcmEndTime()
{
    return this->slcm->SetEndTime();
}

QPushButton* Top_Form::SetSlcmType1()
{
    return this->slcm->SetType1();
}

QPushButton* Top_Form::SetSlcmType2()
{
    return this->slcm->SetType2();
}

QPushButton* Top_Form::SetSlcmType3()
{
    return this->slcm->SetType3();
}

QPushButton* Top_Form::SetSlcmType4()
{
    return this->slcm->SetType4();
}

QToolButton* Top_Form::SetSlcmDelFile()
{
    return this->slcm->SetDelFile();
}

QToolButton* Top_Form::SetSlcmInqDir()
{
    return this->slcm->SetInqDir();
}

QTableWidget* Top_Form::SetCuTableFile()
{
    return this->cu->SetTableFile();
}

QTreeWidget* Top_Form::SetCuTreeFile()
{
    return this->cu->SetTreeFile();
}

QPushButton* Top_Form::SetMgmTimeSync()
{
    return this->mgm->SetTimeSync();
}

QDateTimeEdit* Top_Form::SetMgmTimeEdit()
{
    return this->mgm->SetTimeEdit();
}

QToolButton* Top_Form::SetMgmSendTime()
{
    return this->mgm->SetSendTime();
}

QToolButton* Top_Form::SetMgmSelfCheck()
{
    return this->mgm->SetSelfCheck();
}

QString Top_Form::GetUmUsrname()
{
    return this->um->Getusrname();
}

QString Top_Form::GetUmUsrpswd()
{
    return this->um->Getusrpswd();
}

QToolButton* Top_Form::SetUmCreatUsr()
{
    return this->um->SetCreatUsr();
}

QToolButton* Top_Form::SetUmModifyPswd()
{
    return this->um->SetModifyPswd();
}

QToolButton* Top_Form::SetUmDelusr()
{
    return this->um->SetDelUsr();
}

QToolButton* Top_Form::SetSpmClearData()
{
    return this->spm->SetCleardata();
}

QToolButton* Top_Form::SetSpmPoweroff()
{
    return this->spm->SetPoweroff();
}

QToolButton* Top_Form::SetSpmSoftdistory()
{
    return this->spm->SetSoftdistory();
}

QToolButton* Top_Form::SetSpmSoftReset()
{
    return this->spm->SetSoftreset();
}

QToolButton* Top_Form::SetSpmUpdate()
{
    return this->spm->SetUpdate();
}

QProgressBar* Top_Form::SetCuProgress()
{
    return this->cu->SetProgress();
}

void Top_Form::slot_cuhandlesignal(int type)
{
    switch (type) {
    case cusignal_trwitemselectionchange:
        emit signals_cutype(cusignal_trwitemselectionchange);
        break;
    case cusignal_pbpbbitsmall:
        emit signals_cutype(cusignal_pbpbbitsmall);
        break;
    default:
        break;
    }
}

 QTabWidget *Top_Form::SetCuDevStaCap()
 {
     return this->cu->SetDevStaCap();
 }

 QVBoxLayout *Top_Form::SetCuChart()
 {
     return this->cu->SetChart();
 }
