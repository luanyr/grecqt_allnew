#ifndef SENDTHREAD_H
#define SENDTHREAD_H

#include <QThread>
#include <QTime>
#include <QTcpSocket>
#include <QFile>
#include <QDataStream>
#include <QApplication>
#include <QMetaType>
#include "spark_def.h"
#include "rw29.h"
#include "cmi_pkt.h"
#include "myhelper.h"

#define TMCMD_DEL   (1)
#define TMCMD_PB    (2)
#define TMCMD_DL    (3)
#define TMCMD_WPON  (4)
#define TMCMD_WPOFF (5)
#define TMCMD_DIR   (6)

#define NUM_CHECKCMD    (256)

typedef struct tagVec
{
    double dblLon;
    double dblLat;
    double dblDir;
} T_VEC;

typedef struct tagFilter
{
    UINT32 uValid;
    UINT32 uChannel;
    QDateTime timeBeg;
    QDateTime timeEnd;
    UINT32 uRange;
    T_VEC vec1;
    T_VEC vec2;
    UINT32 uFileFlags;
    QString strFileName;
} T_FILTER;

Q_DECLARE_METATYPE(T_FILTER)

class CCmdStatus
{
public:
    enum enuCmdStatus {
        CMDST_INVALID   = 1,
        CMDST_SEND      = 2,
        CMDST_SUCCESS   = 3,
        CMDST_FAIL      = 4,
        CMDST_COMPLETED = 5,
        CMDST_ACKED     = 6
    };
    //存放循环记录的命令、反应时间和返回状态
    UINT16 uCmdId;
    UINT16 uCmdSeq;
    UINT32 uTimeBeg;  //单位：ms
    UINT32 uTimeEnd;  //单位：ms
    INT32 iTimeAck;  //单位：ms
    INT32 iTimeCompleted;  //单位：ms

    enum enuCmdStatus enStatus;

    const CCmdStatus& operator=(const CCmdStatus& src)
    {
        uCmdId = src.uCmdId;
        uCmdSeq = src.uCmdSeq;
        uTimeBeg = src.uTimeBeg;
        uTimeEnd = src.uTimeEnd;

        iTimeAck = src.iTimeAck;
        iTimeCompleted = src.iTimeCompleted;
        enStatus = src.enStatus;

        return *this;
    }
};

class CCmdRes
{
private:
    QList<CCmdStatus> listCmd;

    QTime m_t;

public:
    CCmdRes()
    {
        m_t.start();
    }
    INT32 CalAckTime(UINT16 &uCmdId, UINT16 uCmdSeq)
    {
        INT32 iRes = 0;
        if(uCmdSeq == 0) return iRes;    //序列号为0的不参与统计

        //反向找要快一些
        if(listCmd.isEmpty()) return iRes;    //队列空时返回

        INT32 iCount = listCmd.count();
        for(INT32 i=iCount-1; (i>=0 && i>iCount-128); i--)
        {
            CCmdStatus st = listCmd.at(i);
            if(uCmdSeq == st.uCmdSeq)
            {
                st.uTimeEnd = m_t.elapsed();
                st.iTimeAck = st.uTimeEnd - st.uTimeBeg;
                iRes = st.iTimeAck;
                st.enStatus = CCmdStatus::CMDST_ACKED;

                uCmdId = st.uCmdId;

                listCmd.replace(i, st);
                break;
            }
        }

        return iRes;
    }
    INT32 SetGenStatus(UINT16 uCmdId, UINT16 uCmdSeq, UINT32 uRes)
    {
        //uCmdId注意这个uCmdId是发出去的指令ID，不是接收回来的结果CMI_CMDR__ALL_CMDRESULT
        //应该在CMI_MSGR__ALL_CMDRESULT结构体中
        INT32 iRes = 0;
        if(uCmdSeq == 0) return iRes;    //序列号为0的不处理
        //反向找要快一些
        INT32 iCount = listCmd.count();
        for(INT32 i=iCount-1; (i>=0 && i>iCount-NUM_CHECKCMD); i--)//按1秒1个命令算，2048可以检查半小时的指令
        {
            CCmdStatus st = listCmd.at(i);
            if((uCmdId == st.uCmdId) &&
                    (uCmdSeq == st.uCmdSeq))
            {
                st.iTimeCompleted = m_t.elapsed() - st.uTimeBeg;
                st.enStatus = (uRes == 0)?CCmdStatus::CMDST_FAIL:CCmdStatus::CMDST_SUCCESS;
                listCmd.replace(i, st);

                iRes = i;
                break;
            }
        }

        return iRes;
    }
    INT32 SetSpeStatus(UINT16 uCmdSeq)
    {
        INT32 iRes = 0;
        if(uCmdSeq == 0) return iRes;    //序列号为0的不处理
        //反向找要快一些
        INT32 iCount = listCmd.count();
        for(INT32 i=iCount-1; (i>=0 && i>iCount-NUM_CHECKCMD); i--)//按1秒1个命令算，2048可以检查半小时的指令
        {
            CCmdStatus st = listCmd.at(i);
            if(uCmdSeq == st.uCmdSeq)
            {
                st.iTimeCompleted = m_t.elapsed() - st.uTimeBeg;
                st.enStatus = CCmdStatus::CMDST_COMPLETED;
                listCmd.replace(i, st);

                iRes = i;
                break;
            }
        }

        return iRes;
    }
    void AddCmd(UINT16 uCmdId, UINT16 uCmdSeq)
    {
        CCmdStatus status;
        status.uCmdId = uCmdId;
        status.uCmdSeq = uCmdSeq;
        status.uTimeBeg = m_t.elapsed();
        status.uTimeEnd = 0;
        status.iTimeAck = -1;
        status.enStatus = CCmdStatus::CMDST_SEND;

        listCmd.append(status);

        //过老的数据删除
        while(listCmd.count()>NUM_CHECKCMD)
        {
            listCmd.removeFirst();
        }
    }
    INT32 GetAckBuffer(INT32* buf, INT32 uMaxLen)
    {
        memset(buf, 0, uMaxLen*sizeof(INT32));
        INT32 idx = uMaxLen-1;
        INT32 iCount = listCmd.count();
        for(INT32 i=iCount-1; (i>=0 && i>iCount-NUM_CHECKCMD); i--)//按1秒1个命令算，2048可以检查半小时的指令
        {
            CCmdStatus st = listCmd.at(i);
            buf[idx--] = st.iTimeAck;
            if(idx<0) break;
        }

        return idx;
    }
    UINT32 GetCount()
    {
        return listCmd.count();
    }
    void Clear()
    {
        listCmd.clear();
    }
    CCmdStatus::enuCmdStatus GetPrevCmdStatus(UINT &uCmdRes, UINT uCmdInq = 0)
    {
        CCmdStatus::enuCmdStatus eRes = CCmdStatus::CMDST_INVALID;
        BOOL bFind = FALSE;
        INT32 iCount = listCmd.count();
        for(INT32 i=iCount-1; (i>=0 && i>iCount-NUM_CHECKCMD && !bFind); i--)//按1秒1个命令算，2048可以检查半小时的指令
        {
            CCmdStatus st = listCmd.at(i);
            if(uCmdInq == 0)
            {//如果uCmd为0表示只检查重点类型检查
                switch(st.uCmdId )
                {
                case RW_CMD__ADM_SELFDIAG:
                case RW_CMD__ADM_SOFTRESET:
                case CMI_CMD__STOR_OPERFILE:
                case CMI_CMD__STOR_LISTFILE:
                    eRes = st.enStatus;
                    uCmdRes = st.uCmdId;
                    bFind = TRUE;
                    break;
                case RW_CMD__ADM_INQSTATUS:
                case RW_CMD__MED_WIPESTOR:
                case RW_CMD__ADM_SELFDEST:
                case RW_CMD__BIZ_CTRLREC:
                case RW_CMD__USR_LOGIN:
                case RW_CMD__ADM_SYNCTIME:
                case CMI_CMD__BIZ_CTRLPLAY:
                case CMI_CMD__BIZ_CTRLMIG:
                case CMI_CMD__ADM_UPGSYS:
                case CMI_CMD__BIZ_OPERCHAN:
                case CMI_CMD__ADM_TRANSBULK:
                case CMI_CMD__STOR_OPERMED:
                default:
                    continue;
                    break;
                }
            }
            else
            {
                if(st.uCmdId == uCmdInq)
                {
                    eRes = st.enStatus;
                    uCmdRes = st.uCmdId;
                    bFind = TRUE;
                }
            }
        }

        return eRes;
    }
    BOOL WaitRes(UINT16 uCmd=0, UINT32 uOverTime=3000)
    {
        BOOL bRes = TRUE;
        UINT32 uStart = m_t.elapsed();
        UINT uCmdRes;
        CCmdStatus::enuCmdStatus eStatus;
        do
        {
            eStatus = GetPrevCmdStatus(uCmdRes, uCmd);
            if(m_t.elapsed()-uStart > uOverTime)
            {
                bRes = FALSE;
                break;
            }
        } while (eStatus==CCmdStatus::CMDST_SEND);
        return bRes;
    }
    BOOL CmdIsDone(UINT16 uCmd=0)
    {
        UINT uCmdRes;
        CCmdStatus::enuCmdStatus eStatus;
        eStatus = GetPrevCmdStatus(uCmdRes, uCmd);
        return (eStatus!=CCmdStatus::CMDST_SEND) && (eStatus!=CCmdStatus::CMDST_ACKED);
    }
};

class SendThread : public QThread
{
    Q_OBJECT
public:
    explicit SendThread(QObject *parent = 0);
    ~SendThread();

    void SetSocketWord(QTcpSocket *socket){
        m_tcpClient = socket;
        //m_tcpClient = new QTcpSocket();
        //m_tcpClient->setSocketDescriptor(socket->socketDescriptor());
    }
    void Run(void);

signals:
    void SendMsgToui(QString); //发送发送线程的消息到界面
    void SendProcessToui(bool bShow, UINT32 value); //更新界面的进度条

public slots:
/*********************************接收来自界面的消息信号槽************************************/
    void UserLoginSlot(QString strUserName, QString strPassword);
    void UserQuitSlot();
    void UserNewSlot(QString strUserName, QString strPassword);
    void UserChgPwSlot(QString strUserName, QString strPwNew);
    void UserDelSlot(QString strUserName, QString strPassword);

    void RecConSlot(UINT32 uCon);
    void RecModeSlot(UINT32 uRecMode);
    void WriteProtectSlot(QVariant filter);
    void WriteProtectOffSlot(QVariant filter);
    void UnInstStartSlot(QVariant filter);
    void UnInstStopSlot();
    void PlaybackStartSlot(QVariant filter);
    void PlaybackStopSlot();
    void PlaybackPauseSlot(UINT32 uPbPause);
    void DirSlot(QVariant filter);
    void FileDelSlot(QVariant filter);
    void CompressConSlot();
    void EncryptConSlot();
    void SetTimeSlot(QDateTime dateTime);
    void SetfileSzSlot(UINT32 filesz);

    void HeartBeatSlot();
    void BitSlot();
    void InqStatusSlot();
    void ResetSlot();
    void PowerDownSlot();
    void LogSlot();

    void WipeSlot();
    void ReportSlot();
    void SelfDestorySlot();
    void UpdateSlot();
    void UnmountSlot(bool bShow);
    void MountSlot(bool bShow);
    void SetCurVecSlot(double dblLon, double dblLat, double dblDir);

    void MenuActionSlot(UINT32 uCmdTm, QStringList listFile);

    /*********************************接收来自接收线程的消息信号槽************************************/
    void AllAckSlot(QByteArray head);
    void AllCmdSendSlot(QByteArray head, QByteArray data);
    void SetSysBizTypeSlot(QByteArray data);

private slots:
    void TimerFunction();

protected:
    /************************************tcp发送过程*************************************/
    UINT32 updateclientProgressChannel(UINT8 *,UINT32); //发送数据的通道

    void Mysleep(UINT32 sec){
        QTime reachtime = QTime::currentTime().addSecs(sec);
        while(QTime::currentTime() < reachtime)
            QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }

private:
    QTcpSocket *m_tcpClient;
    UINT32 m_uCmdSeq;

    BIZ_TYPE m_eSysBiztype;
    CCmdRes m_response;

    T_FILTER m_filterWp;
    T_FILTER m_filterPb;
    T_FILTER m_filterUn;
    T_FILTER m_filterDir;
    T_FILTER m_filterDel;

    QTimer *m_pTimer;
    QStringList m_listFile;
    UINT32 m_uCmdTm;

    BOOL SendPacket(void *pBody, UINT32 uSizes, UINT16 uCmd);
    BOOL SendFile(QString strFileName, UINT16 uCmd);

public:
    BOOL IsSysInBiz(BIZ_TYPE biz_type)
    {
        return(!!(m_eSysBiztype & biz_type));
    }

    void SetSysInBiz(BIZ_TYPE biz_type)
    {
        int oldtype = (int)m_eSysBiztype;
        oldtype |= (int)biz_type;
        m_eSysBiztype = (BIZ_TYPE)oldtype;
    }

    void SetSysBizType(BIZ_TYPE biz_type)
    {
        m_eSysBiztype = biz_type;
    }

    UINT32 GetAckBuffer(INT32* buf, INT32 uMaxLen)
    {
        return m_response.GetAckBuffer(buf, uMaxLen);
    }
    CCmdStatus::enuCmdStatus GetPrevCmdStatus(UINT &uCmdRes, UINT uCmdInq = 0)
    {
        return m_response.GetPrevCmdStatus(uCmdRes, uCmdInq);
    }
    BOOL WaitRes(UINT16 uCmd=0, UINT32 uOverTime=3000)
    {
        return m_response.WaitRes(uCmd, uOverTime);
    }
    BOOL CmdIsDone(UINT16 uCmd=0)
    {
        return m_response.CmdIsDone(uCmd);
    }
};

#endif // SENDTHREAD_H
