#include <QFileDialog>
#include "sendthread.h"
#include <QDebug>

#define WARNING_RESPONSE    (200)
#define OVERTIME_NUM        (20)    //定时器是500ms，20就是10s
#define OVERTIME_DL_NUM     (300)   //定时器是500ms，300就是150s

inline void StrCpyFix(QString str, char *p, UINT32 uMaxLen)
{
    //字符串长度，含结束符
    UINT32 uLen = str.length();
    //超长，截断
    QByteArray data = str.toLatin1();
    memcpy(p, data.data(), qMin((UINT32)data.count(),uMaxLen));

    if(uLen>=(uMaxLen-1))
    {
        p[uMaxLen-2] = '\r';
        p[uMaxLen-1] = '\n';
    }
    else if(uLen == 0)
    {
        p[0] = '\r';
        p[1] = '\n';
    }
    else
    {   //末尾修正
        p[uLen] = '\r';
        p[uLen+1] = '\n';
    }
}

inline void VecToCoord(RW_COORDVEC *pCoord, T_VEC *pVec)
{
    pCoord->lon = qRound(pVec->dblLon * 100000);
    pCoord->lat = qRound(pVec->dblLat * 100000);
    pCoord->dir = qRound(pVec->dblDir * 10);
}

inline void VecToCoord2(T_VEC *pCoord, cmi_coordvec_t *pVec)
{
    pVec->lon = static_cast<double>(pCoord->dblLon);
    pVec->lat = static_cast<double>(pCoord->dblLat);
    pVec->dir = static_cast<float>(pCoord->dblDir);
}

inline void TimeFormat(QDateTime dateTime, cmi_datetime_t *pMsgTime)
{
    int y = dateTime.date().year();
    int m = dateTime.date().month();
    int d = dateTime.date().day();

    int h = dateTime.time().hour();
    int mi = dateTime.time().minute();
    int s = dateTime.time().second();

    pMsgTime->year = y%100;
    pMsgTime->month = m;
    pMsgTime->day = d;
    pMsgTime->hour = h;
    pMsgTime->minute = mi;
    pMsgTime->second = s;
}

void RecalFilter(T_FILTER *pFilterSrc, cmi_filter_t *pFilterDest)
{
    Q_ASSERT(pFilterSrc != NULL);
    Q_ASSERT(pFilterDest != NULL);

    memset(pFilterDest, 0, sizeof(cmi_filter_t));
    pFilterDest->en_field = pFilterSrc->uValid&0xFFFF;
    pFilterDest->chan = pFilterSrc->uChannel;
    pFilterDest->dtype_bmp = pFilterSrc->uRange&0xFFFF;
    TimeFormat(pFilterSrc->timeBeg, &pFilterDest->tm_begin);
    TimeFormat(pFilterSrc->timeEnd, &pFilterDest->tm_end);
    VecToCoord2(&pFilterSrc->vec1, &pFilterDest->vec1);
    VecToCoord2(&pFilterSrc->vec2, &pFilterDest->vec2);
    pFilterDest->file_flag = pFilterSrc->uFileFlags;

    strcpy(pFilterDest->file_name, pFilterSrc->strFileName.toLatin1());
}

///////////////////////////////////////////////////////////////////////////
SendThread::SendThread(QObject *parent) :
    QThread(parent)
{
    m_uCmdSeq = 0;
    m_tcpClient = NULL;
    m_pTimer = new QTimer();

    //将定时器超时信号与槽(功能函数)联系起来
    connect( m_pTimer, SIGNAL(timeout()), this, SLOT(TimerFunction()) );
}

SendThread::~SendThread()
{
    if(m_pTimer!=NULL) delete m_pTimer;
}

void SendThread::Run(void)
{

}

BOOL SendThread::SendPacket(void *pBody, UINT32 uSize, UINT16 uCmd)
{
    BOOL bRes(TRUE);

#if 0
    if( (uCmd != RW_CMD__ADM_HEARTBEAT) //心跳不验证返回
            && (uCmd != CMI_CMD__ADM_TRANSBULK) //大文件不验证中间返回
            && (uCmd != CMI_CMD__STOR_OPERFILE)
            && (uCmd != RW_CMD__USR_LOGIN))    //登录不验证返回
    {
        UINT uCmdRes;
        CCmdStatus::enuCmdStatus eCmdStatus = m_response.GetPrevCmdStatus(uCmdRes);
        if(eCmdStatus == CCmdStatus::CMDST_SEND)
        {
            emit SendMsgToui(tr("错误：上一次操作(0x")+QString::number(uCmdRes&0xFFFF,16)+
                             tr(")还没有完成，无法执行本次命令！"));
            return FALSE;
        }
    }
#endif

    if((uSize&1) == 1) uSize += 1;    //如果不按16bit对齐，则补齐

    UINT8 buf[RW_MAX_PLLEN +sizeof(rw_raw_hdr_t)+sizeof(rw_raw_tail_t)+4];  //定义的数据缓存区
    memset(buf, 0, sizeof(buf));
    rw_raw_hdr_t *pHead = (rw_raw_hdr_t*)&buf[0];
    pHead->tag_start = RW_TAG_START;
    pHead->tgt_addr = RW_DEV_ID;
    pHead->cmd_id = uCmd;
    pHead->ver = CUR_VER;
    pHead->attr = 0;
    pHead->seq = 0;

    if((pBody==NULL) || (uSize==0))
    {   //无数据包
        pHead->cmd_seq = m_uCmdSeq++;
        pHead->total_pkts = 1;
        pHead->pkt_num = 1;
        pHead->pkt_len = sizeof(rw_raw_hdr_t)+sizeof(rw_raw_tail_t);

        rw_raw_tail_t *pTail = (rw_raw_tail_t*)&buf[sizeof(rw_raw_hdr_t)];
        pTail->csum = 0;
        pTail->tag_end = RW_TAG_END;

        m_response.AddCmd(pHead->cmd_id, pHead->cmd_seq);
        if (m_tcpClient) {
            if(-1 == m_tcpClient->write((const char*)buf, pHead->pkt_len))
            {
                bRes = FALSE;
            }
        }
    }
    else
    {
        UINT8 *p = (UINT8*)pBody;
        UINT32 nPkt = 0;
        UINT32 uLen = 0;
        while(nPkt*RW_MAX_PLLEN<uSize)
        {
            if((uSize-nPkt*RW_MAX_PLLEN)<RW_MAX_PLLEN) uLen = uSize - nPkt*RW_MAX_PLLEN;
            else uLen = RW_MAX_PLLEN;

            pHead->cmd_seq = (m_uCmdSeq++)&0xFFFF;
            pHead->total_pkts = (uSize/RW_MAX_PLLEN)&0xFFFF;
            if((uSize%RW_MAX_PLLEN)!=0) pHead->total_pkts++;
            pHead->pkt_num = (++nPkt)&0xFFFF;
            pHead->pkt_len = (uLen+sizeof(rw_raw_hdr_t)+sizeof(rw_raw_tail_t))&0xFFFF;

            memcpy(&buf[sizeof(rw_raw_hdr_t)], p, uLen);
            p += uLen;

            rw_raw_tail_t *pTail = (rw_raw_tail_t*)&buf[sizeof(rw_raw_hdr_t)+uLen];
            pTail->csum = 0;
            pTail->tag_end = RW_TAG_END;

            m_response.AddCmd(pHead->cmd_id, pHead->cmd_seq);
            if(-1 == m_tcpClient->write((const char*)buf, pHead->pkt_len))
            {
                bRes = FALSE;
            }
        }
    }
#if 0
    int i =0;
    for (i = 0; i < pHead->pkt_len;i++){
        emit SendMsgToui(tr("0x")+QString::number(buf[i],16));
    }
#endif
    return bRes;
}

void SendThread::HeartBeatSlot()
{
    SendPacket(NULL, 0, RW_CMD__ADM_HEARTBEAT);
}

void SendThread::AllAckSlot(QByteArray head)
{
    rw_raw_hdr_t *pHead = (rw_raw_hdr_t *)head.data();
    INT32 iResTime = m_response.CalAckTime(pHead->cmd_id, pHead->cmd_seq);
    if(WARNING_RESPONSE < iResTime)
    {
        //emit SendMsgToui(tr("警告：指令0x")+QString::number(pHead->cmd_id,16)+
        //                 tr("的响应时间为")+QString::number(iResTime,10)+tr("ms。"));
    }
}

void SendThread::UserLoginSlot(QString strUserName, QString strPassword)
{
    RW_MSG__USR_LOGIN login;
    memset(&login, 0, sizeof(login));
    StrCpyFix(strUserName, &login.uid[0], RW_MAX_UID);
    StrCpyFix(strPassword, &login.pwd[0], RW_MAX_PWD);

    emit SendMsgToui(tr("用户登录指令(")+strUserName+tr(")发送。"));

    SendPacket(&login, sizeof(login), RW_CMD__USR_LOGIN);
}

void SendThread::UserQuitSlot()
{
    emit SendMsgToui(tr("用户退出指令发送。"));

    SendPacket(NULL, 0, RW_CMD__USR_LOGOFF);
}

void SendThread::UserNewSlot(QString strUserName, QString strPassword)
{
    cmi_msg_usr_t user;
    memset(&user, 0, sizeof(user));
    StrCpyFix(strUserName, &user.uid[0], CMI_MAX_UID_LEN);
    StrCpyFix(strPassword, &user.pwd1[0], CMI_MAX_UID_LEN);
    StrCpyFix(strPassword, &user.pwd2[0], CMI_MAX_UID_LEN);
    user.usr_opcode = CMI_USROP__ADDUSR;

    emit SendMsgToui(tr("新建用户指令(")+strUserName+tr(")发送。"));

    SendPacket(&user, sizeof(user), CMI_CMD__USR_OPERUSR);
}

void SendThread::UserChgPwSlot(QString strUserName, QString strPwNew)
{
    cmi_msg_usr_t user;
    memset(&user, 0, sizeof(user));
    StrCpyFix(strUserName, &user.uid[0], CMI_MAX_UID_LEN);
    StrCpyFix(strPwNew, &user.pwd1[0], CMI_MAX_UID_LEN);
    StrCpyFix(strPwNew, &user.pwd2[0], CMI_MAX_UID_LEN);
    user.usr_opcode = CMI_USROP__MODPWD;

    emit SendMsgToui(tr("修改密码指令(")+strUserName+tr(")发送。"));

    SendPacket(&user, sizeof(user), CMI_CMD__USR_OPERUSR);
}

void SendThread::UserDelSlot(QString strUserName, QString strPassword)
{
    cmi_msg_usr_t user;
    memset(&user, 0, sizeof(user));
    StrCpyFix(strUserName, &user.uid[0], CMI_MAX_UID_LEN);
    StrCpyFix(strPassword, &user.pwd1[0], CMI_MAX_UID_LEN);
    StrCpyFix(strPassword, &user.pwd2[0], CMI_MAX_UID_LEN);
    user.usr_opcode = CMI_USROP__DELUSR;

    emit SendMsgToui(tr("删除用户指令(")+strUserName+tr(")发送。"));

    SendPacket(&user, sizeof(user), CMI_CMD__USR_OPERUSR);
}

void SendThread::RecConSlot(UINT32 uCon)
{
    RW_MSG__BIZ_CTRLREC reccon;
    reccon.recctrl = uCon & 0xFFFF;

    emit SendMsgToui(tr("记录控制指令（0x")+QString::number(uCon, 16).toUpper()+tr(")发送。"));

    SendPacket(&reccon, sizeof(reccon), RW_CMD__BIZ_CTRLREC);
}

void SendThread::RecModeSlot(UINT32 uRecMode)
{
    cmi_msg_biz_operchan_t ch;
    memset(&ch, 0, sizeof(cmi_msg_biz_operchan_t));
    ch.filter.en_field = FIELD__CHAN;
    ch.filter.chan = uRecMode;
    ch.param1 = 1;
    ch.chan_opcode = CMI_CHANOP__SET_RECMODE;

    emit SendMsgToui(tr("记录循环配置指令（0x")+QString::number(uRecMode, 16).toUpper()+tr(")发送。"));

    SendPacket(&ch, sizeof(ch), CMI_CMD__BIZ_OPERCHAN);

    ch.filter.chan = ~uRecMode;
    ch.param1 = 0;
    SendPacket(&ch, sizeof(ch), CMI_CMD__BIZ_OPERCHAN);
}

void SendThread::WriteProtectSlot(QVariant filter)
{
    cmi_msg_stor_operfile_t wp;
    memset(&wp, 0, sizeof(cmi_msg_stor_operfile_t));
    wp.file_opcode = CMI_FILEOP__SET_FLAG;
    wp.file_flag = CMI_FILEFLAG__WP;
    m_filterWp = filter.value<T_FILTER>();
    RecalFilter(&m_filterWp, &wp.filter);

    emit SendMsgToui(tr("写保护开启指令（0x")+QString::number(wp.filter.chan, 16).toUpper()+tr(")发送。"));

    SendPacket(&wp, sizeof(cmi_msg_stor_operfile_t), CMI_CMD__STOR_OPERFILE);
}

void SendThread::WriteProtectOffSlot(QVariant filter)
{
    cmi_msg_stor_operfile_t wp;
    memset(&wp, 0, sizeof(cmi_msg_stor_operfile_t));
    wp.file_opcode = CMI_FILEOP__UNDO_FLAG;
    wp.file_flag = CMI_FILEFLAG__WP;
    m_filterWp = filter.value<T_FILTER>();
    RecalFilter(&m_filterWp, &wp.filter);

    emit SendMsgToui(tr("写保护关闭指令（0x")+QString::number(wp.filter.chan, 16).toUpper()+tr(")发送。"));

    SendPacket(&wp, sizeof(cmi_msg_stor_operfile_t), CMI_CMD__STOR_OPERFILE);
}

void SendThread::UnInstStartSlot(QVariant filter)
{
    cmi_msg_biz_ctrlmig_t mig;
    memset(&mig, 0, sizeof(mig));
    mig.ctrl_type = CTRLTYPE__STARTSTOP;
    m_filterUn = filter.value<T_FILTER>();
    RecalFilter(&m_filterUn, &mig.filter);

    emit SendMsgToui(tr("卸载开始指令发送。"));

    if(SendPacket(&mig, sizeof(cmi_msg_biz_ctrlmig_t), CMI_CMD__BIZ_CTRLMIG))
    {
        SetSysInBiz(BIZTYPE__MIG);
    }
}

void SendThread::UnInstStopSlot()
{
    cmi_msg_biz_ctrlmig_t mig;
    memset(&mig, 0, sizeof(mig));
    mig.ctrl_type = CTRLTYPE__STARTSTOP;
    m_filterUn.uChannel = 0;
    //如果是从文件视图发起的操作，需要停止定时器，以取消后续文件。
    if(!m_listFile.isEmpty())
    {
        m_pTimer->stop();
        m_listFile.clear();
    }

    RecalFilter(&m_filterUn, &mig.filter);

    emit SendMsgToui(tr("卸载停止指令发送。"));

    if(SendPacket(&mig, sizeof(cmi_msg_biz_ctrlmig_t), CMI_CMD__BIZ_CTRLMIG))
    {

    }
}

void SendThread::PlaybackStartSlot(QVariant filter)
{
    cmi_msg_biz_ctrlplay_t pb;
    memset(&pb, 0, sizeof(pb));
    pb.ctrl_type = CTRLTYPE__START;
    m_filterPb = filter.value<T_FILTER>();
    RecalFilter(&m_filterPb, &pb.filter);

    emit SendMsgToui(tr("回放开始指令发送。"));

    if(SendPacket(&pb, sizeof(cmi_msg_biz_ctrlplay_t), CMI_CMD__BIZ_CTRLPLAY))
    {
        SetSysInBiz(BIZTYPE__PLAY);
    }
}

void SendThread::PlaybackStopSlot()
{
    cmi_msg_biz_ctrlplay_t pb;
    memset(&pb, 0, sizeof(pb));
    pb.ctrl_type = CTRLTYPE__STOP;
    //开始操作可能是文件视图的菜单发起，也可能是工具栏按钮，两个的过滤器不一样。
    //如果是从文件视图发起的操作，需要停止定时器，以取消后续文件。
    if(!m_listFile.isEmpty())
    {
        m_pTimer->stop();
        m_listFile.clear();
    }

    RecalFilter(&m_filterPb, &pb.filter);

    emit SendMsgToui(tr("回放停止指令发送。"));

    if(SendPacket(&pb, sizeof(cmi_msg_biz_ctrlplay_t), CMI_CMD__BIZ_CTRLPLAY))
    {

    }
}

void SendThread::PlaybackPauseSlot(UINT32 uPbPause)
{
    cmi_msg_biz_ctrlplay_t pb;
    memset(&pb, 0, sizeof(pb));
    pb.ctrl_type = CTRLTYPE__PAUSERESUME;
    pb.filter.en_field = FIELD__CHAN;
    pb.filter.chan = uPbPause;

    emit SendMsgToui(tr("回放暂停指令（0x")+QString::number(uPbPause, 16).toUpper()
                     +tr(")发送。"));

    SendPacket(&pb, sizeof(cmi_msg_biz_ctrlplay_t), CMI_CMD__BIZ_CTRLPLAY);
}

void SendThread::DirSlot(QVariant filter)
{
    cmi_filter_t dir;
    memset(&dir, 0, sizeof(dir));
    m_filterDir = filter.value<T_FILTER>();
    RecalFilter(&m_filterDir, &dir);

    //qDebug("timeBeg = %s\n", qPrintable(m_filterDir.timeBeg.toString()));

    emit SendMsgToui(tr("查询目录指令（0x")+QString::number(dir.chan, 16).toUpper()+tr(")发送。"));

    SendPacket(&dir, sizeof(cmi_filter_t), CMI_CMD__STOR_LISTFILE);
}

void SendThread::FileDelSlot(QVariant filter)
{
    cmi_msg_stor_operfile_t del;
    memset(&del, 0, sizeof(del));
    del.file_opcode = CMI_FILEOP__DELELE;
    del.file_flag = 0;
    m_filterDel = filter.value<T_FILTER>();
    RecalFilter(&m_filterDel, &del.filter);

    emit SendMsgToui(tr("文件删除指令（0x")+QString::number(del.filter.chan, 16).toUpper()+tr(")发送。"));

    SendPacket(&del, sizeof(cmi_msg_stor_operfile_t), CMI_CMD__STOR_OPERFILE);
}

void SendThread::CompressConSlot()
{

}

void SendThread::EncryptConSlot()
{

}

void SendThread::SetTimeSlot(QDateTime dateTime)
{
    qDebug("%s\n", qPrintable(dateTime.toString()));

    RW_MSG__ADM_TIMESYNC t;
    t.year      = dateTime.date().year();
    t.month     = dateTime.date().month();
    t.day       = dateTime.date().day();
    t.hour      = dateTime.time().hour();
    t.minute    = dateTime.time().minute();
    t.second    = dateTime.time().second();

    emit SendMsgToui(tr("授时指令")+dateTime.toString("yyyy-MM-dd HH:mm:ss")+tr("发送。"));

    SendPacket(&t, sizeof(RW_MSG__ADM_TIMESYNC), RW_CMD__ADM_SYNCTIME);
}

void SendThread::BitSlot()
{
    emit SendMsgToui(tr("静态自检指令发送。"));

    SendPacket(NULL, 0, RW_CMD__ADM_SELFDIAG);
   // SendPacket(NULL, 0, RW_CMD__ADM_INQSTATUS);
}

void SendThread::InqStatusSlot()
{
    emit SendMsgToui(tr("状态查询指令发送。"));

    SendPacket(NULL, 0, RW_CMD__ADM_INQSTATUS);
}

void SendThread::ResetSlot()
{
    emit SendMsgToui(tr("软复位指令发送。"));

    SendPacket(NULL, 0, RW_CMD__ADM_SOFTRESET);
}

void SendThread::PowerDownSlot()
{
    emit SendMsgToui(tr("关机指令发送。"));
    cmi_msg_opersys_t temp;
    memset(&temp, 0, sizeof(temp));
    temp.sys_opcode = CMI_SYSOP__SHUTDOWN;
    SendPacket(&temp, sizeof(temp), CMI_CMD__ADM_OPERSYS);
}

void SendThread::WipeSlot()
{
    emit SendMsgToui(tr("数据清除指令发送。"));

    SendPacket(NULL, 0, RW_CMD__MED_WIPESTOR);
}

void SendThread::ReportSlot()
{

}

void SendThread::LogSlot()
{

}

void SendThread::SelfDestorySlot()
{
    emit SendMsgToui(tr("软销毁指令发送。"));

    SendPacket(NULL, 0, RW_CMD__ADM_SELFDEST);
}

void SendThread::UpdateSlot()
{
    INT32 iRes(-2);

    QString fileName = QFileDialog::getOpenFileName(0,
                                   "请选择升级文件",
                                   "",
                                   "GZ files (*.gz);;All files (*.*)",
                                   0,
                                   0);
    if(!fileName.isEmpty())
    {
        emit SendMsgToui(tr("正在发送升级文件 - ")+fileName);

        if(!SendFile(fileName, CMI_CMD__ADM_UPGSYS))
        {
            iRes = -1;
        }
        else
        {
            iRes = 0;
        }
    }
}

BOOL SendThread::SendFile(QString strFileName, UINT16 uCmd)
{
    BOOL bRes(TRUE);

    cmi_msg_adm_transbulk bulk;
    bulk.magic = TRANSBULK_MAGIC;
    bulk.cmd = uCmd;

    QFile file(strFileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        emit SendMsgToui(tr("文件 -")+ strFileName + tr("打开失败！"));
        return FALSE;
    }
    else
    {
        UINT32 uPercent = 0;
        UINT32 uPercentOld = 0;

        bulk.bulk_sz = file.size();
        bulk.offset = 0;

        while(0<(bulk.length = file.read(bulk.data, sizeof(bulk.data))))
        {
            bulk.flag = 0;
            if(bulk.offset == 0) bulk.flag |= TRANSBULK_FLAG__START;
            if((bulk.offset+bulk.length) == bulk.bulk_sz) bulk.flag |= TRANSBULK_FLAG__END;
#if 0
            if((bulk.offset != 0) && !WaitRes())
            {
                emit SendMsgToui(tr("错误：上一个数据无应答，文件传输失败。"));
                bRes = FALSE;
                break;
            }
#endif
            if(!SendPacket(&bulk, sizeof(bulk), CMI_CMD__ADM_TRANSBULK))
            {
                bRes = FALSE;
                break;
            }
            bulk.offset += bulk.length;

            uPercent = (UINT32)((bulk.offset*100)/bulk.bulk_sz);
            if ((uPercentOld!=uPercent))
            {
                uPercentOld = uPercent;
                emit SendProcessToui(true, uPercent);
            }
        }
        file.close();
    }

    //如果失败，发送一个失败消息
    if(!bRes)
    {
        emit SendMsgToui(tr("软件升级失败!"));
    }
    else
    {
        emit SendMsgToui(tr("软件升级完成。"));
    }

    return bRes;
}

void SendThread::UnmountSlot(bool bShow)
{
    cmi_msg_stor_opermed_t oper;
    memset(&oper, 0, sizeof(oper));
    oper.med_opcode = CMI_MEDOP__UMOUNT;
    oper.med_cat = CMI_MEDCAT__UDISK;

    if(bShow) emit SendMsgToui(tr("磁盘箱卸载指令发送。"));

    SendPacket(&oper, sizeof(cmi_msg_stor_opermed_t), CMI_CMD__STOR_OPERMED);
}

void SendThread::MountSlot(bool bShow)
{
    cmi_msg_stor_opermed_t oper;
    memset(&oper, 0, sizeof(oper));
    oper.med_opcode = CMI_MEDOP__MOUNT;
    oper.med_cat = CMI_MEDCAT__UDISK;

    if(bShow) emit SendMsgToui(tr("磁盘箱加载指令发送。"));

    SendPacket(&oper, sizeof(cmi_msg_stor_opermed_t), CMI_CMD__STOR_OPERMED);
}

void SendThread::SetfileSzSlot(UINT32 filesz)
{
    emit SendMsgToui(tr("设置文件大小指令发送:") + QString::number(filesz, 10) + tr(" MB"));
    cmi_msg_biz_operchan_t sMsg;
    memset(&sMsg, 0, sizeof(sMsg));
    sMsg.chan_opcode = 4;
    sMsg.filter.en_field = FIELD__CHAN;
    sMsg.filter.chan = 0x3fff;
    sMsg.param1 = filesz;
    SendPacket(&sMsg, sizeof(cmi_msg_biz_operchan_t), CMI_CMD__BIZ_OPERCHAN);
}


void SendThread::SetCurVecSlot(double dblLon, double dblLat, double dblDir)
{
    RW_MSG__BIZ_SETCURVEC t;
    t.lon = qRound(dblLon*100000);
    t.lat = qRound(dblLat*100000);
    t.dir = qRound(dblDir*10);

    emit SendMsgToui(QString(tr("平台航向(%1,%2,%3)数据发送。")).arg(dblLon).arg(dblLat).arg(dblDir));

    SendPacket(&t, sizeof(RW_MSG__BIZ_SETCURVEC), RW_CMD__BIZ_SETCURVEC);
}

void SendThread::AllCmdSendSlot(QByteArray head, QByteArray data)
{
    rw_raw_hdr_t *pHead = (rw_raw_hdr_t*)head.data();
    //有CMDR返回包的命令在失败时会返回通用包
    if(pHead->cmd_id == CMI_CMDR__ALL_CMDRESULT)
    {
        cmi_msgr_all_cmdresult *pMsgr = (cmi_msgr_all_cmdresult*)data.data();
        if(pMsgr->flag & CMI_CMDRESULT_FLAG__DONE)
            m_response.SetGenStatus(pMsgr->cmd, pHead->cmd_seq, pMsgr->result);
    }
    else if((pHead->cmd_id  != 0) && (pHead->cmd_id  != RW_CMDR__ALL_ACK) && (pHead->cmd_id  != RW_CMDR__ADM_INQSTATUS))
    {
        m_response.SetSpeStatus(pHead->cmd_seq);
    }
}

void SendThread::SetSysBizTypeSlot(QByteArray data)
{
    cmi_msgr_heartbeat_t* hb = (cmi_msgr_heartbeat_t*)data.data();
    BIZ_TYPE biztype = hb->sys_biztype;
    SetSysBizType(biztype);
}

void SendThread::MenuActionSlot(UINT32 uCmdTm, QStringList listFile)
{
    if(m_listFile.isEmpty())
    {
        m_listFile = listFile;

        m_uCmdTm = uCmdTm;

        //开始运行定时器
        m_pTimer->start(500);
    }
    else
    {
        emit SendMsgToui(tr("警告：上一次文件操作未完成，不能进行新的操作。"));
    }
}

void SendThread::TimerFunction()
{
    if(m_listFile.isEmpty())
    {
        m_pTimer->stop();
        return;
    }

    static UINT32 nCount = 0;
    bool bBusy = false;
    UINT32 nOverTimeNum = OVERTIME_NUM;
    switch(m_uCmdTm)
    {
        case TMCMD_PB:
            bBusy = IsSysInBiz(BIZTYPE__REC) ||
                      IsSysInBiz(BIZTYPE__PLAY) ||
                      IsSysInBiz(BIZTYPE__FORMAT);
            break;
        case TMCMD_DL:
            bBusy = IsSysInBiz(BIZTYPE__MIG) ||
                     IsSysInBiz(BIZTYPE__FORMAT);
            nOverTimeNum = OVERTIME_DL_NUM;
            break;
        case TMCMD_DEL:
        case TMCMD_WPON:
        case TMCMD_WPOFF:
            bBusy = !CmdIsDone();
            break;
    }

    //查一下状态,没空就等下一次定时
    if (!bBusy)
    {
        nCount = 0;
        QString strFileName = m_listFile.first();
        T_FILTER filter;
        filter.uChannel = 0;
        filter.uRange = 0;
        filter.timeBeg = QDateTime();
        filter.timeEnd = QDateTime();
        filter.vec1.dblDir = 0;
        filter.vec1.dblLat = 0;
        filter.vec1.dblLon = 0;
        filter.vec2.dblDir = 0;
        filter.vec2.dblLat = 0;
        filter.vec2.dblLon = 0;
        filter.uValid = FIELD__FILE_NAME;
        filter.strFileName = m_listFile.first();
        m_listFile.removeFirst();

        QVariant qfilter;
        qfilter.setValue(filter);
        switch(m_uCmdTm)
        {
            case TMCMD_PB:
                emit SendMsgToui(tr("开始回放文件：")+filter.strFileName);
                PlaybackStartSlot(qfilter);
                break;
            case TMCMD_DL:
                emit SendMsgToui(tr("开始卸载文件：")+filter.strFileName);
                UnInstStartSlot(qfilter);
                break;
            case TMCMD_WPON:
                emit SendMsgToui(tr("写保护打开，文件")+filter.strFileName);
                WriteProtectSlot(qfilter);
                break;
            case TMCMD_WPOFF:
                emit SendMsgToui(tr("写保护关闭，文件")+filter.strFileName);
                WriteProtectOffSlot(qfilter);
                break;
            case TMCMD_DEL:
                emit SendMsgToui(tr("删除文件：")+filter.strFileName);
                FileDelSlot(qfilter);
                break;
        }
    }
    else
    {
        nCount++;
        if(nCount>nOverTimeNum)
        {
            emit SendMsgToui(tr("超时，放弃操作。"));

            nCount = 0;
            m_listFile.clear();
            m_pTimer->stop();
        }
    }
}
