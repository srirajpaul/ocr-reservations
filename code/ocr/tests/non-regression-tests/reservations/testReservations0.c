/*
 * This file is subject to the license agreement located in the file LICENSE
 * and cannot be distributed without it. This notice cannot be
 * removed or modified.
 */

#include "ocr.h"

#ifdef ENABLE_EXTENSION_RESERVATION
#include "extensions/ocr-reservations.h"

#define SIZE 10

u64 count = 0;

ocrGuid_t funcEdt(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {
    ASSERT(paramc == 1 && depc == 1);
    int i;
    u64 temp = count;
    for(i=0;i<100000;i++);
    PRINTF("%d %d\n",*paramv, temp);
    count = temp + 1;
    return NULL_GUID;
}

ocrGuid_t shutEdt(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {
    ASSERT(paramc == 0);
    PRINTF("SHUTDOWN\n");
    ocrShutdown();
    return NULL_GUID;
}

ocrGuid_t mainEdt(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {

    u64 param_val=0, i;

    ocrGuid_t resGuid = NULL_GUID;
    ocrReservationCreate(&resGuid, NULL);

    ocrGuid_t templGuid;
    ocrEdtTemplateCreate(&templGuid, funcEdt, 1, 1);
    
    ocrGuid_t depShutEvt[SIZE] = {UNINITIALIZED_GUID};

    for(i=0;i<SIZE;i++)
    {
      ocrGuid_t outResEvt, depRes;
      ocrEventCreate(&depRes, OCR_EVENT_ONCE_T, EVT_PROP_NONE);
      ocrReservationAcquire(resGuid, OCR_RES_EXCL_T, 1, &depRes, &outResEvt);
      ocrGuid_t edtGuid, outEdtGuid;
      ocrEdtCreate(&edtGuid, templGuid, 1, &param_val, 1, &outResEvt, EDT_PROP_NONE, NULL_HINT, &outEdtGuid);
      param_val++;
      ocrReservationRelease(resGuid, 1, &outEdtGuid, &depShutEvt[i]);
      ocrEventSatisfy(depRes, NULL_GUID);
    }

    ocrGuid_t templShutGuid;
    ocrEdtTemplateCreate(&templShutGuid, shutEdt, 0, SIZE);

    ocrGuid_t edtShutGuid;
    ocrEdtCreate(&edtShutGuid, templShutGuid, 0, NULL, SIZE, &depShutEvt[0], EDT_PROP_NONE, NULL_HINT, NULL);

    return NULL_GUID;
}

#else

ocrGuid_t mainEdt(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {
    PRINTF("Test disabled - ENABLE_EXTENSION_RESERVATION not defined\n");
    ocrShutdown();
    return NULL_GUID;
}

#endif

