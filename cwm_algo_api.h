/*
 *  @brief   sample code for algo api definition
 *  @date    2017-07-12
 *  @author  SSD3 team
 *  @history v0.1: new created  2017-07-12
 *           v0.2: fix the struct pointer issue    2017-07-13
 *           v1.0: algo api redefine                2017-08-11
 *           v1.1: algo api refinement            2017-08-15
 *           v1.2: algo api refinement            2017-08-17
*/
#ifndef __CWM_ALGO_API_H__
#define __CWM_ALGO_API_H__

#include <stdint.h>

typedef enum {
    ACCELERATION = 0, /* physical sensor type list */
    MAGNETIC,
    GYRO,
    LIGHT,
    PROXIMITY,
    HEART_RATE,
    PRESSURE,
    GPS,
    /* new added */
    STD_SEN_END,
    STEP_DETECTOR = 100, /* algo sensor type list */
    STEP_COUNTER,
    SIGNIFICANT_MOTION,
    CWM_FALL,
    CWM_CONTEXT_AWARENESS,
    /* new added */
    ALGO_SEN_END
} sensortype;

typedef enum {
    SET_CONFIG = 1,
    ANY_MOTION,
    NON_MOTION,
    SET_GPS_INFO,
    SET_ALGO_BROADCAST_INFO,
    SET_HEALTH_INFO,
    SET_XXX_INFO, /* others info */
    /* new added */
} infotype;

typedef enum {
    SEN_DISABLE = 1,
    SEN_ENABLE,
    SYS_NO_SLEEP,
    SYS_CAN_SLEEP,
    ALGO_BROADCAST,
    /* new added */
} algoreq;

enum {
    ERROR_XXXXX = -2,
    ERROR_INVALID = -1,
    NO_ERROR = 0
};

typedef struct {
    algoreq reqId;
    union {
        struct {
            int sentype;
        };
        struct {
            int rate;
        };
        struct {
            int min_wake_period;
        };
    };
} algo_req_t;

typedef struct {
    union {
        struct {
          uint16_t id;
          uint8_t magAccuracy;
        };
    };
    union {
        struct {
            float raw_data[3];
            float data[3];
        };
        struct {
            uint64_t pedometer;
        };
        struct {
            float fdata[6];
        };
    };
    int64_t timestamp; /* nano secs */
}sensor_evt_t, *psensor_evt_t;

typedef struct {
    sensor_evt_t senevt;
}algo_evt_t, *palgo_evt_t;

/* used for set info example */
typedef struct {
    float fHeight_cm;       
    float fWeight_kg;       
    char cAge;              
    char cGender;            
    char chand;              
    int64_t timestamp;        
} health_info_t, *phealth_info_t;

typedef struct {
    int(*log)(const char *format ,...); /*ALGO Log info callback function */
    int(*request)(sensortype, algo_req_t *); /* ALGO Request callback function */
    int(*output)(palgo_evt_t); /* ALGO Output sensor evt callback function */
    int(*brdcast)(palgo_evt_t); /* ALGO Broadcast algo evt callback function */
} cwm_algo_cb_handle_t, *pcwm_algo_cb_handle_t;

/* supporting external memory */
int cwm_algo_xxx_get_memsize(void);
int cwm_algo_xxx_init(void *mem, pcwm_algo_cb_handle_t cwm_algo_cb);
int cwm_algo_xxx_enable(void *mem);
int cwm_algo_xxx_disable(void *mem);
int cwm_algo_xxx_input(void *mem, psensor_evt_t senevt);
int cwm_algo_xxx_set_info(void *mem, int infotype, void *reserved);
int cwm_algo_xxx_get_version(void *mem);

#endif