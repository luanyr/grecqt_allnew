#ifndef __SPARK_DEF_H__
#define __SPARK_DEF_H__

#ifdef QT_GUI_LIB
#include <windows.h>
#include <QTextCodec>
typedef QString         CString;
#define _T(x)           QString(x)
#endif

#ifdef WIN32
// Windows
#pragma once
#ifdef _WIN64
// x64
    #define SPARK64
#else
// x86
    #define SPARK32
#endif
    #include <stdint.h>
    #define unlikely(x)     (x)
    #define likely(x)       (x)
#ifdef QT_GUI_LIB
#else
    typedef void            pthread_mutex_t;
    typedef void            pthread_t;
    typedef void            pthread_barrier_t;
    typedef int64_t         ssize_t;
#endif
    #define spk_get_tick_count GetTickCount
    typedef CString STRING;
    #define SAFE_RELEASE(PTR)  {if(PTR){delete PTR; (PTR)=NULL;}}
    #define SAFE_RELEASE_ARY(PTR)  {if(PTR){delete [] PTR; (PTR)=NULL;}}
#else
// linux x86_64
    #define SPARK64
    #define STRING char*
    #define _T
    #include "pthread.h"
#endif

#define ID2BIT(N)               (1<<(N))

#define SPK_MAX_FILE_NAMELEN    (64-1)
#define SPK_MAX_ID_NAMELEN      (32-1)
#define SPK_MAX_PATHNAME        (1024)

#define SPK_MAX_CHANNELS        (15)
#define SPK_MAX_ZONES           (2)
#define SPK_MAX_SUBCH_NUM       (32)

#define SPK_MAX_MEDIA           (16)
#define SPK_MAX_RAID_DEVS       (2)

#define SPK_MAX_MI_CONNS        (4)

typedef uint16_t CH_BMP;
#if SPK_MAX_CHANNELS > 16
#error SPK_MAX_CHANNELS too big
#endif
typedef uint32_t SUBCH_BMP;
#if SPK_MAX_SUBCH_NUM > 32
#error SPK_MAX_SUBCH_NUM too big
#endif
typedef int CH_SUBID;

typedef uint64_t CPU_BMP;

#define LINKSTAT__DOWN       (0x0)
#define LINKSTAT__ERROR      (0x1)
#define LINKSTAT__UP         (0x2)
#define LINKSTAT__RSVD       (0x3)

#define RUNSTAT__INIT        (0x00)
#define RUNSTAT__READY       (0x01)
#define RUNSTAT__RECORD      (0x02)
#define RUNSTAT__DOWNLOAD    (0x03)
#define RUNSTAT__PLAYBACK    (0x04)
#define RUNSTAT__MIGRATE     (0x05)

#define DIAGBIT__SYSDIR    (0)
#define DIAGBIT__LOGSYS    (1)
#define DIAGBIT__CONFIG    (2)
#define DIAGBIT__ADDIN     (3)
#define DIAGBIT__TOOLS     (4)
#define DIAGBIT__SRIO_BASE (5) // 5-6
#define DIAGBIT__ALL       (0x07f)

#define DIAGBIT__MED_BASE  (16)

typedef enum
{
    MEDCAT__BASE,
    MEDCAT__EXT,
    MEDCAT__UDISK,
    MAX_MEDCAT
} MED_CAT;

typedef enum
{
    DPCTYPE__UNKNOWN = 0,
    DPCTYPE__IPS,
    DPCTYPE__TCP,
    DPCTYPE__UDP,
    DPCTYPE__FILE,
    DPCTYPE__MAX
} DPC_TYPE;

typedef enum
{
    WORKSTATUS__INIT = 0,
    WORKSTATUS__IDLE,
    WORKSTATUS__WORKING,
    WORKSTATUS__STOP,
    WORKSTATUS__MAX
} WORK_STATUS;

typedef enum
{
    BIZTYPE__UNKNOWN  = 0,
    BIZTYPE__REC      = (0x01 << 0),
    BIZTYPE__PLAY     = (0x01 << 1),
    BIZTYPE__MIG      = (0x01 << 2),
    BIZTYPE__FORMAT   = (0x01 << 3),
} BIZ_TYPE;

typedef enum
{
    CTRLTYPE__UNKNOWN = 0,
    CTRLTYPE__STARTSTOP,
    CTRLTYPE__START,
    CTRLTYPE__STOP,
    CTRLTYPE__PAUSERESUME,
    CTRLTYPE__PAUSE,
    CTRLTYPE__RESUME,
} BIZCTRL_TYPE;

typedef enum
{
    EOFTYPE__EACCESS = -1,
    EOFTYPE__NOTEOF = 0,
    EOFTYPE__AUTOSTOP,
    EOFTYPE__DONE,
    EOFTYPE__DISKFULL,    
} EOF_TYPE;

#define USR_PRIVILEGE__NONE     (0x00)
#define USR_PRIVILEGE__OPER     (0x01)
#define USR_PRIVILEGE__ADMIN    (0x02)

typedef enum
{
    ROUTING__DEFAULT,
    ROUTING__TRUNK,
    ROUTING__BRANCH,
    ROUTING__CUSTOM
} ROUTING_METHOD;

typedef enum
{
	LOGLEVEL__DEBUG = 20,
	LOGLEVEL__INFO = 40,
	LOGLEVEL__SYSOUT = 50,
	LOGLEVEL__NOTICE = 60,
	LOGLEVEL__WARN = 80,
	LOGLEVEL__ERROR = 100,
	LOGLEVEL__FATAL = 120
} LOG_LEVEL;

typedef enum
{
    LOGCAT__UNKNOWN = 0,
    LOGCAT__IPS,
    LOGCAT__SYS,
    LOGCAT__ADN,
    LOGCAT__CMI,
    LOGCAT__STO,
    LOGCAT__MAX
} LOG_CATEGORY;

#define SPEEDUNIT__B     (0x00)
#define SPEEDUNIT__KB    (0x01)
#define SPEEDUNIT__MB    (0x02)
#define SPEEDUNIT__GB    (0x03)

typedef struct spk_stats_elem
{
    ssize_t bytes;
    ssize_t pkts;
} spk_stats_elem_t;

typedef struct spk_stats
{
    uint64_t start;
    uint64_t last_tick;
    spk_stats_elem_t xfer;
    spk_stats_elem_t last_xfer;
    uint64_t  xfer_speed;
    spk_stats_elem_t error;
    spk_stats_elem_t drop;
    pthread_mutex_t* lock;
} spk_stats_t;

#ifndef WIN32
static inline void spk_stats_init(spk_stats_t* s)
{
    memset(s, 0, sizeof(spk_stats_t));
    s->lock = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(s->lock, NULL);
}

static inline void spk_stats_reset(spk_stats_t* s)
{
    SAFE_RELEASE(s->lock);
    spk_stats_init(s);
}

static inline void spk_stats_inc_xfer(spk_stats_t* s, uint64_t b, uint64_t p)
{
    assert(s->lock);
    uint64_t now = spk_get_tick_count();
    pthread_mutex_lock(s->lock);
    s->xfer.bytes += b;
    s->xfer.pkts += p;
    if(unlikely(s->start <= 0)) {
        s->last_tick = s->start = now;
    }  else {
        if (now - s->last_tick > 1000) {
            double speed = (double)s->xfer.bytes - s->last_xfer.bytes;
            speed /= (now - s->last_tick);
            speed *= 1000.0;
            s->xfer_speed = (uint64_t)speed;
            s->last_xfer.bytes = s->xfer.bytes;
            s->last_tick = now;
        }
    }
    pthread_mutex_unlock(s->lock);
}

#define DECL_SPK_STATS_INC(CAT) \
static inline void spk_stats_inc_##CAT(spk_stats_t* s, uint64_t b, uint64_t p) \
{ \
    assert(s->lock); \
    uint64_t now = spk_get_tick_count(); \
    pthread_mutex_lock(s->lock); \
    s->CAT.bytes += b; \
    s->CAT.pkts += p; \
    if(unlikely(s->start <= 0)) { \
        s->last_tick = s->start = now; \
    } \
    pthread_mutex_unlock(s->lock); \
}
DECL_SPK_STATS_INC(error);
//DECL_SPK_STATS_INC(overflow);
DECL_SPK_STATS_INC(drop);

static inline uint64_t spk_stats_get_xfer_speed(spk_stats_t* s)
{
    spk_stats_inc_xfer(s, 0, 0); // force to update speed
    return(s->xfer_speed); 
}
#endif

static inline uint64_t spk_stats_get_time_elapsed(spk_stats_t* s)
{
    return((s->start > 0)?(spk_get_tick_count() - s->start) : 0);
}

static inline double spk_stats_get_bps_overall(spk_stats_t* s)
{
    if (!s->start)
        return 0;

    uint64_t now = spk_get_tick_count();
    if (now <= s->start) {
        return 0;
    }
    double rate = (double)(s->xfer.bytes) / (now - s->start); // bytes per ms
    return(rate * 1000); // bytes per second
}
#if 0
static inline double spk_stats_get_bps_wire(spk_stats_t* s)
{
    if (!s->start)
        return 0;

    uint64_t now = spk_get_tick_count();
    if (now <= s->last_tick) {
        return 0;
    }
    uint64_t bytes = s->xfer.bytes - s->xfer_last.bytes;
    speed rate = (double)(bytes) / (now - s->last_tick); // bytes per ms

    s->xfer_last.bytes = s->xfer.bytes;
    s->last_tick = now;
    return(rate * 1000); // bytes per second
}
#endif

#define DECL_SPK_STATS_GETBYTES(CAT) \
static inline uint64_t spk_stats_get_##CAT##_bytes(spk_stats_t* s) \
{ \
    return(s->CAT.bytes); \
}
DECL_SPK_STATS_GETBYTES(xfer)
DECL_SPK_STATS_GETBYTES(error)
//DECL_SPK_STATS_GETBYTES(overflow)
DECL_SPK_STATS_GETBYTES(drop)

#define DECL_SPK_STATS_GETPKTS(CAT) \
static inline uint64_t spk_stats_get_##CAT##_pkts(spk_stats_t* s) \
{ \
    return(s->CAT.pkts); \
}

static inline STRING fmtstr_routing_method(ROUTING_METHOD routing)
{
    switch(routing) {
        case ROUTING__DEFAULT:  return(_T("default")); break;
        case ROUTING__TRUNK:    return(_T("trunk")); break;
        case ROUTING__BRANCH:   return(_T("brance")); break;
        case ROUTING__CUSTOM:   return(_T("custom")); break;
        default:
            break;
    }
    
    return(_T("<UNKNOWN_routing>"));
}

static inline STRING fmtstr_biztype(BIZ_TYPE type)
{
    switch(type) {
        case BIZTYPE__REC:  return(_T("rec")); break;
        case BIZTYPE__PLAY: return(_T("play")); break;
        case BIZTYPE__MIG:  return(_T("mig")); break;
        default:
            break;
    }
    
    return(_T("<UNKNOWN_biztype>"));
}

static inline STRING fmtstr_ctrltype(BIZCTRL_TYPE type)
{
    switch(type) {
        case CTRLTYPE__STARTSTOP: return(_T("start-stop")); break;
        case CTRLTYPE__START:    return(_T("start")); break;
        case CTRLTYPE__STOP:     return(_T("stop")); break;
        case CTRLTYPE__PAUSERESUME: return(_T("pause-resume")); break;
        case CTRLTYPE__PAUSE:    return(_T("pause")); break;
        case CTRLTYPE__RESUME:   return(_T("resume")); break;
        default:
            break;
    }
    return(_T("<UNKNOWN_ctrltype>"));
}

static inline STRING fmtstr_dpctype(DPC_TYPE type)
{
	switch (type)
	{
	case DPCTYPE__IPS:
		return(_T("ips")); break;
	case DPCTYPE__TCP:
		return(_T("tcp")); break;
	case DPCTYPE__UDP:
		return(_T("udp")); break;
	case DPCTYPE__FILE:
		return(_T("file")); break;
    default:
        break;
	}
	return(_T("<UNKNOWN_dpctype>"));
}

static inline STRING fmtstr_med_category(MED_CAT group)
{
	switch (group)
	{
	case MEDCAT__BASE:
		return(_T("base")); break;
	case MEDCAT__EXT:
		return(_T("ext")); break;
	case MEDCAT__UDISK:
		return(_T("udisk")); break;
    default:
        break;
	}
	return(_T("<UNKNOWN_medgroup>"));
}

static inline STRING fmtstr_speed_unit(uint16_t unit)
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

	return _T("<UNKNOWN_speedunit");
}

static inline STRING fmtstr_link_status(uint16_t status)
{
	switch (status)
	{
	case LINKSTAT__DOWN:
		return(_T("down"));	    break;
	case LINKSTAT__UP:
		return(_T("up"));	    break;
	case LINKSTAT__ERROR:
		return(_T("error"));    break;
	default:
	    break;
	}

	return(_T("<UNKNOWN_linkstatus>"));
}

static inline STRING fmtstr_boolean(uint16_t value)
{
	return(!!value ? _T("true") : _T("false"));
}

#endif
