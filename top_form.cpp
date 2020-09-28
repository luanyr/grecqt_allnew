#include "top_form.h"
#include "ui_top_form.h"

Top_Form::Top_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Top_Form)
{
    ui->setupUi(this);
    this->resize(1200, 800);
    Startmenu_Init();
    Content_Init();
    connect(this->CBXmenu, SIGNAL(currentIndexChanged(QString)), this, SLOT(menu_change(QString)));

}

Top_Form::~Top_Form()
{
    delete ui;
}

void Top_Form::Startmenu_Init()
{
    ft.setPixelSize(12);
    menulist << tr("开始") << tr("记录") << tr("回放") << tr("筛选") << tr("管理")\
             << tr("用户") << tr("特殊");
    CBXmenu = new QComboBox(this);
    CBXmenu->resize(100, 20);
    CBXmenu->setFont(ft);
    CBXmenu->addItems(menulist);
    CBXmenu->move(10,40);
    sm = new startmenu_init();
    sm->setParent(this);
    sm->move(120, 0);
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

void Top_Form::menu_change(QString text)
{
    menu_distory();
    if(text == tr("开始"))
    {
        sm = new startmenu_init();
        sm->setParent(this);
        sm->move(120, 0);
        sm->show();
    } else if(text == tr("记录"))
    {
        rcdm = new recordmenu_init();
        rcdm->setParent(this);
        rcdm->move(120, 0);
        rcdm->show();
    } else if(text == tr("回放"))
    {
        rpm = new replaymenu_init();
        rpm->setParent(this);
        rpm->move(120, 0);
        rpm->show();
    } else if(text == tr("筛选"))
    {
        slcm = new selectmenu_init();
        slcm->setParent(this);
        slcm->move(120, 0);
        slcm->show();
    } else if(text == tr("管理"))
    {
        mgm = new managemenu_init();
        mgm->setParent(this);
        mgm->move(120, 0);
        mgm->show();
    } else if(text == tr("用户"))
    {
        um = new usermenu_init();
        um->setParent(this);
        um->move(120, 0);
        um->show();
    } else if(text == tr("通道"))
    {
        cm = new channelmenu_init();
        cm->setParent(this);
        cm->move(120, 0);
        cm->show();
    } else if(text == tr("特殊"))
    {
        spm = new specfymenu_init();
        spm->setParent(this);
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
