#include "contentui.h"
#include "ui_contentui.h"

ContentUI::ContentUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContentUI)
{
    ui->setupUi(this);
    this->resize(1200, 600);

    TW_init();
    FileView_init();
}

ContentUI::~ContentUI()
{
    delete ui;
}

void ContentUI::TW_init()
{
    ft = new QFont();
    ft->setPixelSize(12);
    TW_DevStaCap = new QTabWidget(this);
    W_DevSta = new QWidget(this);
    W_Selfcheck = new QWidget(this);
    LAselfcheck = new QLabel(this);

    W_DevSta->resize(300, 500);
    TLW_DevStatus = new QTableWidget();
    TLW_DevStatus->setParent(W_DevSta);
    TLW_DevStatus->resize(290, 490);
    TLW_DevStatus->move(5,5);

    TW_DevStaCap->resize(320, 520);
    TW_DevStaCap->insertTab(0, W_DevSta, tr("设备状态"));

    LAselfcheck = new QLabel(W_Selfcheck);
    LAselfcheck->setText(tr("自检状态"));
    LAselfcheck->setFont(*ft);
    LAselfcheck->move(10,5);

    LW_listbit = new QListWidget(W_Selfcheck);
    LW_listbit->resize(250, 150);
    LW_listbit->move(5, 20);

    QIcon PB_bitsmallicon;
    PB_bitsmallicon.addFile(tr(":/png/png/settings-128.png"));
    PB_bitsmall = new QPushButton(W_Selfcheck);
    PB_bitsmall->resize(30, 30);
    PB_bitsmall->move(280, 20);
    PB_bitsmall->setIcon(PB_bitsmallicon);
    PB_bitsmall->setIconSize(QSize(25, 25));
    TW_DevStaCap->insertTab(1, W_Selfcheck, tr("自检和容量"));
}

void ContentUI::FileView_init()
{
    LA_filetext = new QLabel(this);
    LA_filetext->setFont(*ft);
    LA_filetext->setText(tr("请在“筛选”功能中设置条件后点击“查询目录”更新列表中显示的文件"));
    LA_filetext->move(400, 5);

    TRW_treefile = new QTreeWidget(this);
    TRW_treefile->resize(130, 500);
    TRW_treefile->move(400, 20);

    TW_tablefile = new QTableWidget(this);
    TW_tablefile->resize(550, 500);
    TW_tablefile->move(550, 20);

    TB_liststatus = new QTextBrowser(this);
    TB_liststatus->resize(160, 500);
    TB_liststatus->move(1200, 500);
}

void ContentUI::ListShowStatus(QString msg)
{
    this->TB_liststatus->append(tr("[")+DATETIME+tr("] ")+msg);
}
