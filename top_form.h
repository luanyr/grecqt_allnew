#ifndef TOP_FORM_H
#define TOP_FORM_H

#include <QWidget>
#include "menuui.h"
#include "headers.h"
#include "contentui.h"
namespace Ui {
class Top_Form;
}

class Top_Form : public QWidget
{
    Q_OBJECT
public:
    explicit Top_Form(QWidget *parent = 0);
    ~Top_Form();
    void UIShowMsg(QString msg);
    QString Getipaddr();
    QString Getipport();
    QString Getusrname();
    QString Getuserpswd();
    void CuStatusNet(int);
    void CuStatusUsr(QString, int);
    QTableWidget *setchannel();
    QTableWidget *SetTlwDevSta();
    QListWidget *SetLwListBit();
    QLineEdit* SetTcpAddr();
    QLineEdit* SetTcport();
    QToolButton* SetSmConnect();
    QToolButton* SetSmDisconnect();
    QToolButton* SetSmLogin();
    QToolButton* SetSmLogout();
    QLineEdit* SetUserName();
    QLineEdit* SetUserPswd();
    QPushButton *SetRcdmChkA();
    QPushButton *SetRcdmChkB();
    UINT32  GetRcdmFilesz();
    QString GetRcdmFileszUint();
    QToolButton *SetRcdmRecStart();
    QToolButton *SetRcdmRecStop();
    QToolButton *SetRcdmFilesz();
    QPushButton *SetRpmChkA();
    QPushButton *SetRpmChkB();
    QPushButton *SetRpmType1();
    QPushButton *SetRpmType2();
    QPushButton *SetRpmType3();
    QPushButton *SetRpmType4();
    QPushButton *SetRpmChkchnselreplay();
    QPushButton *SetRpmchkchansetreplay();
    QPushButton *SetRpmchktypesetreplay();
    QPushButton *SetRpmchktimesetreplay();
    QComboBox *SetRpmOutCom();
    QDateTimeEdit *SetRpmStartTime();
    QDateTimeEdit *SetRpmEndTime();
    QToolButton *SetRpmReplay();
    QToolButton *SetRpmPause();
    QToolButton *SetRpmStop();
    QPushButton *SetSlcmChkA();
    QPushButton *SetSlcmChkB();
    QPushButton *SetSlcmChnSelect();
    QPushButton *SetSlcmTimeSelect();
    QPushButton *SetSlcmTypeSelect();
    QDateTimeEdit *SetSlcmStartTime();
    QDateTimeEdit *SetSlcmEndTime();
    QPushButton *SetSlcmType1();
    QPushButton *SetSlcmType2();
    QPushButton *SetSlcmType3();
    QPushButton *SetSlcmType4();
    QToolButton *SetSlcmDelFile();
    QToolButton *SetSlcmInqDir();
    QTableWidget *SetCuTableFile();
    QTreeWidget *SetCuTreeFile();
    QPushButton *SetMgmTimeSync();
    QDateTimeEdit *SetMgmTimeEdit();
    QToolButton *SetMgmSendTime();
    QToolButton *SetMgmSelfCheck();
    QPushButton *SetCuBitsmall();
    QListWidget *SetCuListBit();
    QString GetUmUsrname();
    QString GetUmUsrpswd();
    QToolButton *SetUmCreatUsr();
    QToolButton *SetUmModifyPswd();
    QToolButton *SetUmDelusr();
    QToolButton *SetSpmClearData();
    QToolButton *SetSpmSoftReset();
    QToolButton *SetSpmPoweroff();
    QToolButton *SetSpmUpdate();
    QToolButton *SetSpmSoftdistory();
    QProgressBar *SetCuProgress();
    QTabWidget *SetCuDevStaCap();
private:
    Ui::Top_Form *ui;
    startmenu_init      *sm;
    replaymenu_init     *rpm;
    recordmenu_init     *rcdm;
    selectmenu_init     *slcm;
    managemenu_init     *mgm;
    usermenu_init       *um;
    channelmenu_init    *cm;
    specfymenu_init     *spm;
    QComboBox   *CBXmenu;
    QTabWidget  *TW;
    ContentUI *cu;
    void Startmenu_Init();
    void Content_Init();
    void ptr_delete(void *ptr);
protected:
    QSize tbsize;
    QStringList menulist;
    QFont ft;
public slots:
    void menu_change(QString);
    void menu_distory();
    void menu_hidden();
    void slot_handlepbsignals(int);
    void slot_cmhandlesignals(int);
    void slot_rpmhandlesignals(int);
    void slot_rcdmhandlesignal(int);
    void slot_slcmhandlesignal(int);
    void slot_mgmhandlesignal(int);
    void slot_umhandlesignal(int);
    void slot_spmhandlesignal(int);
    void slot_cuhandlesignal(int);
signals:
    void signals_pbtype(int);
    void signals_cmtype(int);
    void signals_rpmtype(int);
    void signals_rcdmtype(int);
    void signals_smtype(int);
    void signals_mgmtype(int);
    void signals_umtype(int);
    void signals_spmtype(int);
    void signals_cutype(int);
};

#endif // TOP_FORM_H
