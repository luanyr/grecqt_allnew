#include "recvthread.h"

RecvThread::RecvThread(QObject *parent) :
    QThread(parent)
{
    m_nByteRecv = 0;
    m_nByteRecvTotal = 0;
    m_nOffset = 0;

    m_bShouldStop = false;

    connect(this, SIGNAL(TransBulkSignal(QByteArray)), this, SLOT(TransBulkSlot(QByteArray)));
}

RecvThread::~RecvThread()
{

}

void RecvThread::run()
{
    while(!shouldStop())
    {
        RecvPacket();
        msleep(1);
    }
}

bool RecvThread::shouldStop()
{
    return m_bShouldStop;
}

void RecvThread::stop()
{
    m_bShouldStop = true;
}

void RecvThread::RecvStartSlot()
{
    RecvPacket();
}

UINT16 RecvThread::CalCheckCode(UINT8 *p, UINT32 nBytes)
{
    unsigned short uRes;
    unsigned short *p16 = (unsigned short *)p;  //这里默认是对齐的
    UINT32 uTemp = 0;
    for(UINT32 i=0; i<nBytes/2; i++)  //长度默认是对齐的
    {
        uTemp += p16[i];
    }
    uRes = (unsigned short)(uTemp&0xFFFF);
    return uRes;
}

void RecvThread::RecvPacket()
{
    if (m_tcpClient->bytesAvailable() <= 0) {
        return;
    }

    UINT64 retval = 0;
    rw_raw_hdr_t *pHead = (rw_raw_hdr_t*)&m_bufTemp[0];
    UINT16 uCmd;

    uint sizeHead = sizeof(rw_raw_hdr_t);
    uint sizeTail = sizeof(rw_raw_tail_t);
    //先接收包头
//    qDebug("bytesAvailable = %d bytes  sizeof(rw_raw_hdr_t) = %d bytes\n", m_tcpClient->bytesAvailable(), sizeHead);
    if((m_nByteRecvTotal==0) && (m_tcpClient->bytesAvailable() >= sizeHead))
    {
        retval = m_tcpClient->read((char *)&m_bufTemp[0], sizeHead);
        //qDebug("head retval = %d bytes\n", retval);
        uCmd = pHead->cmd_id;
        if(uCmd == RW_CMDR__ALL_ACK)
        {   //ACK放这里是要准确测试时间，通知发送线程计算时间
            emit AllAckSignal(QByteArray::fromRawData((char*)pHead, sizeHead));
        }
        if((pHead->tag_start != RW_TAG_START))
        {
            //有错，读空缓存，容错处理
            m_tcpClient->readAll();
            //emit RecvMsgToui("错误：返回包起始码不符!");
        }
        else if(pHead->pkt_len<(sizeHead+sizeTail))
        {
            emit RecvMsgToui("错误：返回包长度有误!");
        }
        else
        {
            m_nByteRecv += retval;
            m_nByteRecvTotal = pHead->pkt_len;
        }
    }

    //接收包体和包尾
    if((m_nByteRecv < m_nByteRecvTotal) && (m_tcpClient->bytesAvailable() >= m_nByteRecvTotal-m_nByteRecv))   //接收不满数据长度时
    {
        retval = m_tcpClient->read((char*)&m_bufTemp[m_nByteRecv], m_nByteRecvTotal - m_nByteRecv);
        //qDebug("body retval = %d bytes\n", retval);

        m_nByteRecv  += retval;
        if(m_nByteRecv == m_nByteRecvTotal)   //接满数据长度时
        {
            rw_raw_tail_t *pTail = (rw_raw_tail_t *)&m_bufTemp[pHead->pkt_len-sizeof(rw_raw_tail_t)];
            if((pTail->tag_end != RW_TAG_END))
            {
                //有错，读空缓存，容错处理
                m_tcpClient->readAll();
                //emit RecvMsgToui("错误：返回包结束码有误!");
            }
            else if((pTail->csum != 0)
                    && pTail->csum != CalCheckCode((UINT8*)pHead, pHead->pkt_len-sizeof(rw_raw_tail_t)))
            {
                emit RecvMsgToui("错误：校验失败!");
            }
            else
            {
                //接收正常，复制有效数据到缓存
                UINT32 uLenValid = pHead->pkt_len-sizeof(rw_raw_hdr_t)-sizeof(rw_raw_tail_t);
                if(uLenValid>0)
                {
                    memcpy(&m_bufIn[m_nOffset], &m_bufTemp[sizeof(rw_raw_hdr_t)], uLenValid);
                    m_nOffset += uLenValid;
                }

                //如果包序号等于包数，则接收完成
                if(pHead->total_pkts <= pHead->pkt_num)
                {//合成完毕，可以开始处理数据
                    ParseData(pHead, m_bufIn, m_nOffset);
                }
            }
            m_nByteRecvTotal = 0;
            m_nByteRecv = 0;
            m_nOffset = 0;
        }
    }
}

void RecvThread::ParseData(rw_raw_hdr_t *pHead, UINT8 *buf, UINT32 nBytes)
{
    //qDebug("Received %d bytes. (0x%04x): %02x %02x %02x %02x\n", nBytes, pHead->cmd_id, buf[0], buf[1], buf[2], buf[3]);

    QByteArray head = QByteArray::fromRawData((char*)pHead, sizeof(rw_raw_hdr_t));
    QByteArray data;
    if(nBytes>0) data = QByteArray::fromRawData((char*)buf, nBytes);

    switch(pHead->cmd_id)
    {
    case RW_CMDR__ALL_ACK:
        break;
    case RW_CMDR__ADM_SELFDIAG:
        emit BitSignal(data);
        break;
    case RW_CMDR__ADM_INQSTATUS:
        emit InqStatusSignal(data);
        break;
    case RW_CMDR__MED_WIPESTOR:
        emit WipeSignal(data);
        break;
    case RW_CMDR__USR_LOGIN:
        emit UserLoginSignal(data);
        break;
    case CMI_CMD__ADM_TRANSBULK:
        emit TransBulkSignal(data);
        break;
    case CMI_MAKE_CMDR(CMI_CMD__USR_OPERUSR):
        emit UserOperSignal(data);
        break;
    case CMI_MAKE_CMDR(CMI_CMD__ADM_HEARTBEAT):
    {
        emit WorkStatusSignal(data);
        emit SetSysBizTypeSignal(data);
    }
        break;
    case CMI_CMDR__ALL_CMDRESULT:
        emit AllCmdUiSignal(head, data);
        break;
    }

    emit AllCmdSendSignal(head, data);
}

void RecvThread::TransBulkSlot(QByteArray data)
{
    cmi_msg_adm_transbulk *pMsgr = (cmi_msg_adm_transbulk *)data.data();
    if(pMsgr->magic != TRANSBULK_MAGIC)
    {
        emit RecvMsgToui(tr("收到错误的块数据文件头。"));
        return;
    }
    BOOL bRet = false;
    int nRawCmd = (pMsgr->cmd & 0xff);
    int nFlag = pMsgr->flag;

    CBulkData* pBulk = &m_cBulkStubs[nRawCmd];
    if (nFlag & TRANSBULK_FLAG__ABORT)
    {
        pBulk->Destroy();
        return;
    }

    if (nFlag & TRANSBULK_FLAG__START)
    {
        bRet = pBulk->Allocate((ULONG)pMsgr->bulk_sz);
        Q_ASSERT(bRet);
    }

    bRet = pBulk->ProcessData((BYTE*)(pMsgr->data), (ULONG)pMsgr->offset, (ULONG)pMsgr->length);
    Q_ASSERT(bRet);

    if (nFlag & TRANSBULK_FLAG__END)
    {
        if (!pBulk->IsDataReady())
        {
            emit RecvMsgToui(tr("收到的块数据信息不足。"));
        }
        else
        {
            ULONG nLen;
            BYTE* pData = pBulk->GetData(&nLen);
            char* p = (char*)pData;
            if (pData != NULL)
            {
                switch (pMsgr->cmd)
                {
                case CMI_MAKE_CMDR(CMI_CMD__STOR_LISTFILE):
                    m_retDir.aryFileAttrib.clear();

#if 0
                    //测试代码
                    BYTE temp[10240];
                {

                    pData = temp;
                    p = (char*)pData;
                    //数据拼装完成，开始解析目录
                    char* ptest = (char*)pData;
                    char *pTest1 = "170404213040_00000012-00002345-0033_36_CH01_1";
                    strcpy(ptest, pTest1);
                    ptest+=(SPK_MAX_FILE_NAMELEN + 1)+sizeof(uint64_t);
                    char *pTest11 = "170404213041_00000012-00002345-0033_36_CH01_1";
                    strcpy(ptest, pTest11);
                    ptest+=(SPK_MAX_FILE_NAMELEN + 1)+sizeof(uint64_t);
                    char *pTest12 = "170404213042_00000012-00002345-0033_36_CH01_1";
                    strcpy(ptest, pTest12);
                    ptest+=(SPK_MAX_FILE_NAMELEN + 1)+sizeof(uint64_t);

                    char *pTest2 = "170405213042_00000012-00002345-0033_36_CH01_1";
                    strcpy(ptest, pTest2);
                    ptest+=(SPK_MAX_FILE_NAMELEN + 1)+sizeof(uint64_t);
                    char *pTest21 = "170405213042_00000012-00002345-0033_36_CH01_2";
                    strcpy(ptest, pTest21);
                    ptest+=(SPK_MAX_FILE_NAMELEN + 1)+sizeof(uint64_t);
                    char *pTest22 = "170405213042_00000012-00002345-0033_36_CH01_3";
                    strcpy(ptest, pTest22);
                    ptest+=(SPK_MAX_FILE_NAMELEN + 1)+sizeof(uint64_t);

                    char *pTest3 = "170406213042_00000012-00002345-0033_36_CH02_2";
                    strcpy(ptest, pTest3);
                    ptest+=(SPK_MAX_FILE_NAMELEN + 1)+sizeof(uint64_t);

                    char *pTest4 = "170406213042_00000012-00002345-0033_36_CH03_3";
                    strcpy(ptest, pTest4);
                    ptest+=(SPK_MAX_FILE_NAMELEN + 1)+sizeof(uint64_t);
                    char *pTest41 = "170406213042_00000012-00002345-0033_36_CH02_3";
                    strcpy(ptest, pTest41);
                    ptest+=(SPK_MAX_FILE_NAMELEN + 1)+sizeof(uint64_t);
                    char *pTest42 = "170406213042_00000012-00002345-0033_36_CH01_3";
                    strcpy(ptest, pTest42);
                    ptest+=(SPK_MAX_FILE_NAMELEN + 1)+sizeof(uint64_t);

                    nLen = ((SPK_MAX_FILE_NAMELEN + 1)+sizeof(uint64_t))*10;
                }
#endif

                    while (p < (char*)pData + nLen)
                    {
                        int fname_len = strlen(p);
                        Q_ASSERT(fname_len <= SPK_MAX_FILE_NAMELEN);

                        QString szFilename = QString::fromLatin1(p);
                        CFileAttrib cFileAttrib;
                        p += (SPK_MAX_FILE_NAMELEN + 1);
                        uint64_t fae = *(uint64_t*)p;
                        cFileAttrib.SetFileLength(fae);
                        p += sizeof(uint64_t);
                        if(cFileAttrib.ConvertFromFilename(szFilename))
                        {
                            m_retDir.aryFileAttrib << cFileAttrib;
                        }
                    }
                    emit RecvMsgToui(tr("文件目录已接收完毕。"));
                    emit DirSignal(m_retDir.aryFileAttrib);

                    break;
                default:
                    Q_ASSERT(0);
                    break;
                }
            }
            //使用完，释放缓存
            pBulk->Destroy();
        }
    }
}
