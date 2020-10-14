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
    QLineEdit* SetUserName();
    QLineEdit* SetUserPswd();
    QPushButton *SetRcdmChkA();
    QPushButton *SetRcdmChkB();
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
    QComboBox* SetRpmOutCom();
    QDateTimeEdit *SetRpmStartTime();
    QDateTimeEdit *SetRpmEndTime();
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
signals:
    void signals_pbtype(int);
    void signals_cmtype(int);
};

#endif // TOP_FORM_H
