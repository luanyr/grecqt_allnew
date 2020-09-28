#ifndef __RWHB_H__
#define __RWHB_H__

#define __little_endian

#define RWHB_DEV_ID     (0x36)

#include <QTextCodec>
typedef QString         CString;
#define _T(x)           QString(x)

#ifdef WIN32
typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned int        uint32_t;
typedef unsigned long long  uint64_t;
typedef int                 int32_t;
typedef uint16_t CH_BMP;
typedef CString STR;
#define SPK_MAX_CHANNELS    (14)
#define SPK_FC_CHANNELS     (12)
#define SPK_MAX_DATATYPE    (16)
#define SPK_MAX_FILENAME    (64-1)
#define MAXSENDLEN      (1024)      //最大发送数据包长度，单位：字节
#define MAXRECVLEN      (8192)      //最大接收数据包长度，单位：字节
#else
#define _T
typedef char* STR;
#endif

typedef struct
{
    uint16_t tag_start;
    uint16_t pkt_len;
    uint16_t src_addr;
    uint16_t tgt_addr;
    uint16_t cmd_seq;
    uint16_t cmd_id;
    uint16_t ver;
    uint8_t  attr;
    uint8_t  seq;
    uint16_t total_pkts;
    uint16_t pkt_num;
} rw_raw_hdr_t;

typedef struct
{
    uint16_t  csum;
    uint16_t  tag_end;
} rw_raw_tail_t;
#define TAG_START       (0x7e7e)
#define TAG_END         (0x0a0d)
#define CUR_VER         (1)
#define MI_MAX_PAYLOAD  (64*1024 - sizeof(rw_raw_hdr_t) - sizeof(rw_raw_tail_t))

////////////////////////////////////////////////////////////////////////////////
// CORE CMD
////////////////////////////////////////////////////////////////////////////////
#define RWHB_CMD_BASE       (0x3600)
#define CMD__ADM_HEARTBEAT  (RWHB_CMD_BASE | 0x01)  //通信查询
#define CMD__ADM_SELFDIAG   (RWHB_CMD_BASE | 0x02)  //静态自检
#define CMD__ADM_SOFTRESET  (RWHB_CMD_BASE | 0x03)  //软件复位
#define CMD__ADM_INQSTATUS  (RWHB_CMD_BASE | 0x04)  //工作状态查询
#define CMD__ADM_SYNCTIME   (RWHB_CMD_BASE | 0x05)  //校时
#define CMD__MED_WIPESTOR   (RWHB_CMD_BASE | 0x0C)  //清除
#define CMD__ADM_SELFDEST   (RWHB_CMD_BASE | 0x0D)  //软销毁
#define CMD__ADM_REMOVEHD   (RWHB_CMD_BASE | 0x0E)  //磁盘箱卸载

#define CMD__BIZ_CTRLREC    (RWHB_CMD_BASE | 0x0B)  //记录控制
#define CMD__BIZ_SETCURVEC  (RWHB_CMD_BASE | 0x08)  //平台航向数据

#define CMD__USR_LOGIN      (RWHB_CMD_BASE | 0x09)  //用户登录
#define CMD__USR_LOGOFF     (RWHB_CMD_BASE | 0x0A)  //用户退出

#define CMDR__ALL_ACK       (RWHB_CMD_BASE | 0x81)  //命令响应
#define CMDR__ADM_SELFDIAG  (RWHB_CMD_BASE | 0x83)  //静态自检
#define CMDR__ADM_INQSTATUS (RWHB_CMD_BASE | 0x85)  //工作状态上报
#define CMDR__MED_WIPESTOR  (RWHB_CMD_BASE | 0x87)  //清除状态上报
#define CMDR__USR_LOGIN     (RWHB_CMD_BASE | 0x89)  //登录结果

////////////////////////////////////////////////////////////////////////////////
// EXTEND CMD
////////////////////////////////////////////////////////////////////////////////
#define CMDR__ALL_CMDRESULT (RWHB_CMD_BASE | 0x80)
typedef struct
{
    __little_endian uint16_t    cmd;
    __little_endian uint16_t    flag;
#define CMDRESULT_FLAG__FINISH  (0x01 << 0)
    __little_endian int32_t     result;
    __little_endian uint32_t    rsvd1;
    __little_endian uint32_t    rsvd2;
} MSGR__ALL_CMDRESULT;

#define CMD__USR_ADDUSR     (RWHB_CMD_BASE | 0x41)
#define CMDR__USR_ADDUSR    (CMD__USR_ADDUSR | 0x80)

#define CMD__USR_DELUSR     (RWHB_CMD_BASE | 0x42)
#define CMDR__USR_DELUSR    (CMD__USR_DELUSR | 0x80)

#define CMD__USR_MODIFYPWD  (RWHB_CMD_BASE | 0x43)
#define CMDR__USR_MODIFYPWD (CMD__USR_MODIFYPWD | 0x80)

#define CMD__MED_SETFLAGWP  (RWHB_CMD_BASE | 0x20)  //写保护
#define CMD__MED_DIR        (RWHB_CMD_BASE | 0x21)
#define CMDR__MED_DIR       (CMD__MED_DIR  | 0x80)
#define CMD__MED_DELFILE    (RWHB_CMD_BASE | 0x22)

#define CMD__BIZ_CTRLPLAY   (RWHB_CMD_BASE | 0x31)  //
#define CMD__BIZ_CTRLDL     (RWHB_CMD_BASE | 0x32)  //
#define CMD__BIZ_PAUSEPLAY  (RWHB_CMD_BASE | 0x33)  //
#define CMD__BIZ_UPDATE     (RWHB_CMD_BASE | 0x34)  //

#define CMD__BIZ_OPERCHAN   (RWHB_CMD_BASE | 0x51)  //
#define CHAN_OPCODE__SET_RECMODE    (0x01)
#define CHAN_OPCODE__SET_CMPMODE    (0x02)
#define CHAN_OPCODE__SET_ENCMODE    (0x03)
#define CMDR__BIZ_OPERCHAN  (CMD__BIZ_OPERCHAN  | 0x80)

#define CMD__BIZ_TRANSBULK  (RWHB_CMD_BASE | 0x61)  //

#pragma pack(push, 2)

typedef struct
{
    __little_endian uint8_t year;   //0x0~0x99，代表2000~2099，BCD码
    __little_endian uint8_t month;  //0x1~0x12，BCD码
    __little_endian uint8_t day;    //0x1~0x31，BCD码
    __little_endian uint8_t hour;   //0x0~0x23，BCD码
    __little_endian uint8_t minute; //0x0~0x59，BCD码
    __little_endian uint8_t second; //0x0~0x59，BCD码
} MSG_TIME8;

typedef struct
{
    __little_endian uint16_t year;
    __little_endian uint16_t month;
    __little_endian uint16_t day;
    __little_endian uint16_t hour;
    __little_endian uint16_t minute;
    __little_endian uint16_t second;
} MSG_TIME16;

typedef struct
{
    __little_endian uint32_t lon;   //精度：0.00001度
    __little_endian uint32_t lat;   //精度：0.00001度
    __little_endian uint16_t dir;   //单位：0.1度，范围0~3600
} COORD_VEC;

//显控发给存储设备////////////////////////////////////////////
//校时
typedef MSG_TIME16 MSG__ADM_TIMESYNC;

//平台航向数据
#define PRECISION_LONLAT    (0.00001)
#define PRECISION_DIR       (0.1)
typedef COORD_VEC       MSG__BIZ_SETCURVEC;

//用户登录
#if defined(MAXNAMELEN)
#undef MAXNAMELEN
#endif
#define MAXNAMELEN      (24+2)
#if defined(MAXPASSLEN)
#undef MAXPASSLEN
#endif
#define MAXPASSLEN      (26+2)
typedef struct
{
    uint8_t username[MAXNAMELEN];
    uint8_t password[MAXPASSLEN];
} MSG__USR_LOGIN;

typedef struct
{
    uint8_t uid[MAXNAMELEN];
    uint8_t pwd1[MAXPASSLEN];
    uint8_t pwd2[MAXPASSLEN];
} MSG__USR_OPERATE;

typedef MSG__USR_OPERATE MSG__USR_ADDUSR;
typedef MSG__USR_OPERATE MSG__USR_DELUSR;
typedef MSG__USR_OPERATE MSG__USR_MODIFYPWD;

//存储控制
#define RECCTRL_OFF     (0) //记录停止
#define RECCTRL_ON      (1) //记录启动
#define RECMODE_STOP    (0) //记录停止
#define RECMODE_CYCLE   (1) //循环覆盖
#define FILESIZE_100M   (0x1)
#define FILESIZE_200M   (0x2)
#define FILESIZE_300M   (0x3)
#define FILESIZE_400M   (0x4)
#define FILESIZE_500M   (0x5)
#define FILESIZE_800M   (0x6)
#define FILESIZE_1G     (0x7)
#define FILESIZE_2G     (0x8)
#define FILESIZE_3G     (0x9)
#define FILESIZE_5G     (0xA)
typedef struct
{
    __little_endian CH_BMP  recctrl;
} MSG__BIZ_CTRLREC;

#ifndef FIELD_BMP
typedef uint16_t FIELD_BMP;
#define FIELD__CHAN         (0x01 << 0)
#define FIELD__TIME_BEGIN   (0x01 << 1)
#define FIELD__TIME_END     (0x01 << 2)
#define FIELD__COORD_1P     (0x01 << 3)
#define FIELD__COORD_2P     (0x01 << 4)
#define FIELD__DIR          (0x01 << 5)
#define FIELD__DATA_TYPE    (0x01 << 6)
#define FIELD__FILE_FLAGS   (0x01 << 7)
#define FIELD__FILE_NAME    (0x01 << 8)
#define FIELD__ALL          (FIELD__CHAN|FIELD__TIME_BEGIN|FIELD__TIME_END|FIELD__COORD_1P| \
                             FIELD__COORD_2P|FIELD__DATA_TYPE|FIELD__DATA_TYPE| \
                             FIELD__FILE_FLAGS | FIELD__FILE_NAME)
#endif
typedef struct
{
    __little_endian FIELD_BMP   en_field;
    __little_endian CH_BMP      chan;
    __little_endian MSG_TIME8   tm_begin;
    __little_endian MSG_TIME8   tm_end;
    __little_endian COORD_VEC   coord1;
    __little_endian COORD_VEC   coord2;
    __little_endian uint32_t    dtype_bmp;
    __little_endian uint32_t    file_flag;
    __little_endian char        file_name[SPK_MAX_FILENAME + 1];
} __MSG_FILTER;

typedef struct
{
    __little_endian CH_BMP          set_bmp;
    __little_endian CH_BMP          undo_bmp;
    __little_endian __MSG_FILTER    filter;
} __MSG_CTRL;

//写保护
typedef __MSG_CTRL      MSG__MED_SETFLAGWP;
typedef __MSG_CTRL      MSG__BIZ_CTRLPLAY;
typedef __MSG_CTRL      MSG__BIZ_PAUSEPLAY;
typedef __MSG_CTRL      MSG__BIZ_CTRLDL;
typedef __MSG_FILTER    MSG__MED_DIR;
typedef __MSG_FILTER    MSG__MED_DELFILE;

typedef struct
{
    __little_endian uint16_t    magic;
#define TRANSBULK_MAGIC     (0xf1a1)
    __little_endian uint16_t    cmd;
    __little_endian uint16_t    flag;
#define TRANSBULK_START     (0x01)
#define TRANSBULK_END       (0x02)
#define TRANSBULK_ABORT     (0x04)
    __little_endian uint16_t    rsvd1;
    __little_endian uint64_t    bulk_sz;
    __little_endian uint64_t    offset;
    __little_endian uint32_t    length;
    __little_endian uint32_t    rsvd2;
    __little_endian uint8_t     data[1024];
} MSG__BIZ_TRANSBULK;

//存储设备发给显控////////////////////////////////////////////
//静态自检
#define LINKSTAT_DOWN   (0x0)
#define LINKSTAT_ERROR  (0x1)
#define LINKSTAT_UP     (0x2)
#define LINKSTAT_RSVD   (0x3)
typedef uint16_t    CHAN_LINKSTAT;
typedef uint16_t    STOR_LINKSTAT;
typedef struct
{
    __little_endian uint16_t        rsvd1;
    __little_endian uint16_t        rsvd2;
    __little_endian CHAN_LINKSTAT   chan_stat[2];   //光纤和网络链路状态
    __little_endian STOR_LINKSTAT   stor_stat;      //电子盘工作状态
    __little_endian uint16_t        rsvd3;
} MSGR__ADM_SELFDIAG;

//工作状态上报，5s定时上报或查询
#define CAP_UNIT    (4) //单位：kB
typedef struct
{
    __little_endian uint32_t        capboard;       //载板总容量，单位为4KB。
    __little_endian uint64_t        capextboard;    //扩展板总容量，单位为4KB。
    __little_endian uint64_t        capusable;      //设备可用容量，单位为4KB。
    __little_endian uint64_t        caphd;          //磁盘箱总容量，单位为4KB。
    __little_endian uint64_t        caphdusable;    //磁盘箱可用容量，单位为4KB。
    __little_endian CHAN_LINKSTAT   chan_stat[2];   //光纤和网络链路状态
    __little_endian uint32_t        rsvd1;
    __little_endian uint16_t        fc_speed;       //光纤记录速度
    __little_endian uint16_t        fc_speed_unit;  //光纤总记录速度，Bit[1:0]：速度GB,MB,KB，B单位显示。00：B；01：KB；10：MB；11：GB
    __little_endian uint16_t        net_speed;      //网络总记录速度，二进制
    __little_endian uint16_t        net_speed_unit; //网络总记录速度，Bit[1:0]：速度GB,MB,KB，B单位显示。00：B；01：KB；10：MB；11：GB
#define SPEEDUNIT_B     (0x00)
#define SPEEDUNIT_KB    (0x01)
#define SPEEDUNIT_MB    (0x02)
#define SPEEDUNIT_GB    (0x03)
    __little_endian uint16_t        dev_runstat;   //设备工作状态
#define RUNSTAT_INIT        (0x00)
#define RUNSTAT_READY       (0x01)
#define RUNSTAT_RECORD      (0x02)
#define RUNSTAT_DOWNLOAD    (0x03)
#define RUNSTAT_PLAYBACK    (0x04)
    __little_endian uint16_t        dev_num;        //设备号
    __little_endian uint8_t         verFPGA;        //FPGA版本号
    __little_endian uint8_t         verVx;          //VxWorks版本号
    __little_endian uint16_t        rsvd2;
    __little_endian uint16_t        rsvd3;
} MSGR__ADM_INQSTATUS;

static inline STR get_speed_unit_str(uint16_t unit)
{
    switch (unit & 0x03)
    {
    case SPEEDUNIT_B:
        return _T("B"); break;
    case SPEEDUNIT_KB:
        return _T("KB"); break;
    case SPEEDUNIT_MB:
        return _T("MB"); break;
    case SPEEDUNIT_GB:
        return _T("GB"); break;
    default:
        break;
    }

    return _T("<UNKNOWN>");
}

//清除状态上报
#define WIPE_DONE   (0x5555)
#define WIPE_INPROG (0x0000)
typedef struct
{
    __little_endian uint16_t rsvd1;
    __little_endian uint16_t progress;      //清除进度条，百分比，0x0064: 100%
    __little_endian uint16_t done_flag;     //清除完成标志, 0x5555：完成；0x0000：未完成。
    __little_endian uint16_t rsvd2;
    __little_endian uint16_t rsvd3;
} MSGR__MED_WIPESTOR;

//登录状态上报
#define LOGIN_OK    (0x1)
#define LOGIN_FAIL  (0x0)
typedef struct
{
    __little_endian uint16_t result;
} MSGR__USR_LOGIN;

#define FILE_FLAG__WP       (0x01 << 0)
#define FILE_FLAG__CMP      (0x01 << 1)
#define FILE_FLAG__ENC      (0x01 << 2)
#define FILE_FLAG__MARK     (0x01 << 3)
#define FILE_FLAG__START    (0x01 << 4)
#define FILE_FLAG__END      (0x01 << 5)

typedef struct
{
    __little_endian CH_BMP      set_bmp;
    __little_endian CH_BMP      undo_bmp;
    __little_endian uint32_t    opcode;
    __little_endian uint32_t    param1;
    __little_endian uint32_t    param2;
    __little_endian uint32_t    param3;
} __MSG_CHAN_OPERATE;
typedef __MSG_CHAN_OPERATE   MSG_BIZ_OPERCHAN;

typedef struct
{
    uint64_t    file_sz;
} FILE_ATTR_EX;

#pragma pack(pop)

#endif
