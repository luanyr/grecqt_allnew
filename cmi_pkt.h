#ifndef __CMI_PKT_H__
#define __CMI_PKT_H__

#include "spark_def.h"
#define __little_endian

#define CMI_MAX_PKTLEN      (256*1024)
#define CMI_TAG_START       (0xf1a1)
#define CMI_TAG_END         (0xa1f1)
#pragma pack(push, 4)
typedef struct
{
	uint16_t tag_start;
	uint16_t cmd_seq;

    uint32_t pkt_len2;

	uint16_t src_addr;
	uint16_t tgt_addr;

	uint16_t cmd_id;
	uint16_t valid_sz;

	uint16_t total_pkts;
	uint16_t pkt_num;

} cmi_pkt_hdr_t;

typedef struct
{
	uint16_t  csum;
	uint16_t  tag_end;
} cmi_pkt_tail_t;

typedef struct
{
    struct cmi_srcinfo*  src_info;
    uint16_t        cmd;
    cmi_pkt_hdr_t*  hdr;
    cmi_pkt_tail_t* tail;
    char*           pl_buf;
    int             pl_len;
    char            pkt_buf[CMI_MAX_PKTLEN];
} cmi_pkt_t;

typedef struct
{
    double lon;
    double lat;
    float dir;
} cmi_coordvec_t;

typedef struct
{
    char year;
    char month;
    char day;
    char hour;
    char minute;
    char second;
    char rsvd1;
    char rsvd2;
} cmi_datetime_t;

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
                             FIELD__COORD_2P|FIELD__DIR|FIELD__DATA_TYPE|FIELD__DATA_TYPE| \
                             FIELD__FILE_FLAGS | FIELD__FILE_NAME)
typedef struct
{
    FIELD_BMP       en_field;
    CH_BMP          chan;
    cmi_datetime_t  tm_begin;
    cmi_datetime_t  tm_end;
    cmi_coordvec_t  vec1;
    cmi_coordvec_t  vec2;
    uint32_t        dtype_bmp;
    uint32_t        file_flag;
    char            file_name[SPK_MAX_FILE_NAMELEN+1];
} cmi_filter_t;

#define SPK_DEV_ID              (0xff)
#define CMI_MAKE_CMD(DEVID, CAT, CMD)      (DEVID<<8 | CAT<<4 | CMD)
#define CMI_MAKE_CMDR(CMD)      (CMD | 0x80)
#define CMI_GET_DEVID(CMD)      (((CMD) >> 8) & 0xff)
#define CMI_GET_CMDCAT(CMD)     (((CMD) & 0x0070) >> 4)
#define CMI_CMDR__ALL_ACK       CMI_MAKE_CMDR(CMI_MAKE_CMD(SPK_DEV_ID, CMI_CMDCAT__INTERNAL, 0))
#define CMI_CMDCAT__INTERNAL    (0x0)
////////////////////////////////////////////////////////////////////////////////
// Category: User
////////////////////////////////////////////////////////////////////////////////
#define CMI_CMDCAT__USR         (0x1)
#define CMI_CMD__USR_LOGIN      CMI_MAKE_CMD(SPK_DEV_ID, CMI_CMDCAT__USR, 0x1) /* cmi_msg_usr_t */ /* void */
#define CMI_CMD__USR_LOGOUT     CMI_MAKE_CMD(SPK_DEV_ID, CMI_CMDCAT__USR, 0x2) /* void */ /* void */
#define CMI_CMD__USR_OPERUSR    CMI_MAKE_CMD(SPK_DEV_ID, CMI_CMDCAT__USR, 0x3) /* cmi_msg_usr_t */ /* void */
    #define CMI_USROP__ADDUSR   (0x01)
    #define CMI_USROP__DELUSR   (0x02)
    #define CMI_USROP__MODPWD   (0x03)
#define CMI_MAX_UID_LEN   (32)
typedef struct
{
    uint32_t usr_opcode;
    char uid[CMI_MAX_UID_LEN];
    char pwd1[CMI_MAX_UID_LEN];
    char pwd2[CMI_MAX_UID_LEN];
} cmi_msg_usr_t;

////////////////////////////////////////////////////////////////////////////////
// Category: storage
////////////////////////////////////////////////////////////////////////////////
#define CMI_CMDCAT__STOR        (0x2)
#define CMI_CMD__STOR_FORMATALL     CMI_MAKE_CMD(SPK_DEV_ID, CMI_CMDCAT__STOR, 0x1) /* void */ /* void */
#define CMI_CMD__STOR_OPERMED       CMI_MAKE_CMD(SPK_DEV_ID, CMI_CMDCAT__STOR, 0x2) /* cmi_msg_stor_opermed_t */ /* void */
    #define CMI_MEDOP__MOUNT    (0x01)
    #define CMI_MEDOP__UMOUNT   (0x02)
    #define CMI_MEDCAT__UDISK   (0x01)
typedef struct
{
    uint32_t med_opcode;
    uint32_t med_cat;
} cmi_msg_stor_opermed_t;

#define CMI_CMD__STOR_OPERFILE      CMI_MAKE_CMD(SPK_DEV_ID, CMI_CMDCAT__STOR, 0x3) /* cmi_msg_stor_operfile_t */ /* void */
    #define CMI_FILEOP__SET_FLAG    (0x01)
    #define CMI_FILEOP__UNDO_FLAG   (0x02)
        #define CMI_FILEFLAG__WP       (0x01 << 0)
        #define CMI_FILEFLAG__CMP      (0x01 << 1)
        #define CMI_FILEFLAG__ENC      (0x01 << 2)
        #define CMI_FILEFLAG__MARK     (0x01 << 3)
        #define CMI_FILEFLAG__START    (0x01 << 4)
        #define CMI_FILEFLAG__END      (0x01 << 5)
    #define CMI_FILEOP__DELELE      (0x03)
typedef struct
{
    cmi_filter_t filter;
    uint32_t file_opcode;
    uint32_t file_flag;
} cmi_msg_stor_operfile_t;

#define CMI_CMD__STOR_LISTFILE      CMI_MAKE_CMD(SPK_DEV_ID, CMI_CMDCAT__STOR, 0x4) /* cmi_filter_t */ /* CUSTOM */
typedef struct
{
    char     filename[SPK_MAX_FILE_NAMELEN];
    uint64_t file_sz;
} cmi_filedesc_t;

////////////////////////////////////////////////////////////////////////////////
// Category: business
////////////////////////////////////////////////////////////////////////////////
#define CMI_CMDCAT__BIZ         (0x3)
#define CMI_CMD__BIZ_CTRLREC    CMI_MAKE_CMD(SPK_DEV_ID, CMI_CMDCAT__BIZ, 0x1) /* cmi_msg_biz_ctrlrec_t */ /* void */
typedef struct
{
    BIZCTRL_TYPE ctrl_type;
    cmi_filter_t filter;
} cmi_msg_biz_ctrlrec_t;

#define CMI_CMD__BIZ_CTRLPLAY   CMI_MAKE_CMD(SPK_DEV_ID, CMI_CMDCAT__BIZ, 0x2) /* cmi_msg_biz_ctrlplay_t */ /* void */
typedef struct
{
    BIZCTRL_TYPE ctrl_type;
    cmi_filter_t filter;
} cmi_msg_biz_ctrlplay_t;

#define CMI_CMD__BIZ_CTRLMIG    CMI_MAKE_CMD(SPK_DEV_ID, CMI_CMDCAT__BIZ, 0x3) /* cmi_msg_biz_ctrlmig_t */ /* void */
#define CMI_CMD__BIZ_CTRLDL     CMI_MAKE_CMD(SPK_DEV_ID, CMI_CMDCAT__BIZ, 0x4) /* void */ /* void */
typedef struct
{
    BIZCTRL_TYPE ctrl_type;
    cmi_filter_t filter;
} cmi_msg_biz_ctrlmig_t;

#define CMI_CMD__BIZ_OPERCHAN   CMI_MAKE_CMD(SPK_DEV_ID, CMI_CMDCAT__BIZ, 0x5) /* cmi_msg_biz_operchan_t */ /* void */
    #define CMI_CHANOP__SET_RECMODE    (0x01)
    #define CMI_CHANOP__SET_CMPMODE    (0x02)
    #define CMI_CHANOP__SET_ENCMODE    (0x03)
typedef struct
{
    int     chan_opcode;
    cmi_filter_t filter;
    uint32_t param1;
    uint32_t param2;
} cmi_msg_biz_operchan_t;

#define CMI_CMD__BIZ_SETCURVEC  CMI_MAKE_CMD(SPK_DEV_ID, CMI_CMDCAT__BIZ, 0x6) /* cmi_coordvec_t */ /* void */

////////////////////////////////////////////////////////////////////////////////
// Category: administration
////////////////////////////////////////////////////////////////////////////////
#define CMI_CMDCAT__ADM         (0x4)
#define CMI_CMD__ADM_HEARTBEAT  CMI_MAKE_CMD(SPK_DEV_ID, CMI_CMDCAT__ADM, 0x1) /* void */ /* cmi_msgr_heartbeat */
typedef struct
{
    BIZ_TYPE    sys_biztype;
    uint32_t    sys_error;
} cmi_msgr_heartbeat_t;
#define CMI_CMD__ADM_SYNCTIME   CMI_MAKE_CMD(SPK_DEV_ID, CMI_CMDCAT__ADM, 0x2) /* cmi_datetime_t */ /* void */

#define CMI_CMD__ADM_SELFDIAG   CMI_MAKE_CMD(SPK_DEV_ID, CMI_CMDCAT__ADM, 0x3) /* void */ /* cmi_msgr_selfdiag */
typedef struct
{
    uint16_t chan_linkstat[SPK_MAX_CHANNELS];
    uint16_t stor_linkstat[MAX_MEDCAT];
    uint32_t test_bit;
} cmi_msgr_selfdiag;

#define CMI_CMD__ADM_SOFTRESET  CMI_MAKE_CMD(SPK_DEV_ID, CMI_CMDCAT__ADM, 0x4) /* void */ /* void */
#define CMI_CMD__ADM_INQSTATUS  CMI_MAKE_CMD(SPK_DEV_ID, CMI_CMDCAT__ADM, 0x5) /* void */ /* cmi_msgr_inqstatus */
typedef struct
{
    uint64_t medcat_cap[MAX_MEDCAT];
    uint64_t medcat_free[MAX_MEDCAT];
    uint16_t chan_linkstat[SPK_MAX_CHANNELS];
    uint16_t stor_linkstat[MAX_MEDCAT];
    uint64_t chan_speed[SPK_MAX_CHANNELS];
    uint64_t dev_runstat;
    uint64_t ver_fpga;
    uint64_t ver_os;
} cmi_msgr_inqstatus;
#define CMI_CMD__ADM_SELFDEST   CMI_MAKE_CMD(SPK_DEV_ID, CMI_CMDCAT__ADM, 0x6) /* void */ /* void */

#define CMI_CMD__ADM_UPGSYS     CMI_MAKE_CMD(SPK_DEV_ID, CMI_CMDCAT__ADM, 0x7) /* FILE */ /* void */
#define CMI_CMD__ADM_TRANSBULK  CMI_MAKE_CMD(SPK_DEV_ID, CMI_CMDCAT__ADM, 0x8) /* cmi_msg_adm_transbulk */ /* void */

#define CMI_CMD__ADM_OPERSYS    CMI_MAKE_CMD(SPK_DEV_ID, CMI_CMDCAT__ADM, 0x9) /* cmi_msg_syscmd_t */ /* void */
    #define CMI_SYSOP__QUIT_SYS    (0x01)
    #define CMI_SYSOP__SHUTDOWN    (0x02)
typedef struct
{
    int sys_opcode;
} cmi_msg_opersys_t;

typedef struct
{
#define TRANSBULK_MAGIC         (0xf1a1)
	__little_endian uint16_t    magic;
	__little_endian uint16_t    cmd;
#define TRANSBULK_FLAG__START   (0x01)
#define TRANSBULK_FLAG__END     (0x02)
#define TRANSBULK_FLAG__ABORT   (0x04)
	__little_endian uint16_t    flag;
	__little_endian uint16_t    rsvd1;
	__little_endian uint64_t    bulk_sz;
	__little_endian uint64_t    offset;
	__little_endian uint32_t    length;
	__little_endian uint32_t    rsvd2;
#define CMI_MAX_BULK_PIECE_SZ   (1024)
	__little_endian char        data[CMI_MAX_BULK_PIECE_SZ];
} cmi_msg_adm_transbulk;

////////////////////////////////////////////////////////////////////////////////
// Category: system debug interface
////////////////////////////////////////////////////////////////////////////////
#define CMI_CMDCAT__DBG         (0x7)
#define CMI_CMD__DBG_INQENV     CMI_MAKE_CMD(SPK_DEV_ID, CMI_CMDCAT__DBG, 0x1) /* void */ /* void */
#define CMI_CMD__DBG_INQCTX     CMI_MAKE_CMD(SPK_DEV_ID, CMI_CMDCAT__DBG, 0x2) /* void */ /* void */
#define CMI_CMD__DBG_SUBLOG     CMI_MAKE_CMD(SPK_DEV_ID, CMI_CMDCAT__DBG, 0x3) /* cmi_msg_dbg_sublog */ /* void */
typedef struct
{
    char sub_cat;
    char sub_lvl;
} cmi_msg_dbg_sublog;
#define CMI_CMD__DBG_LOGEVENT   CMI_MAKE_CMD(SPK_DEV_ID, CMI_CMDCAT__DBG, 0x4) /* NA */ /* NA */
#define CMI_CMDR__DBG_LOGEVENT  CMI_MAKE_CMDR(CMI_CMD__DBG_LOGEVENT) /* NA */ /* cmi_msgr_dbg_evt */
#ifdef WIN32
#pragma warning( push )
#pragma warning( disable : 4200 )
#endif
typedef struct
{
    char category;
    char level;
    char msg[0];
} cmi_msgr_dbg_evt;
#ifdef WIN32
#pragma warning( pop )
#endif
#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////
// Category: all
////////////////////////////////////////////////////////////////////////////////
#define CMI_CMDR__ALL_CMDRESULT     CMI_MAKE_CMDR(CMI_MAKE_CMD(SPK_DEV_ID, CMI_CMDCAT__INTERNAL, 0x1))
typedef struct
{
	__little_endian uint16_t    cmd;
	__little_endian uint16_t    flag;
#define CMI_CMDRESULT_FLAG__DONE    (0x01 << 0)
#define CMI_CMDRESULT_FLAG__ABORT   (0x01 << 1)
	__little_endian uint32_t    op_code;
    union {
    	__little_endian int32_t     result;
    	__little_endian int32_t     progress;
    };
	__little_endian uint32_t    rsvd1;
} cmi_msgr_all_cmdresult;


static inline STRING fmtstr_chan_opcode(uint32_t opcode)
{
    switch(opcode) {
    case CMI_CHANOP__SET_RECMODE:
        return (_T("SET_RECMODE")); break;
    case CMI_CHANOP__SET_CMPMODE:
        return (_T("SET_CMPMODE")); break;
    case CMI_CHANOP__SET_ENCMODE:
        return (_T("SET_ENCMODE")); break;
    default:        break;
    }
    return(_T("<UNKNOWN_chan_opcode>"));
}

static inline STRING fmtstr_file_opcode(uint32_t opcode)
{
    switch(opcode) {
    case CMI_FILEOP__SET_FLAG:
        return (_T("SET_FLAG")); break;
    case CMI_FILEOP__UNDO_FLAG:
        return (_T("UNDO_FLAG")); break;
    case CMI_FILEOP__DELELE:
        return (_T("DELETE")); break;
    default:        break;
    }
    return(_T("<UNKNOWN_file_opcode>"));
}

static inline STRING fmtstr_med_opcode(uint32_t opcode)
{
    switch(opcode) {
    case CMI_MEDOP__MOUNT:
        return (_T("MOUNT")); break;
    case CMI_MEDOP__UMOUNT:
        return (_T("UMOUNT")); break;
    default:        break;
    }
    return(_T("<UNKNOWN_med_opcode>"));
}

static inline STRING fmtstr_usr_opcode(uint32_t opcode)
{
    switch(opcode) {
    case CMI_USROP__ADDUSR:
        return (_T("ADD_USER")); break;
    case CMI_USROP__DELUSR:
        return (_T("DEL_USER")); break;
    case CMI_USROP__MODPWD:
        return (_T("MODIFY_PWD")); break;
    default:        break;
    }
    return(_T("<UNKNOWN_usr_opcode>"));
}

static inline int __get_setbits(uint32_t bits)
{
    int bits_set = 0;
    for (int i=0; i<32; i++)
        bits_set += !!(bits & (0x1<<i));
    return bits_set;
}

static inline STRING fmtstr_file_flag(uint32_t flag)
{
    if (__get_setbits(flag) > 1) {
        return(_T("<MULTI_file_flag>"));
    }
    switch(flag) {
    case CMI_FILEFLAG__WP:
        return (_T("PROTECTED")); break;
    case CMI_FILEFLAG__CMP:
        return (_T("COMPRESSED")); break;
    case CMI_FILEFLAG__ENC:
        return (_T("ENCODED")); break;
    case CMI_FILEFLAG__MARK:
        return (_T("MARKED")); break;
    default:        break;
    }
    return(_T("<UNKNOWN_file_flag>"));
}

#endif
