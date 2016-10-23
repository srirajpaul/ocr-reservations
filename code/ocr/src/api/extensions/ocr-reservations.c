/*
 * This file is subject to the license agreement located in the file LICENSE
 * and cannot be distributed without it. This notice cannot be
 * removed or modified.
 */

//#include "ocr-config.h"

#define ENABLE_EXTENSION_RESERVATION //this should be moved to ocr-config.h

#ifdef ENABLE_EXTENSION_RESERVATION

#include "ocr.h"

#ifdef ENABLE_RES_TESTING
#include <string.h>
#else
#include "ocr-hal.h"
#endif

#include "extensions/ocr-reservations.h"
#include "utils/circular-queue.h"

#pragma message "RESERVATION extension is experimental but should be supported on all platforms"

#define U64_COUNT(given_type) ((sizeof(given_type)+7)/sizeof(u64))

typedef enum {
    RES_DEP_SLOT,
    NUM_RES_SLOTS
} ocrResSlots_t;

ocrGuid_t ocr_realm_res_acq_func(u32 argc, u64 *argv, u32 depc, ocrEdtDep_t depv[])
{
    ocrCircularQueue_t *params = (ocrCircularQueue_t *)(depv[RES_DEP_SLOT].ptr);
    ocrCircularQueueData_t *arr = (ocrCircularQueueData_t *) (params+1), data = {NULL_GUID};
    ocrGuid_t evt_save, ret_evt = *((ocrGuid_t *)argv);

    //if queue is not empty peek the last added element (normal peek is for the first added element)
    if(params->front != -1)
      data = arr[params->rear];
    ocrAddDependence(data.elem, ret_evt, 0, DB_MODE_RO);
    ocrEventCreate(&evt_save, OCR_EVENT_ONCE_T, EVT_PROP_NONE);
    ocrCircularQueueData_t data_save = {evt_save};
    u8 q_ret = c_enqueue(arr, params, data_save);
    ASSERT(q_ret == 0);
    return NULL_GUID;
}

ocrGuid_t ocr_realm_res_rel_func(u32 argc, u64 *argv, u32 depc, ocrEdtDep_t depv[])
{
    ocrCircularQueue_t *params = (ocrCircularQueue_t *)(depv[RES_DEP_SLOT].ptr);
    ocrCircularQueueData_t *arr = (ocrCircularQueueData_t *) (params+1), data = {NULL_GUID};

    u8 q_ret = c_dequeue(arr, params, &data);
    ASSERT(q_ret == 0);
    ocrEventSatisfy(data.elem, NULL_GUID);
    return NULL_GUID;
}

/**
 * @brief Create a Reservation
 **/
u8 ocrReservationCreate(ocrGuid_t *res, void * params)
{
    ASSERT(params == NULL); //for now
    void *ptr;
    u64 len = sizeof(ocrCircularQueue_t) + sizeof(ocrCircularQueueData_t)*RESERVATION_SIZE;
    u8 ret = ocrDbCreate(res, (void **) &ptr, len, DB_PROP_NONE, NULL_HINT, NO_ALLOC);
    if(ret == 0)
    {
        ocrCircularQueue_t * q_params = (ocrCircularQueue_t *) ptr;
        q_params->front = q_params->rear = -1;
        q_params->capacity = RESERVATION_SIZE;
    }
    return ret;
}

/**
 * @brief Acquire a reservation
 **/
u8 ocrReservationAcquire(ocrGuid_t res, ocrReservationMode_t mode, u32 depc, ocrGuid_t *depv, ocrGuid_t *outputEvent)
{
    ocrGuid_t edt_t, edt;
    ocrEventCreate(outputEvent, OCR_EVENT_ONCE_T, EVT_PROP_NONE);

    ocrGuid_t res_depv[depc+1];
#ifdef ENABLE_RES_TESTING
    memcpy(&res_depv[NUM_RES_SLOTS], depv, depc*sizeof(ocrGuid_t));
#else
    hal_memCopy(&res_depv[NUM_RES_SLOTS], depv, depc*sizeof(ocrGuid_t), 0);
#endif
    res_depv[RES_DEP_SLOT] = UNINITIALIZED_GUID;
    ocrEdtTemplateCreate(&edt_t, ocr_realm_res_acq_func, EDT_PARAM_UNK, EDT_PARAM_UNK);
    int ret = ocrEdtCreate(&edt, edt_t, U64_COUNT(ocrGuid_t), (u64*)outputEvent, depc+1, res_depv, EDT_PROP_NONE, NULL_HINT, NULL);
    ocrAddDependence(res, edt, RES_DEP_SLOT, DB_MODE_EW);
    ocrEdtTemplateDestroy(edt_t);
    return ret;
}

/**
 * @brief Release a reservation
 *
 * Releasing a reservation triggers the next pending acquire
 **/
u8 ocrReservationRelease(ocrGuid_t res, u32 depc, ocrGuid_t *depv)
{
    ocrGuid_t edt_t, edt, res_depv[depc+1];
#ifdef ENABLE_RES_TESTING
    memcpy(&res_depv[NUM_RES_SLOTS], depv, depc*sizeof(ocrGuid_t));
#else
    hal_memCopy(&res_depv[NUM_RES_SLOTS], depv, depc*sizeof(ocrGuid_t), 0);
#endif
    res_depv[RES_DEP_SLOT] = UNINITIALIZED_GUID;
    ocrEdtTemplateCreate(&edt_t, ocr_realm_res_rel_func, EDT_PARAM_UNK, EDT_PARAM_UNK);
    int ret = ocrEdtCreate(&edt, edt_t, 0, NULL, depc+1, res_depv, EDT_PROP_NONE, NULL_HINT, NULL);
    ocrAddDependence(res, edt, RES_DEP_SLOT, DB_MODE_EW);
    ocrEdtTemplateDestroy(edt_t);
    return ret;
}

/**
 * @brief Destroys a reservation
 **/
u8 ocrReservationDestroy(ocrGuid_t res)
{
    return ocrDbDestroy(res);
}

#endif /* ENABLE_EXTENSION_RESERVATION */

