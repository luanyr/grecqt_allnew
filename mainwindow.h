#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTime>
#include "recvthread.h"
#include "sendthread.h"
#include "qchartviewer.h"

//#define MY_DEBUG

const int noOfChartViewers = 2;

static QStringList SetDefaultChannelName()
{
    QStringList list;
    list << "光纤1" << "光纤2" << "光纤3" << "光纤4"
                  << "光纤5" << "光纤6" << "光纤7" << "光纤8"
                  << "光纤9" << "光纤10" << "光纤11" << "光纤12"
                  << "网络1" << "网络2" << "网络3";
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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void Mysleep(UINT32 usec){
        QTime reachtime = QTime::currentTime().addMSecs(usec);
        while(QTime::currentTime() < reachtime)
            QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }
    bool UIinit();      //界面初始化

signals:
    void Uimsg(QString uimsg); //ui消息显示信号

    void AutoConnectSignal();
    void HeartBeatSignal();

    void UserLoginSignal(QString strUserName, QString strPassword);
    void UserQuitSignal();
    void UserNewSignal(QString strUserName, QString strPassword);
    void UserChgPwSignal(QString strUserName, QString strPwNew);
    void UserDelSignal(QString strUserName, QString strPassword);

    void RecConSignal(UINT32 uCon);
    void RecModeSignal(UINT32 uRecMode);
    void WriteProtectSignal(QVariant fileter);
    void WriteProtectOffSignal(QVariant fileter);
    void UnInstStartSignal(QVariant fileter);
    void UnInstStopSignal();
    void PlaybackStartSignal(QVariant fileter);
    void PlaybackStopSignal();
    void PlaybackPauseSignal(UINT32 uPbPause);
    void DirSignal(QVariant fileter);
    void FileDelSignal(QVariant fileter);
    void CompressConSignal();
    void EncryptConSignal();
    void SetTimeSignal(QDateTime dateTime);

    void MBitSignal();
    void InqStatusSignal();
    void InqPUSignal(UINT32 uPU, UINT32 uRange);
    void ResetSignal();
    void PowerDownSignal();
    void SetfileszSignal(UINT32 uPU);
    void LogSignal();

    void WipeSignal();
    void ReportSignal();
    void SelfDestorySignal();
    void UnmountSignal(bool bshow);
    void MountSignal(bool bShow);
    void UpdateSignal();
    void SetCurVecSignal(double dblLon, double dblLat, double dblDir);

    void MenuActionSignal(UINT32 uCmdTm, QStringList listFile);

private slots:
    void TimerSyncTimeFunction();
    void TimerHeartBeatFunction();

    /*******net init****/
    void ConnectServer();   //连接服务器
    void HaveConnected();    //已经连接服务器
    void NotConnect();      //连接失败
    void displayError(QAbstractSocket::SocketError socketError); // 连接错误

    void UIStatusShow(QString msg);

    void UserLoginSlot(QByteArray data);
    void UserOperSlot(QByteArray data);
    void DirSlot(QList<CFileAttrib> list);
    void BitSlot(QByteArray data);
    void InqStatusSlot(QByteArray data);
    void WipeSlot(QByteArray data);

    void AllAckSlot(QByteArray head);
    void AllCmdUiSlot(QByteArray head, QByteArray data);

    void WorkStatusSlot(QByteArray data);
    void UpdateProgressSlot(bool bShow, UINT32 percent=0);

    void CustomMenuRequested(QPoint);
    void MenuFileSlot();

    void on_PbConnect_clicked();

    void on_PbDisConnect_clicked();

    void on_PbRec_clicked();

    void on_PbSetTime_clicked();

    void on_PbReset_clicked();

    //void on_PbInqStatus_clicked();

    void on_PbBit_clicked();

    void on_toolUserLogin_clicked();

    void on_toolUserQuit_clicked();

    void on_toolUserNew_clicked();

    void on_toolUserChgPw_clicked();

    void on_toolUserDel_clicked();

    void on_toolFormat_clicked();

    void on_toolSelfDest_clicked();

    void on_toolDelFile_clicked();

    void on_toolUpdate_clicked();

    void on_treeFile_itemSelectionChanged();

    void on_PbRecStop_clicked();

    void on_tabWidget_main_currentChanged(int index);

    void on_chkChnSelSelect_clicked();
    void on_chkTimeSelSelect_clicked();

    void on_PbInqDir_clicked();

    void on_PbBitSmall_clicked();

    void on_pbChannel_clicked();

    void on_pbChannelDefault_clicked();

    void on_toolUserQuit_2_clicked();

    void on_PbPowerDown_clicked();

    void on_PbReplay_clicked();

    void on_PbReplayPause_clicked();

    void on_PbReplayStop_clicked();

    void on_chkChnSelReplay_clicked();

    void on_chkTimeSelReplay_clicked();

    void on_chkTypeSelSelect_clicked();

    void on_chkTypeSelReplay_clicked();

    void on_chkChanSelReplay_clicked();

    void on_SetFilesz_clicked();

private:
    Ui::MainWindow *ui;

    UINT32 m_uRetry;
    BOOL m_bManual;
    QTcpSocket *m_tcpClient; //tcp 客户端套接字

    QMutex m_mutexFile;
    BOOL m_bConnected;
    BOOL m_bLogined;
    BOOL m_bAlive;

    BOOL m_bNeedRecMode;
    BOOL m_bMounted;
    UINT m_uMountMode;

    RecvThread *m_pThrdRecv;
    SendThread *m_pThrdSend;

    CStoreInfo m_infoStore;

    QChartViewer *m_chartViewers[noOfChartViewers];

    QLabel *m_labelStatUser;
    QLabel *m_labelStatWork;
    QPushButton *m_pbStatNet;
    QProgressBar *m_pProgress;
    QMenu *m_pMenu;

    QStringList m_listStrLink;
    QList<QLabel*> m_listStatLink;
    QList<QPushButton*> m_listChkRec;
    QList<QPushButton*> m_listChkReplay;
    QList<QPushButton*> m_listChkTypeReplay;
    QList<QPushButton*> m_listChkDownload;
    QList<QPushButton*> m_listChkTypeDownload;
    QList<QPushButton*> m_listChkSelect;
    QList<QPushButton*> m_listChkTypeSelect;
    QStringList m_listStrBit;
    QStringList m_listStrFile;
    QStringList m_listStrChannel;

    QList<CFileAttrib> m_listFile;

    QVariant ReadLimitReplay();
    QVariant ReadLimitDownload();
    QVariant ReadLimitSelect();

    void StartPageInit();
    void RecPageInit();
    void ReplayPageInit();
    void DownloadPageInit();
    void SelectPageInit();
    void OtherPageInit();
    void ChannelPageInit();

    void FilePageInit();

    void uiChgToolEnable(bool bEnable=true);
    void uiChgBizEnable(bool bEnable=true);
    void FunInIdleEnable(bool bEnable=true);
    void StatusbarInit();
    void StatusTableInit();
    void StatusChartInit();
    void BitListInit();

    void DownloadFilterSave();
    void SelectFilterSave();
    void ReplayFilterSave();

    void EnableReplaySel(bool bEnable);
    void EnableDownloadSel(bool bEnable);

    void ChartUpdate(int idx, double dblUsed, double dblUsable);

    void InsertTreeItem(CFileAttrib *pfileAttribSrc, INT32 nTier, QTreeWidgetItem *pParent);

    void UpdateChannelName();
};

#endif // MAINWINDOW_H
