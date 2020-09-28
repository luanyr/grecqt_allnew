#ifndef MENUUI_H
#define MENUUI_H

#include <QWidget>
#include "headers.h"
namespace Ui {
class MenuUI;
}

class MenuUI : public QWidget
{
    Q_OBJECT

public:
    explicit MenuUI(QWidget *parent = 0);
    ~MenuUI();
    QFont *ft;
private:
    Ui::MenuUI *ui;
};

class startmenu_init : public  MenuUI
{
public:
    startmenu_init();
    ~startmenu_init();
private:
    QLabel *LAipaddr;
    QLabel *LAport;
    QLineEdit *LEipaddr;
    QLineEdit *LEport;
    QToolButton *TBconnect;
    QToolButton *TBdisconnect;
    QLabel *LAusrname;
    QLabel *LApswd;
    QLineEdit *LEusrname;
    QLineEdit *LEpswd;
    QToolButton *TBlogin;
    QToolButton *TBlogout;
    QToolButton *TBcloseapp;
};

class recordmenu_init : public MenuUI
{
public:
    recordmenu_init();
    ~recordmenu_init();
private:
    QLineEdit *LEfilesz;
    QComboBox *CBfilesz;
    QToolButton *TBfilesz;
    QToolButton *TBchkRec_A;
    QToolButton *TBchkRec_B;
    QToolButton *TBRecStart;
    QToolButton *TBRecStop;
};

class replaymenu_init : public MenuUI
{
public:
    replaymenu_init();
    ~replaymenu_init();
private:
    QPushButton *PBchkchnselreplay;
    QPushButton *TBchkReplayA;
    QPushButton *TBchkReplayB;
    QPushButton *PBcjkTimeSelReplay;
    QLabel *LAstarttime;
    QLabel *LAendtime;
    QDateTimeEdit *DTEstarttime;
    QDateTimeEdit *DTEendtime;
    QPushButton *PBchktypeSelReplay;
    QPushButton *PBtype1;
    QPushButton *PBtype2;
    QPushButton *PBtype3;
    QPushButton *PBtype4;
    QPushButton *PBchkChanSelReplay;
    QComboBox *CBXoutcom;
    QToolButton *TBreplay;
    QToolButton *TBpause;
    QToolButton *TBstop;
    QLine *line1;
    QLine *line2;
    QLine *line3;
    QLine *line4;
};

class selectmenu_init : public MenuUI
{
public:
    selectmenu_init();
    ~selectmenu_init();
private:
    QPushButton *PBchkchnselreplay;
    QToolButton *TBchkReplayA;
    QToolButton *TBchkReplayB;
    QPushButton *PBcjkTimeSelReplay;
    QLabel *LAstarttime;
    QLabel *LAendtime;
    QDateTimeEdit *DTEstarttime;
    QDateTimeEdit *DTEendtime;
    QPushButton *PBchktypeSelReplay;
    QPushButton *PBtype1;
    QPushButton *PBtype2;
    QPushButton *PBtype3;
    QPushButton *PBtype4;
    QToolButton *TBdelfile;
    QToolButton *TBsearchfile;
};

class managemenu_init : public MenuUI
{
    Q_OBJECT
public:
    managemenu_init();
    ~managemenu_init();
private:
    QTimer *pTime;
    QLabel *LAsettime;
    QPushButton *PBsync;
    QDateTimeEdit *DTEtime;
    QToolButton *TBsendtime;
    QToolButton *TBselfcheck;
public slots:
    void slot_timesyncfunc();
};

class usermenu_init : public MenuUI
{
public:
    usermenu_init();
    ~usermenu_init();
private:
    QLabel *LAusername;
    QLabel *LAuserpswd;
    QLineEdit *LEusername;
    QLineEdit *LEuserpwsd;
    QToolButton *TBcreatuser;
    QToolButton *TBmodifypsdw;
    QToolButton *TBdeluser;
};

class channelmenu_init : public MenuUI
{
public:
    channelmenu_init();
    ~channelmenu_init();
private:
    QLabel *LAmdftunnel;
    QPushButton *PBbacktodef;
    QPushButton *PBconfirm;
    QTabWidget *TWchannel;
};

class specfymenu_init : public MenuUI
{
public:
    specfymenu_init();
    ~specfymenu_init();
private:
    QToolButton *TBcleardata;
    QToolButton *TBsoftreset;
    QToolButton *TBpoweroff;
    QToolButton *TBupdate;
    QToolButton *TBsoftdistory;
    QLabel *LAnotice;
};

#endif // MENUUI_H
