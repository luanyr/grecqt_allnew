#ifndef MENUUI_H
#define MENUUI_H

#include <QWidget>
#include "headers.h"
#define PBconnect        0
#define PBdisconnect     1
#define PBuserlogin      2
#define PBuserlogout     3
#define PBturnoffapp     4

#define statusnet_connected         0x00
#define statusnet_disconnected      0x01

#define statususr_login             0x00
#define statususr_logout            0x01

#define cmsignal_setchanneldefault  0x00
#define cmsignal_confirm            0x01

#define rcdmsignal_setfilesz        0x00
#define rcdmsignal_chkRec_A         0x01
#define rcdmsignal_chkRec_B         0x02
#define rcdmsignal_recstart         0x03
#define rcdmsignal_recstop          0x04

#define rpmsignal_chkchnselreplay   0x00
#define rpmsignal_chkReplayA        0x01
#define rpmsignal_chkReplayB        0x02
#define rpmsignal_cjkTimeSelReplay  0x03
#define rpmsignal_chktypeSelReplay  0x04
#define rpmsignal_type1             0x05
#define rpmsignal_type2             0x06
#define rpmsignal_type3             0x07
#define rpmsignal_type4             0x08
#define rpmsignal_chkChanSelReplay  0x09
#define rpmsignal_replay            0x0a
#define rpmsignal_pause             0x0b
#define rpmsignal_stop              0x0c

#define smsignal_chkchnselect       0x00
#define smsignal_chkSelectTypeA     0x01
#define smsignal_chkSelectTypeB     0x02
#define smsignal_chkTimeSelect      0x03
#define smsignal_chkTypeSelect      0x04
#define smsginal_type1              0x05
#define smsginal_type2              0x06
#define smsginal_type3              0x07
#define smsginal_type4              0x08
#define smsignal_DeleteFile         0x09
#define smsignal_InqDir             0x0a

#define mgmsignal_timesync          0x00
#define mgmsignal_sendtime          0x01
#define mgmsignal_selfcheck         0x02

#define umsignal_creatusr           0x00
#define umsignal_modifypswd         0x01
#define umsignal_delusr             0x02

#define spmsignal_cleardata         0x00
#define spmsignal_softreset         0x01
#define spmsignal_poweroff          0x02
#define spmsignal_update            0x03
#define spmsignal_spftdistory       0x04
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
signals:

};

class startmenu_init : public  MenuUI
{
    Q_OBJECT
public:
    startmenu_init();
    ~startmenu_init();
    QString Getipaddr();
    QString Getipport();
    QString Getusrname();
    QString Getusrpswd();
    QLineEdit* GetAddrEdit();
    QLineEdit* GetPortEdit();
    QLineEdit *GetUserNameEdit();
    QLineEdit *GetUserpswdEdit();
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
public slots:
    void slot_emitconnnect();
    void slot_emitdisconnect();
    void slot_emituserlogin();
    void slot_emituserlogout();
    void slot_emitturnoffapp();
signals:
    void SIGNAL_Pbtype(int);
};

class recordmenu_init : public MenuUI
{
    Q_OBJECT
public:
    recordmenu_init();
    ~recordmenu_init();
    QPushButton *SetchkRec_A();
    QPushButton *SetchkRec_B();
    UINT32 GetFilesz();
    QString GetFileszUint();
    QToolButton *SetRecStart();
    QToolButton *SetRecStop();
private:
    QLineEdit *LEfilesz;
    QComboBox *CBfilesz;
    QToolButton *TBfilesz;
    QPushButton *TBchkRec_A;
    QPushButton *TBchkRec_B;
    QToolButton *TBRecStart;
    QToolButton *TBRecStop;
public slots:
    void slot_emitSetFileSz();
    void slot_emitchkRec_A();
    void slot_emitchkRec_B();
    void slot_emitRecStart();
    void slot_emitRecStop();
signals:
    void SIGNAL_RcdmType(int);
};

class replaymenu_init : public MenuUI
{
    Q_OBJECT
public:
    replaymenu_init();
    ~replaymenu_init();
    QPushButton* SetchkReplayA();
    QPushButton* SetchkReplayB();
    QPushButton* SetType1();
    QPushButton* SetType2();
    QPushButton* SetType3();
    QPushButton* SetType4();
    QPushButton* SetchkChnSelReplay();
    QPushButton* SetchkTimeSelReplay();
    QPushButton* SetchkChanSelReplay();
    QPushButton* SetchkTypeSelReplay();
    QComboBox* SetOutCom();
    QDateTimeEdit *SetStartTime();
    QDateTimeEdit *SetEndTime();
    QToolButton *SetTbReplay();
    QToolButton *SetTbPause();
    QToolButton *SetTbStop();
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
public slots:
    void slot_emitchkchnselreplay();
    void slot_emitchkReplayA();
    void slot_emitchkReplayB();
    void slot_emitcjkTimeSelReplay();
    void slot_emitchktypeSelReplay();
    void slot_emittype1();
    void slot_emittype2();
    void slot_emittype3();
    void slot_emittype4();
    void slot_emitchkChanSelReplay();
    void slot_emitreplay();
    void slot_emitpause();
    void slot_emitstop();
signals:
    void SIGNAL_RpmType(int);
};

class selectmenu_init : public MenuUI
{
    Q_OBJECT
public:
    selectmenu_init();
    ~selectmenu_init();
    QPushButton* SetchkReplayA();
    QPushButton* SetchkReplayB();
    QPushButton* SetchkChnSelReplay();
    QPushButton* SetchkTimeSelReplay();
    QPushButton* SetchkTypeSelReplay();
    QPushButton* SetType1();
    QPushButton* SetType2();
    QPushButton* SetType3();
    QPushButton* SetType4();
    QDateTimeEdit *SetStartTime();
    QDateTimeEdit *SetEndTime();
    QToolButton *SetDelFile();
    QToolButton *SetInqDir();
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
    QToolButton *TBdelfile;
    QToolButton *TBsearchfile;

private slots:
    void slot_emitchkchnselect();
    void slot_emitchkSelectA();
    void slot_emitchkSelectB();
    void slot_emitcjkTimeSelect();
    void slot_emitchktypeSelect();
    void slot_emittype1();
    void slot_emittype2();
    void slot_emittype3();
    void slot_emittype4();
    void slot_emitDeleteFile();
    void slot_emitInqDir();
signals:
    void SIGNAL_SmType(int);
};

class managemenu_init : public MenuUI
{
    Q_OBJECT
public:
    managemenu_init();
    ~managemenu_init();
    QPushButton *SetTimeSync();
    QDateTimeEdit *SetTimeEdit();
    QToolButton *SetSendTime();
    QToolButton *SetSelfCheck();
private:
    QTimer *pTime;
    QLabel *LAsettime;
    QPushButton *PBsync;
    QDateTimeEdit *DTEtime;
    QToolButton *TBsendtime;
    QToolButton *TBselfcheck;
signals:
    void SIGNAL_MgmType(int);
public slots:
    void slot_timesyncfunc();
    void slot_emitPBsync();
    void slot_emitTBsendtime();
    void slot_emitTBselfcheck();

};

class usermenu_init : public MenuUI
{
    Q_OBJECT
public:
    usermenu_init();
    ~usermenu_init();
    QString Getusrname();
    QString Getusrpswd();
    QToolButton *SetCreatUsr();
    QToolButton *SetModifyPswd();
    QToolButton *SetDelUsr();
private:
    QLabel *LAusername;
    QLabel *LAuserpswd;
    QLineEdit *LEusername;
    QLineEdit *LEuserpwsd;
    QToolButton *TBcreatuser;
    QToolButton *TBmodifypsdw;
    QToolButton *TBdeluser;
signals:
    void SIGNAL_UmType(int);
public slots:
    void slot_emitTBcreatusr();
    void slot_emitTBmodifypswd();
    void slot_emitTBdelusr();
};

class channelmenu_init : public MenuUI
{
    Q_OBJECT
public:
    channelmenu_init();
    ~channelmenu_init();
    QTableWidget *setchannel();
private:
    QLabel *LAmdftunnel;
    QPushButton *PBbacktodef;
    QPushButton *PBconfirm;
    QTableWidget *TWchannel;
public slots:
    void cmhandle_setchanneldefault();
    void cmhandle_confirm();
signals:
    void cmsignaltype(int);
};

class specfymenu_init : public MenuUI
{
    Q_OBJECT
public:
    specfymenu_init();
    ~specfymenu_init();
    QToolButton *SetCleardata();
    QToolButton *SetSoftreset();
    QToolButton *SetPoweroff();
    QToolButton *SetUpdate();
    QToolButton *SetSoftdistory();
private:
    QToolButton *TBcleardata;
    QToolButton *TBsoftreset;
    QToolButton *TBpoweroff;
    QToolButton *TBupdate;
    QToolButton *TBsoftdistory;
    QLabel *LAnotice;
public slots:
    void slot_emitcleardata();
    void slot_emitsoftreset();
    void slot_emitpoweroff();
    void slot_emitupdate();
    void slot_emitsoftdistory();
signals:
    void SIGNAL_SpmType(int);
};

#endif // MENUUI_H
