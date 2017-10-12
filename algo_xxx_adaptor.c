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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cwm_algo_api.h"

/************************************************************************************************************
**
**     implementation at sensor hub host side
**
**************************************************************************************************************/
int cwm_host_main_process(void);
static int hostAlgoRequestCb(sensortype sentype, algo_req_t *); 
static int hostAlgoLogCb(const char *msg,...);
static int hostAlgoOutputCb(palgo_evt_t algoevt);
static int hostAlgoBrdcastCb(palgo_evt_t algoevt);

cwm_algo_cb_handle_t cwmAlgoCb = 
{
    .request = hostAlgoRequestCb,
    .log = hostAlgoLogCb,
    .output = hostAlgoOutputCb,
    .brdcast = hostAlgoBrdcastCb
};

int cwm_host_main_process(void) 
{
    void *handle;
    int memLen;
    float acc[3] = {0.1, 0.2, 9.81};
    health_info_t healthInfo;

    /* health info example */
    healthInfo.fHeight_cm = 181.5;
    healthInfo.fWeight_kg = 72.5;
    healthInfo.cAge = 30;
    healthInfo.cGender = 'M';
    healthInfo.chand = 'R';
    healthInfo.timestamp = 5432000000;

    /* algo config */
    char cPedometerCfgParmData[3] = {3, 3, 10};

    /* dynamic memory */
    memLen = cwm_algo_xxx_get_memsize();
    handle = malloc(memLen);

    /* sensor event */
    sensor_evt_t senevt;
    senevt.id = ACCELERATION;
    senevt.timestamp = 5432000000;
    memcpy(senevt.data, acc, sizeof(float)*3);

    cwm_algo_xxx_init(handle, &cwmAlgoCb);
    cwm_algo_xxx_set_info(handle, SET_HEALTH_INFO, &healthInfo);
    cwm_algo_xxx_set_info(handle, SET_CONFIG, cPedometerCfgParmData);
    
    cwm_algo_xxx_input(handle, &senevt);
    
    return 0;
}

static int hostAlgoRequestCb(sensortype sentype, algo_req_t *algoreq) /* define the algo request message */
{
    if(sentype > ALGO_SEN_END)
        return -1;
    
    /* example 1*/
    if(algoreq->reqId == SYS_CAN_SLEEP) {
        int wakePeriod = algoreq->min_wake_period;
    }
    /* example 2*/
    if(algoreq->reqId == SEN_DISABLE || algoreq->reqId == SEN_ENABLE) {
        int senId = algoreq->sentype;
    }
    return 0; /* to be defined */
}

static int hostAlgoLogCb(const char *msg ,...)
{ 
    char logdebug[32] = {0,};
    
    memcpy(logdebug, msg, sizeof(const char)*32);

    /* LOGEI("XXXXX\n"); //use log tool at sensor hub host side */
  
    return 0; /* to be defined */
}

static int hostAlgoOutputCb(palgo_evt_t algoevt)
{
    return 0; /* to be defined */
}

static int hostAlgoBrdcastCb(palgo_evt_t algoevt)
{  
    sensor_evt_t myBroadCastEvt;

    memcpy(&myBroadCastEvt, &algoevt->senevt, sizeof(sensor_evt_t));

    return 0; /* to be defined */
}