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
#include <string.h>
#include "cwm_algo_api.h"

/************************************************************************************************************
**
**     implementation at ALGO side
**
**************************************************************************************************************/
static int cwm_algo_main_process(void); /* algo team define */
static int cwm_algo_send_request(sensortype sentype, algo_req_t *);
static int cwm_algo_send_log(const char *msg,...);
static int cwm_algo_send_evt(palgo_evt_t algoevt);
static int cwm_algo_send_brdcast(palgo_evt_t algoevt);

static pcwm_algo_cb_handle_t cwmAlgoCbPtr = NULL;

int cwm_algo_xxx_init(void *mem, pcwm_algo_cb_handle_t cwm_algo_cb)
{
    /* 
        do the initialize stuff here
        e.g. init_cwm_activity_pedometer() 
        */
        
    if(cwm_algo_cb != NULL) {
        if(cwm_algo_cb->request != NULL && cwm_algo_cb->log != NULL && cwm_algo_cb->output != NULL) {
            cwmAlgoCbPtr = cwm_algo_cb;
        }
        else {
            return ERROR_INVALID;
        }
    }
    else {
        return ERROR_INVALID;
    }
    
    return NO_ERROR; 
}


int cwm_algo_xxx_input(void *mem, psensor_evt_t senevt) 
{
    sensor_evt_t mySenEvt;

    memcpy(&mySenEvt, senevt, sizeof(sensor_evt_t));
    
    switch(mySenEvt.id) {
        case ACCELERATION:
            cwm_algo_main_process();  /* algo team define */
            break;
        default:
          break;
    };

    return NO_ERROR;
}

int cwm_algo_xxx_set_info(void *mem, int infotype, void *reserved)
{
    float height, weight;
    char param[3] = {0};

    if(reserved != NULL ) {
        if(infotype == SET_HEALTH_INFO) {
            phealth_info_t heath_info = (health_info_t*)reserved;
            height = heath_info->fHeight_cm;
            weight = heath_info->fWeight_kg;
        }
        if(infotype == SET_CONFIG) {
            char *cfg_param = (char *)reserved;
            memcpy(param, cfg_param, sizeof(char)*3);
        }
    }
    
    return NO_ERROR;
}

int cwm_algo_xxx_get_memsize(void)
{
    int size = 128; /* just for debug purpose */

    return size;
}

int cwm_algo_xxx_enable(void *mem)
{
    /* 
        do the enable stuff here
        e.g. enable_cwm_activity_pedometer()
      */
    return NO_ERROR;
}

int cwm_algo_xxx_disable(void *mem)
{
    /* 
        do the disable stuff here
        e.g. disable_cwm_activity_pedometer()
      */
    return NO_ERROR;
}

int cwm_algo_xxx_get_version(void *mem)
{
    /* 
        do the get version stuff here
        e.g. version_cwm_activity_pedometer()
      */
    return NO_ERROR;
}


static int cwm_algo_main_process(void)
{
    static uint32_t ped_dbg = 0; /* just for debug purpose */

    /* send request to host */
    algo_req_t algoReqst;
    algoReqst.reqId = SYS_CAN_SLEEP; /* system can go to low power mode */
    algoReqst.min_wake_period = 1000;

    cwm_algo_send_request(STEP_COUNTER, &algoReqst);

    algoReqst.reqId = SEN_ENABLE;
    algoReqst.sentype = HEART_RATE;

    cwm_algo_send_request(STEP_COUNTER, &algoReqst);

    /* send LOG to host */
    cwm_algo_send_log("cwm algo output log\n");

    /* send algo event to host */
    algo_evt_t algoevt;
    algoevt.senevt.id = STEP_COUNTER;
    algoevt.senevt.pedometer = ped_dbg;
    ped_dbg++; /* just for debug purpose */
    cwm_algo_send_evt(&algoevt);

    /* send broadcast event to host */
    cwm_algo_send_brdcast(&algoevt);

    return NO_ERROR;
}
/**
** @brief send request to host
**
*/
static int cwm_algo_send_request(sensortype sentype, algo_req_t *algoReq) {
    
    if(cwmAlgoCbPtr != NULL && cwmAlgoCbPtr->request != NULL) {
        cwmAlgoCbPtr->request(sentype, algoReq);
    }
    else {
        return ERROR_INVALID;
    }

    return NO_ERROR;
}

/**
** @brief send log to host
**
*/
static int cwm_algo_send_log(const char *msg,...) {
    
    if(cwmAlgoCbPtr != NULL && cwmAlgoCbPtr->log != NULL) {
        cwmAlgoCbPtr->log(msg);
    }
    else {
        return ERROR_INVALID;
    }
  
    return NO_ERROR;
}

/**
** @brief send algo event to host
**
*/
static int cwm_algo_send_evt(palgo_evt_t algoevt) {
    
    if(cwmAlgoCbPtr != NULL && cwmAlgoCbPtr->output != NULL) {
        cwmAlgoCbPtr->output(algoevt);
    }
    else {
        return ERROR_INVALID;
    }
  
  return NO_ERROR;
}

/**
** @brief send broadcast event to host
**
*/
static int cwm_algo_send_brdcast(palgo_evt_t algoevt) {
    
    if(cwmAlgoCbPtr != NULL && cwmAlgoCbPtr->brdcast != NULL) {
        cwmAlgoCbPtr->brdcast(algoevt);
    }
    else {
        return ERROR_INVALID;
    }

    return NO_ERROR;
}