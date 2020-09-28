#ifndef RECVTHREAD_H
#define RECVTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QFile>
#include <QFileDialog>
#include <QDateTime>
#include <QDebug>
#include "spark_def.h"
#include "rw29.h"
#include "cmi_pkt.h"
#include "myhelper.h"

class CBulkData
{
public:
    CBulkData() { Init(); }
    ~CBulkData() { Fini(); }
    void Init()
    {
        nCmd = 0;
        pBuffer = NULL;
        nTotalLen = nCurrentLen = 0l;
    }
    void Fini()
    {
        if (pBuffer)
            delete[] pBuffer;
        Init();
    }
    BOOL Allocate(int nLen)
    {
        if(nLen<=0) return false;
        if (IsAllocated())
        {
            Destroy();
        }
        pBuffer = new BYTE[nLen];
        nTotalLen = nLen;
        nCurrentLen = 0;
        return true;
    }
    void Destroy() { Fini(); }
    BOOL ProcessData(BYTE* pData, ULONG nOffset, ULONG nLen)
    {
        if(nLen<=0) return false;
        Q_ASSERT(pBuffer); // must be allocated before
        Q_ASSERT(nOffset + nLen <= nTotalLen);
        Q_ASSERT(nCurrentLen + nLen <= nTotalLen);
        memcpy(pBuffer + nOffset, pData, nLen);
        nCurrentLen += nLen;
        return true;
    }
    BOOL IsDataReady() { return(nCurrentLen == nTotalLen); }
    BOOL IsAllocated() { return (nTotalLen > 0); }
    BYTE* GetData(ULONG* pLen)
    {
        if (!IsDataReady())
            return NULL;
        if (pLen)
            *pLen = nTotalLen;
        return pBuffer;
    }
private:
    int		nCmd;
    BYTE*	pBuffer;
    ULONG	nTotalLen;
    ULONG	nCurrentLen;
};

class CFileAttrib
{
public:
    CFileAttrib()
    {
        nFileLenInk = 0;
    }
    ~CFileAttrib() {}
    BOOL IsSameDate(CFileAttrib* that)
    {
        return (this->tmCreate.date() == that->tmCreate.date());
    }
    BOOL IsSameChannel(CFileAttrib* that)
    {
        return (0 == this->strChName.compare(that->strChName));
    }
    BOOL IsSameDtype(CFileAttrib* that)
    {
        return (this->nDataType == that->nDataType);
    }
    QDateTime ConvertDateTime(QString str)
    {
        QDateTime t = QDateTime::fromString(str, "yyMMddHHmmss");
        QDate d = t.date();
        if(d.year()<2000)   //识别出来的是1900年-1999
            t.setDate(QDate(d.year()+100, d.month(), d.day()));
        return t;
    }
    bool ConvertFromFilename(QString strFilename)
    {
        this->tmCreate = ConvertDateTime(strFilename.section('_', 0, 0));
        this->strChName = strFilename.section('_', 3, 3);
        this->nDataType = strFilename.section('_', 4, 4).toInt();
        UINT32 flag = strFilename.section('_', 5, 5).toUInt();
        bProtected = flag & CMI_FILEFLAG__WP;
        bCompressed = flag & CMI_FILEFLAG__CMP;
        bEncryped = flag & CMI_FILEFLAG__ENC;
        bMarked = flag & CMI_FILEFLAG__MARK;

        this->strFilename = strFilename;

        return tmCreate.isValid() && tmCreate.date().year()<=2099 && tmCreate.date().year()>=2000;
    }
    QString GetCreateDate()
    {
        return tmCreate.date().toString("yyyy/MM/dd");
    }
    QString GetCreateDateTime()
    {
        return tmCreate.toString("yyyy/MM/dd HH:mm:ss");
    }
    QString GetDataType()
    {
        return "类型"+QString::number(nDataType, 10);
    }
    QString GetChannelName()
    {
        return strChName;
    }
    QString GetFullFilename()
    {
        return strFilename;
    }
    QString GetSimpleFilename()
    {
        return strFilename;
    }
    QString GetFileLength()
    {
        return QString::number(nFileLenInk, 10)+" KB";
    }
    void SetFileLength(UINT64 len)
    {
        nFileLenInk = (UINT32)(len >> 10);
    }
public:
    BOOL bProtected;
    BOOL bCompressed;
    BOOL bEncryped;
    BOOL bMarked;

    const CFileAttrib& operator=(const CFileAttrib& src)
    {
        bProtected = src.bProtected;
        bCompressed = src.bCompressed;
        bEncryped = src.bEncryped;
        bMarked = src.bMarked;

        strFilename = src.strFilename;
        tmCreate = src.tmCreate;
        strChName = src.strChName;
        nDataType = src.nDataType;
        nFileLenInk = src.nFileLenInk;

        return *this;
    }

private:
    QString strFilename;
    QDateTime tmCreate;
    QString strChName;
    int nDataType;
    UINT32 nFileLenInk;
};

typedef struct tagDirInfo
{
    QList<CFileAttrib> aryFileAttrib;
} T_DIRINFO;

class RecvThread : public QThread
{
    Q_OBJECT
public:
    explicit RecvThread(QObject *parent = 0);
    ~RecvThread();

    void SetSocketWord(QTcpSocket *socket){
        m_tcpClient = socket;
        //m_tcpClient = new QTcpSocket(this);
        //m_tcpClient->setSocketDescriptor(socket->socketDescriptor());
    }

    void run();
    void stop();

signals:
    void RecvMsgToui(QString msg); //发送接受线程的消息到界面
    void RecvProcessToui(bool bShow, UINT32 value); //更新界面的进度条

    /********接收消息发给界面的信号*********/
    void UserLoginSignal(QByteArray data);
    void UserOperSignal(QByteArray data);
    void BitSignal(QByteArray data);
    void InqStatusSignal(QByteArray data);
    void WipeSignal(QByteArray data);
    void WorkStatusSignal(QByteArray data);

    /********接收消息发给界面和发送线程的信号*********/
    void AllAckSignal(QByteArray head);
    void AllCmdUiSignal(QByteArray head, QByteArray data);
    void AllCmdSendSignal(QByteArray head, QByteArray data);
    void SetSysBizTypeSignal(QByteArray data);

    //对块数据加工后送给主窗口
    void DirSignal(QList<CFileAttrib> list);

    void TransBulkSignal(QByteArray data);

public slots:
    void RecvStartSlot(); //接收线程开始的信号

private slots:
    void TransBulkSlot(QByteArray data);

protected:
    /************************************tcp接收过程*************************************/
    void RecvPacket(); //接收数据的通道   important;
    /**********************************************************************************/

private:
    QTcpSocket *m_tcpClient;

    bool m_bShouldStop;

    UINT32 m_nByteRecvTotal;
    UINT32 m_nByteRecv;     //已经接收到的数据

    UINT32 m_nOffset;
    UINT8 m_bufIn[RW_MAX_PKTLEN];    //定义的数据缓存区
    UINT8 m_bufTemp[RW_MAX_PKTLEN];  //定义的数据缓存区

    CBulkData m_cBulkStubs[0x100];
    T_DIRINFO m_retDir;

    UINT16 CalCheckCode(UINT8 *p, UINT32 nBytes);

    bool shouldStop();
    void ParseData(rw_raw_hdr_t *pHead, UINT8 *buf, UINT32 nBytes);

};

#endif // RECVTHREAD_H
