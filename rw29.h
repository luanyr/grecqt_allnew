#ifndef __RWHB_H__
#define __RWHB_H__

#define __little_endian

#ifdef WIN32
#else
#define _T
#define LPCTSTR char*
#endif
#define RW_MAX_FC_CHANNELS  (12)
#define RW_MAX_DATATYPES    (SPK_MAX_SUBCH_NUM)

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
#define RW_TAG_START    (0x7e7e)
#define RW_TAG_END      (0x0a0d)
#define CUR_VER         (1)

#define RW_DEV_ID       (0x36)
#define RW_MAX_PKTLEN   (64*1024)
#define RW_MAX_PLLEN    (RW_MAX_PKTLEN - sizeof(rw_raw_hdr_t) - sizeof(rw_raw_tail_t))

#define RW_CMD_BASE     (0x3600)
////////////////////////////////////////////////////////////////////////////////
// CORE CMD
////////////////////////////////////////////////////////////////////////////////
#define RW_CMD__ADM_HEARTBEAT  (RW_CMD_BASE | 0x01)  //通信查询
#define RW_CMD__ADM_SELFDIAG   (RW_CMD_BASE | 0x02)  //静态自检
#define RW_CMD__ADM_SOFTRESET  (RW_CMD_BASE | 0x03)  //软件复位
#define RW_CMD__ADM_INQSTATUS  (RW_CMD_BASE | 0x04)  //工作状态查询
#define RW_CMD__ADM_SYNCTIME   (RW_CMD_BASE | 0x05)  //校时
#define RW_CMD__MED_WIPESTOR   (RW_CMD_BASE | 0x0C)  //清除
#define RW_CMD__ADM_SELFDEST   (RW_CMD_BASE | 0x0D)  //软销毁

#define RW_CMD__BIZ_CTRLREC    (RW_CMD_BASE | 0x0B)  //记录控制
#define RW_CMD__BIZ_SETCURVEC  (RW_CMD_BASE | 0x08)  //平台航向数据

#define RW_CMD__USR_LOGIN      (RW_CMD_BASE | 0x09)  //用户登录
#define RW_CMD__USR_LOGOFF     (RW_CMD_BASE | 0x0A)  //用户退出

#define RW_CMDR__ALL_ACK       (RW_CMD_BASE | 0x81)  //命令响应
#define RW_CMDR__ADM_SELFDIAG  (RW_CMD_BASE | 0x83)  //静态自检
#define RW_CMDR__ADM_INQSTATUS (RW_CMD_BASE | 0x85)  //工作状态上报
#define RW_CMDR__MED_WIPESTOR  (RW_CMD_BASE | 0x87)  //清除状态上报
#define RW_CMDR__USR_LOGIN     (RW_CMD_BASE | 0x89)  //登录结果

#pragma pack(push, 2)
//用户登录
#define RW_MAX_UID      (24+2)
#define RW_MAX_PWD      (26+2)
typedef struct
{
	char uid[RW_MAX_UID];
	char pwd[RW_MAX_PWD];
} RW_MSG__USR_LOGIN;

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
    __little_endian int lon;   //精度：0.00001度
    __little_endian int lat;   //精度：0.00001度
    __little_endian uint16_t dir;   //单位：0.1度，范围0~3600
} RW_COORDVEC;

//显控发给存储设备////////////////////////////////////////////
//校时
typedef MSG_TIME16      RW_MSG__ADM_TIMESYNC;

//平台航向数据
#define PRECISION_LONLAT    (0.00001)
#define PRECISION_DIR       (0.1)
typedef RW_COORDVEC         RW_MSG__BIZ_SETCURVEC;

//存储控制
typedef struct
{
	__little_endian CH_BMP  recctrl;
} RW_MSG__BIZ_CTRLREC;

//存储设备发给显控////////////////////////////////////////////
//静态自检
typedef struct
{
	__little_endian uint16_t   rsvd1;
	__little_endian uint16_t   rsvd2;
	__little_endian uint16_t   chan_stat[2];   //光纤和网络链路状态
	__little_endian uint16_t   stor_stat;      //电子盘工作状态
	__little_endian uint16_t   rsvd3;
} RW_MSGR__ADM_SELFDIAG;

//工作状态上报，5s定时上报或查询
#define CAP_UNIT    (4) //单位：kB
typedef struct
{
	__little_endian uint32_t    capboard;       //载板总容量，单位为4KB。
	__little_endian uint64_t    capextboard;    //扩展板总容量，单位为4KB。
	__little_endian uint64_t    capusable;      //设备可用容量，单位为4KB。
	__little_endian uint64_t    caphd;          //磁盘箱总容量，单位为4KB。
	__little_endian uint64_t    caphdusable;    //磁盘箱可用容量，单位为4KB。
	__little_endian uint16_t    chan_stat[2];   //光纤和网络链路状态
	__little_endian uint32_t    rsvd1;
	__little_endian uint16_t    fc_speed;       //光纤记录速度
	__little_endian uint16_t    fc_speed_unit;  //光纤总记录速度，Bit[1:0]：速度GB,MB,KB，B单位显示。00：B；01：KB；10：MB；11：GB
	__little_endian uint16_t    net_speed;      //网络总记录速度，二进制
	__little_endian uint16_t    net_speed_unit; //网络总记录速度，Bit[1:0]：速度GB,MB,KB，B单位显示。00：B；01：KB；10：MB；11：GB
	__little_endian uint16_t        dev_runstat;   //设备工作状态
	__little_endian uint16_t        dev_num;        //设备号
	__little_endian uint8_t         verFPGA;        //FPGA版本号
	__little_endian uint8_t         verVx;          //VxWorks版本号
	__little_endian uint16_t        rsvd2;
	__little_endian uint16_t        rsvd3;
} RW_MSGR__ADM_INQSTATUS;

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
} RW_MSGR__MED_WIPESTOR;

//登录状态上报
#define LOGIN_OK    (0x1)
#define LOGIN_FAIL  (0x0)
typedef struct
{
	__little_endian uint16_t result;
} RW_MSGR__USR_LOGIN;

#pragma pack(pop)

static inline STRING get_speed_unit_str(uint16_t unit)
{
	switch (unit & 0x03)
	{
	case SPEEDUNIT__B:
		return _T("B"); break;
	case SPEEDUNIT__KB:
		return _T("KB"); break;
	case SPEEDUNIT__MB:
		return _T("MB"); break;
	case SPEEDUNIT__GB:
		return _T("GB"); break;
	default:
		break;
	}

	return _T("<UNKNOWN_speed_unit");
}

#endif
