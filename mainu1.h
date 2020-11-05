#ifndef MAINU1_H
#define MAINU1_H

#include <QWidget>
#include "headers.h"
#include "top_form.h"
namespace Ui {
class MainU1;
}
const int noOfChartViewers = 2;

static QStringList SetDefaultChannelName()
{
    QStringList list;
    list << QObject::tr("光纤1") << QObject::tr("光纤2") << QObject::tr("光纤3") << QObject::tr("光纤4")
                  << QObject::tr("光纤5") << QObject::tr("光纤6") << QObject::tr("光纤7") << QObject::tr("光纤8")
                  << QObject::tr("光纤9") << QObject::tr("光纤10") << QObject::tr("光纤11") << QObject::tr("光纤12")
                  << QObject::tr("网络1") << QObject::tr("网络2") << QObject::tr("网络3");
    return list;
}

class CStoreInfo
{
public:
    CStoreInfo()
    {
        QSettings settings_ini("./RecPara.ini", QSettings::IniFormat);
        QSettings settings("RW Soft", "RecCtrl");

        settings.beginGroup("StoreInfo");
        settings_ini.beginGroup("LoginInfo");

        m_szIP = settings_ini.value("ServerIP", "192.168.1.100").toString();
        m_uPort = settings_ini.value("ServerPort", 5025).toInt();
        m_strCurUser = settings_ini.value("User", "admin").toString();
        m_strCurPw = settings_ini.value("Pw", "123456").toString();
        m_bAutoLogin = settings_ini.value("AutoLogin",1).toInt();
        m_bRememberPw = settings_ini.value("RemPw",1).toInt();

        m_uRecCon = settings.value("RecCon",0).toUInt();

        m_filterReplay.uValid = settings.value("filterReplay.uValid",0).toUInt();
        m_filterReplay.uChannel = settings.value("filterReplay.uChannel",0).toUInt();
        m_filterReplay.vec1.dblLon = settings.value("filterReplay.vec1.dblLon",0).toDouble();
        m_filterReplay.vec1.dblLat = settings.value("filterReplay.vec1.dblLat",0).toDouble();
        m_filterReplay.vec1.dblDir = settings.value("filterReplay.vec1.dblDir",0).toDouble();
        m_filterReplay.vec2.dblLon = settings.value("filterReplay.vec2.dblLon",0).toDouble();
        m_filterReplay.vec2.dblLat = settings.value("filterReplay.vec2.dblLat",0).toDouble();
        m_filterReplay.vec2.dblDir = settings.value("filterReplay.vec2.dblDir",0).toDouble();
        m_filterReplay.timeBeg = settings.value("filterReplay.timeBeg",0).toDateTime();
        m_filterReplay.timeEnd = settings.value("filterReplay.timeEnd",0).toDateTime();
        m_filterReplay.uRange = settings.value("filterReplay.uRange",0).toUInt();


        m_filterSelect.uValid = settings.value("filterSelect.uValid",0).toUInt();
        m_filterSelect.uChannel = settings.value("filterSelect.uChannel",0).toUInt();
        m_filterSelect.vec1.dblLon = settings.value("filterSelect.vec1.dblLon",0).toDouble();
        m_filterSelect.vec1.dblLat = settings.value("filterSelect.vec1.dblLat",0).toDouble();
        m_filterSelect.vec1.dblDir = settings.value("filterSelect.vec1.dblDir",0).toDouble();
        m_filterSelect.vec2.dblLon = settings.value("filterSelect.vec2.dblLon",0).toDouble();
        m_filterSelect.vec2.dblLat = settings.value("filterSelect.vec2.dblLat",0).toDouble();
        m_filterSelect.vec2.dblDir = settings.value("filterSelect.vec2.dblDir",0).toDouble();
        m_filterSelect.timeBeg = settings.value("filterSelect.timeBeg",0).toDateTime();
        m_filterSelect.timeEnd = settings.value("filterSelect.timeEnd",0).toDateTime();
        m_filterSelect.uRange = settings.value("filterSelect.uRange",0).toUInt();

        m_vecCur.dblLon = settings.value("vecCur.dblLon").toDouble();
        m_vecCur.dblLat = settings.value("vecCur.dblLat").toDouble();
        m_vecCur.dblDir = settings.value("vecCur.dblDir").toDouble();

        m_listStrChannel = SetDefaultChannelName();
        for(int i=0; i<m_listStrChannel.count(); i++)
        {
            QString t = settings.value("channelName"+QString::number(i, 10)).toString();
            if(!t.isEmpty()) m_listStrChannel.replace(i, t);
        }

        settings.endGroup();

        //qDebug("Settings read.\nServerIP=%s\nServerPort=%d\nm_strCurUser=%s\nm_strCurPw=%s\nm_bAutoLogin=%d\nm_bRememberPw=%d\n",
        //       qPrintable(m_szIP), m_uPort, qPrintable(m_strCurUser), qPrintable(m_strCurPw), m_bAutoLogin, m_bRememberPw);
    }
    ~CStoreInfo()
    {
        //QSettings settings("RecPara.ini",QSettings::IniFormat);
        QSettings settings("RW Soft", "RecCtrl");

        settings.beginGroup("StoreInfo");

        settings.setValue("ServerIP", m_szIP);
        settings.setValue("ServerPort", QString::number(m_uPort,10));
        settings.setValue("User", m_strCurUser);
        settings.setValue("Pw", m_strCurPw);
        settings.setValue("AutoLogin", QString::number(m_bAutoLogin,10));
        settings.setValue("RemPw", QString::number(m_bRememberPw,10));

        settings.setValue("RecCon", QString::number(m_uRecCon,10));

        settings.setValue("filterReplay.uValid", QString::number(m_filterReplay.uValid,10));
        settings.setValue("filterReplay.uChannel", QString::number(m_filterReplay.uChannel,10));
        settings.setValue("filterReplay.vec1.dblLon", QString::number(m_filterReplay.vec1.dblLon, 'f', 6));
        settings.setValue("filterReplay.vec1.dblLat", QString::number(m_filterReplay.vec1.dblLat, 'f', 6));
        settings.setValue("filterReplay.vec1.dblDir", QString::number(m_filterReplay.vec1.dblDir, 'f', 6));
        settings.setValue("filterReplay.vec2.dblLon", QString::number(m_filterReplay.vec2.dblLon, 'f', 6));
        settings.setValue("filterReplay.vec2.dblLat", QString::number(m_filterReplay.vec2.dblLat, 'f', 6));
        settings.setValue("filterReplay.vec2.dblDir", QString::number(m_filterReplay.vec2.dblDir, 'f', 6));
        settings.setValue("filterReplay.timeBeg", m_filterReplay.timeBeg.toString("yyyy-MM-dd HH:mm:ss"));
        settings.setValue("filterReplay.timeEnd", m_filterReplay.timeEnd.toString("yyyy-MM-dd HH:mm:ss"));
        settings.setValue("filterReplay.uRange", QString::number(m_filterReplay.uRange,10));

        settings.setValue("filterDownload.uValid", QString::number(m_filterDownload.uValid,10));
        settings.setValue("filterDownload.uChannel", QString::number(m_filterDownload.uChannel,10));
        settings.setValue("filterDownload.vec1.dblLon", QString::number(m_filterDownload.vec1.dblLon, 'f', 6));
        settings.setValue("filterDownload.vec1.dblLat", QString::number(m_filterDownload.vec1.dblLat, 'f', 6));
        settings.setValue("filterDownload.vec1.dblDir", QString::number(m_filterDownload.vec1.dblDir, 'f', 6));
        settings.setValue("filterDownload.vec2.dblLon", QString::number(m_filterDownload.vec2.dblLon, 'f', 6));
        settings.setValue("filterDownload.vec2.dblLat", QString::number(m_filterDownload.vec2.dblLat, 'f', 6));
        settings.setValue("filterDownload.vec2.dblDir", QString::number(m_filterDownload.vec2.dblDir, 'f', 6));
        settings.setValue("filterDownload.timeBeg", m_filterDownload.timeBeg.toString("yyyy-MM-dd HH:mm:ss"));
        settings.setValue("filterDownload.timeEnd", m_filterDownload.timeEnd.toString("yyyy-MM-dd HH:mm:ss"));
        settings.setValue("filterDownload.uRange", QString::number(m_filterDownload.uRange,10));

        settings.setValue("filterSelect.uValid", QString::number(m_filterSelect.uValid,10));
        settings.setValue("filterSelect.uChannel", QString::number(m_filterSelect.uChannel,10));
        settings.setValue("filterSelect.vec1.dblLon", QString::number(m_filterSelect.vec1.dblLon, 'f', 6));
        settings.setValue("filterSelect.vec1.dblLat", QString::number(m_filterSelect.vec1.dblLat, 'f', 6));
        settings.setValue("filterSelect.vec1.dblDir", QString::number(m_filterSelect.vec1.dblDir, 'f', 6));
        settings.setValue("filterSelect.vec2.dblLon", QString::number(m_filterSelect.vec2.dblLon, 'f', 6));
        settings.setValue("filterSelect.vec2.dblLat", QString::number(m_filterSelect.vec2.dblLat, 'f', 6));
        settings.setValue("filterSelect.vec2.dblDir", QString::number(m_filterSelect.vec2.dblDir, 'f', 6));
        settings.setValue("filterSelect.timeBeg", m_filterSelect.timeBeg.toString("yyyy-MM-dd HH:mm:ss"));
        settings.setValue("filterSelect.timeEnd", m_filterSelect.timeEnd.toString("yyyy-MM-dd HH:mm:ss"));
        settings.setValue("filterSelect.uRange", QString::number(m_filterSelect.uRange,10));

        settings.setValue("vecCur.dblLon", QString::number(m_vecCur.dblLon, 'f', 6));
        settings.setValue("vecCur.dblLat", QString::number(m_vecCur.dblLat, 'f', 6));
        settings.setValue("vecCur.dblDir", QString::number(m_vecCur.dblDir, 'f', 6));

        for(int i=0; i<m_listStrChannel.count(); i++)
        {
            settings.setValue("channelName"+QString::number(i, 10), m_listStrChannel.at(i));
        }

        settings.endGroup();

        //qDebug("Settings saved.\nServerIP=%s\nServerPort=%d\nm_strCurUser=%s\nm_strCurPw=%s\nm_bAutoLogin=%d\nm_bRememberPw=%d\n",
        //       qPrintable(m_szIP), m_uPort, qPrintable(m_strCurUser), qPrintable(m_strCurPw), m_bAutoLogin, m_bRememberPw);
    }

    QString m_strCurUser;
    QString m_strCurPw;
    BOOL m_bAutoLogin;
    BOOL m_bRememberPw;

    QString m_szIP;
    UINT32 m_uPort;

    UINT32 m_uRecCon;

    T_FILTER m_filterReplay;
    T_FILTER m_filterDownload;
    T_FILTER m_filterSelect;

    T_VEC  m_vecCur;

    QStringList m_listStrChannel;
};

class myTabWidget : public QTabWidget
{
     Q_OBJECT
protected:
    void mouseDoubleClickEvent(QMouseEvent* e);
public:
    myTabWidget(QWidget* parent = 0);
signals:
    void signal_addwidget(int index);
};


class MainU1 : public QWidget
{
    Q_OBJECT

public:
    explicit MainU1(QWidget *parent = 0);
    ~MainU1();

private:
    Ui::MainU1 *ui;
    QTabWidget *TW_displayUI;
    Top_Form *tf;
    QPushButton *PB_addwidget;
    QTcpSocket *m_tcpclient;
    RecvThread *m_pThrdRecv;
    SendThread *m_pThrdSend;
//    QChartViewer *m_chartViewers[2];
    UINT32 m_uRetry;
    QMutex m_mutexFile;
    BOOL m_connectstatus;
    BOOL m_loginstatus;
    BOOL m_bLogined;
    BOOL m_bNeedRecMode;
    BOOL m_bManual;
    BOOL m_bAlive;
    BOOL m_bMounted;
    UINT m_uMountMode;
    CStoreInfo m_infoStore;
    QMenu *m_pMenu;
    QLabel *m_labelStatUser;
    QStringList m_listStrBit;
    QStringList m_listStrFile;
    QStringList m_ListStrChannel;
    QStringList m_listStrLink;
    QList<QLabel*> m_listStatLink;
    QList<QPushButton*> m_listChkRec;
    QList<QPushButton*> m_listChkReplay;
    QList<QPushButton*> m_listChkTypeReplay;
    QList<QPushButton*> m_listChkDownload;
    QList<QPushButton*> m_listChkTypeDownload;
    QList<QPushButton*> m_listChkSelect;
    QList<QPushButton*> m_listChkTypeSelect;
    QList<CFileAttrib> m_listFile;
    QChartViewer *m_chartViewers[noOfChartViewers];
    void UiInit();
    void addwidget();
    void connectserver();
    void disconnectserver();
    void displayError();
    void userlogin();
    void userlogout();
    void UpdateChannelName();
    void ChannelPageInit();
    void StatusTableInit();
    void StartPageInit();
    void BitListInit();
    void RecPageInit();
    void ReplayPageInit();
    void SelectPageInit();
    void FilePageInit();
    void OtherInit();
    void cmsetdefault();
    void cmconfirm();
    void uiChgToolEnable(bool bEnable);
    void uiChgBizEnable(bool bEnable);
    void FunInIdleEnable(bool bEnable);

    void ChartUpdate(int idx, double dblUsed, double dblUsable);
    void StatusChartInit();
    void InsertTreeItem(CFileAttrib *pfileAttribSrc, INT32 nTier, QTreeWidgetItem *pParent);
    QVariant ReadLimitReplay();
    QVariant ReadLimitSelect();
    void TimerSyncTimeFunction();
    void Mysleep(UINT32 usec){
        QTime reachtime = QTime::currentTime().addMSecs(usec);
        while(QTime::currentTime() < reachtime)
            QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }
//    void ChartUpdate(int idx, double dblUsed, double dblUsable);
signals:
    void UImsg(QString msg);
    void Signal_Usrlogin(QString username, QString userpwsd);
    void Signal_Usrlogout();
    void Signal_statnet(int);
    void Signal_InqStatus();
    void SetfileszSignal(UINT32 uPU);
    void RecConSignal(UINT32 uCon);
    void RecModeSignal(UINT32 uRecMode);
    void PlaybackStartSignal(QVariant fileter);
    void PlaybackPauseSignal(UINT32 uPbPause);
    void PlaybackStopSignal();
    void FileDelSignal(QVariant fileter);
    void DirSignal(QVariant fileter);
    void SetTimeSignal(QDateTime dateTime);
    void MBitSignal();
    void UserNewSignal(QString strUserName, QString strPassword);
    void UserChgPwSignal(QString strUserName, QString strPwNew);
    void UserDelSignal(QString strUserName, QString strPassword);
    void WipeSignal();
    void UpdateSignal();
    void ResetSignal();
    void SelfDestorySignal();
    void PowerDownSignal();
    void MountSignal(bool bShow);
    void UnmountSignal(bool bshow);
    void HeartBeatSignal();
    void WriteProtectSignal(QVariant fileter);
    void WriteProtectOffSignal(QVariant fileter);
    void UnInstStartSignal(QVariant fileter);
    void UnInstStopSignal();
    void CompressConSignal();
    void EncryptConSignal();
    void InqPUSignal(UINT32 uPU, UINT32 uRange);
    void LogSignal();
    void ReportSignal();
    void SetCurVecSignal(double dblLon, double dblLat, double dblDir);
    void MenuActionSignal(UINT32 uCmdTm, QStringList listFile);
    void AutoConnectSignal();
private slots:
    void TimerHeartBeatFunction();
    void slot_havaconnected();
    void slot_havedisconnected();
    void slot_UIstatusshow(QString msg);
    void slot_userlogin(QByteArray data);
    void slot_inqstatus(QByteArray data);
    void rpmchkTimeSelReplay();
    void rpmchkTypeSelReplay();
    void rpmchkChanSelReplay();
    void rpmchkChnSelReplay();
    void rpmchkReplayA();
    void rpmchkReplayB();
    void rpmtype1();
    void rpmtype2();
    void rpmtype3();
    void rpmtype4();
    void rpmreplaystart();
    void rpmreplaystop();
    void rpmreplaypause();
    void slot_slcmChkchnSelSelect();
    void slot_slcmChkTimeSelSelect();
    void slot_slcmChkTypeSelSelect();
    void slot_slcmSelectA();
    void slot_slcmSelectB();
    void slot_slcmType1();
    void slot_slcmType2();
    void slot_slcmType3();
    void slot_slcmType4();
    void slot_slcmdeletefile();
    void slot_slcminqdir();
    void slot_rcdmSetFilesz();
    void slot_rcdmReplayA();
    void slot_rcdmReplayB();
    void slot_rcdmRecordStart();
    void slot_rcdmRecordEnd();
    void slot_mgmtimesync();
    void slot_mgmsendtime();
    void slot_mgmselfcheck();
    void slot_umcreatusr();
    void slot_ummodifypswd();
    void slot_umdelusr();
    void slot_spmcleardata();
    void slot_spmsoftreset();
    void slot_spmpoweroff();
    void slot_spmupdate();
    void slot_spmsoftdistory();
    void slot_cutrwitemselectionchange();
    void slot_cupbbitsmall();
    void AllAckSlot(QByteArray head);
    void AllCmdUiSlot(QByteArray, QByteArray);
    void UpdateProgressSlot(bool bShow, UINT32 percent=0);
    void DirSlot(QList<CFileAttrib> list);
    void BitSlot(QByteArray);
    void WipeSlot(QByteArray data);
    void WorkStatusSlot(QByteArray data);
    void MenuFileSlot();
    void CustomMenuRequested(QPoint);
    void UserOperSlot(QByteArray data);
public slots:
    void slot_handlesignal(int type);
    void slot_closeapp();
    void slot_handlercdmsignal(int);
    void slot_handlecmsignals(int);
    void slot_handlerpmsignals(int);
    void slot_handlesmsignals(int);
    void slot_handlemgmsignals(int);
    void slot_handleumsignals(int);
    void slot_handlespmsignals(int);
    void slot_handlecusignals(int);
    //void slot_handlerpmsignal(int);
};

#endif // MAINU1_H
