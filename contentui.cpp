#include "contentui.h"
#include "ui_contentui.h"

ContentUI::ContentUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContentUI)
{
    ui->setupUi(this);
    QRect deskRect = QApplication::desktop()->availableGeometry();
    this->resize(deskRect.width(), (deskRect.height() / 4) * 3);
    TW_init();
    FileView_init();
    StatusbarInit();

}

ContentUI::~ContentUI()
{
    delete ui;
}

void ContentUI::TW_init()
{
    ft = new QFont();
    ft->setPixelSize(16);
    TW_DevStaCap = new QTabWidget(this);
    W_DevSta = new QWidget(this);
    W_Selfcheck = new QWidget(this);
    LAselfcheck = new QLabel(this);

    W_DevSta->resize(350, 690);
    W_Selfcheck->resize(350, 690);
    TLW_DevStatus = new QTableWidget();
    TLW_DevStatus->setParent(W_DevSta);
    TLW_DevStatus->resize(350, 690);
    TLW_DevStatus->move(5,5);
    TLW_DevStatus->setColumnCount(2);
    TLW_DevStatus->setFont(*ft);
    //TLW_DevStatus->setRowCount(11 + );
    TW_DevStaCap->resize(360, 700);
    W_Selfcheck->setFont(*ft);
    TW_DevStaCap->insertTab(0, W_DevSta, tr("设备状态"));

    LAselfcheck = new QLabel(W_Selfcheck);
    LAselfcheck->setText(tr("自检状态"));
    LAselfcheck->setFont(*ft);
    LAselfcheck->move(10,5);

    LW_listbit = new QListWidget(W_Selfcheck);
    LW_listbit->resize(265, 120);
    LW_listbit->move(5, 20);

    QIcon PB_bitsmallicon;
    PB_bitsmallicon.addFile(tr(":/png/png/settings-128.png"));
    PB_bitsmall = new QPushButton(W_Selfcheck);
    PB_bitsmall->resize(30, 30);
    PB_bitsmall->move(280, 20);
    PB_bitsmall->setIcon(PB_bitsmallicon);
    PB_bitsmall->setIconSize(QSize(25, 25));

    WGTChart = new QWidget(W_Selfcheck);
    WGTChart->move(0, 150);
    WGTChart->resize(350, 500);
    VBLChart = new QVBoxLayout(WGTChart);
    VBLChart->setSpacing(2);
    VBLChart->setSizeConstraint(QLayout::SetMaximumSize);

    TW_DevStaCap->insertTab(1, W_Selfcheck, tr("自检和容量"));
}

void ContentUI::FileView_init()
{
    LA_filetext = new QLabel(this);
    LA_filetext->setFont(*ft);
    LA_filetext->setText(tr("请在“筛选”功能中设置条件后点击“查询目录”更新列表中显示的文件"));
    LA_filetext->move(400, 5);

    TRW_treefile = new QTreeWidget(this);
    TRW_treefile->resize(200, 680);
    TRW_treefile->move(400, 25);
    TRW_treefile->setFont(*ft);

    TW_tablefile = new QTableWidget(this);
    TW_tablefile->resize(700, 680);
    TW_tablefile->move(650, 25);
    TW_tablefile->setFont(*ft);

    TB_liststatus = new QTextBrowser(this);
    TB_liststatus->resize(200, 680);
    TB_liststatus->move(1400, 25);
    TB_liststatus->setFont(*ft);
    connect(this->TRW_treefile, SIGNAL(itemSelectionChanged()), this, SLOT(slot_emittrwitemchange()));
    connect(this->PB_bitsmall, SIGNAL(clicked(bool)), this, SLOT(slot_emitPbBitSmall()));
}

void ContentUI::ListShowStatus(QString msg)
{
    this->TB_liststatus->append(tr("[")+DATETIME+tr("] ")+msg);
}

void ContentUI::StatusbarInit()
{
    QRect deskRect = QApplication::desktop()->availableGeometry();
    this->statusBar = new QStatusBar(this);
    statusBar->setObjectName(QString::fromUtf8("statusBar"));
    statusBar->move(5, deskRect.height()- 300);

    PB_statnet = new QPushButton;
    //PB_statnet->setMinimumSize(PB_statnet->sizeHint());
    PB_statnet->setToolTip(tr("显示与设备的连接状态"));
    QFont font("宋体",8,QFont::Bold);
    PB_statnet->setFont(font);
    PB_statnet->setIcon(QIcon(":/ico/icon/dead.ico"));
    PB_statnet->setText(tr("未连接"));
    PB_statnet->resize(QSize(40,40));
    statusBar->addWidget(PB_statnet);

    QFrame *separator1 = new QFrame;
    separator1->setFrameShape(QFrame::VLine);
    statusBar->addWidget(separator1);

    LAstatusr = new QLabel;
    LAstatusr->setMinimumSize(LAstatusr->sizeHint());
    LAstatusr->setAlignment(Qt::AlignLeft);
    LAstatusr->setText(tr("当前用户:         "));
    statusBar->addWidget(LAstatusr);

    QFrame *separator2 = new QFrame;
    separator2->setFrameShape(QFrame::VLine);
    statusBar->addWidget(separator2);

    LAstatwork = new QLabel;
    LAstatwork->setMinimumSize(LAstatwork->sizeHint());
    LAstatwork->setAlignment(Qt::AlignLeft);
    LAstatwork->setText(tr("工作状态: 空闲"));
    statusBar->addWidget(LAstatwork);

    QFrame *separator3 = new QFrame;
    separator3->setFrameShape(QFrame::VLine);
    statusBar->addWidget(separator3);

    progress = new QProgressBar;
    progress->setOrientation(Qt::Horizontal);
    progress->setMinimum(0);  // 最小值
    progress->setMaximum(100);  // 最大值
    progress->hide();
    statusBar->addWidget(progress);

    statusBar->setStyleSheet(QString("QStatusBar::item{border: 0px}"));
}

void ContentUI::Statusnet_Change(int connectstatus)
{

    if(connectstatus == statusnet_connected)
    {
        QFont font("宋体",8,QFont::Bold);
        PB_statnet->setFont(font);
        PB_statnet->setIcon(QIcon(":/ico/icon/alive.ico"));
        PB_statnet->setText(tr("已连接"));
        PB_statnet->resize(QSize(40,40));
    } else if (connectstatus == statusnet_disconnected)
    {
        QFont font("宋体",8,QFont::Bold);
        PB_statnet->setFont(font);
        PB_statnet->setIcon(QIcon(":/ico/icon/dead.ico"));
        PB_statnet->setText(tr("未连接"));
        PB_statnet->resize(QSize(40,40));
    }
}

void ContentUI::Statususr_Change(QString usrname, int loginstatus)
{
    if(loginstatus == statususr_login)
    {
        LAstatusr->setAlignment(Qt::AlignLeft);
        LAstatusr->setText(tr("当前用户: ") + usrname);
    } else if (loginstatus == statususr_logout)
    {
        LAstatusr->setAlignment(Qt::AlignLeft);
        LAstatusr->setText(tr("当前用户:         "));
    }
}

void ContentUI::Statuswork_Change(QString workstatus)
{
    if(!workstatus.isEmpty())
    {
        if(workstatus.contains("空闲"))
            LAstatwork->setText(tr("工作状态: 空闲"));
        else if(workstatus.contains("记录"))
            LAstatwork->setText(tr("工作状态: 记录"));
        else if(workstatus.contains("回放"))
            LAstatwork->setText(tr("工作状态: 回放"));
        else if(workstatus.contains("格式化"))
            LAstatwork->setText(tr("工作状态: 格式化"));
    }
}
QTableWidget* ContentUI::SetTLWDevSta()
{
    return this->TLW_DevStatus;
}

QListWidget* ContentUI::SetLWListBit()
{
    return this->LW_listbit;
}

QTableWidget *ContentUI::SetTableFile()
{
    return this->TW_tablefile;
}

QTreeWidget *ContentUI::SetTreeFile()
{
    return this->TRW_treefile;
}

QPushButton *ContentUI::Setbitsmall()
{
    return this->PB_bitsmall;
}

QListWidget *ContentUI::Setlistbit()
{
    return this->LW_listbit;
}

QProgressBar *ContentUI::SetProgress()
{
    return this->progress;
}

void ContentUI::slot_emittrwitemchange()
{
    emit SIGNAL_CuType(cusignal_trwitemselectionchange);
}

void ContentUI::slot_emitPbBitSmall()
{
    emit SIGNAL_CuType(cusignal_pbpbbitsmall);
}

QTabWidget *ContentUI::SetDevStaCap()
{
    return this->TW_DevStaCap;
}

QVBoxLayout* ContentUI::SetChart()
{
    return this->VBLChart;
}
