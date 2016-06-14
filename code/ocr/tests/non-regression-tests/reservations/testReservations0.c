/*
 * This file is subject to the license agreement located in the file LICENSE
 * and cannot be distributed without it. This notice cannot be
 * removed or modified.
 */

#include "ocr.h"

#include <assert.h>

#ifdef ENABLE_EXTENSION_RESERVATION
#include "extensions/ocr-reservations.h"

ocrGuid_t funcEdt(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {
    assert(paramc == 1 && depc == 1);
    PRINTF("%d\n",*paramv);
    return NULL_GUID;
}

ocrGuid_t shutEdt(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {
    assert(paramc == 0 && depc == 1);
    ocrShutdown();
    return NULL_GUID;
}

ocrGuid_t mainEdt(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {

    u64 param_val=0, size=10, i;

    ocrGuid_t resGuid = NULL_GUID, outResEvt1, outResEvt2;
    ocrReservationCreate(&resGuid, NULL);

    ocrGuid_t templGuid;
    ocrEdtTemplateCreate(&templGuid, funcEdt, 1, 1);
    
    ocrGuid_t depResEvt[size+1];

    for(i=0;i<size;i++)
    {
      ocrGuid_t outResEvt;
      ocrReservationAcquire(resGuid, OCR_RES_EXCL_T, 1, &depResEvt[i], &outResEvt);
      ocrGuid_t edtGuid, outEdtGuid; 
      ocrEdtCreate(&edtGuid, templGuid, 1, &param_val, 1, &outResEvt, EDT_PROP_NONE, NULL_HINT, &outEdtGuid);
      param_val++;
      ocrReservationRelease(resGuid, 1, &outEdtGuid, &depResEvt[i+1]);
    }

    ocrGuid_t templShutGuid;
    ocrEdtTemplateCreate(&templShutGuid, shutEdt, 0, 1);

    ocrGuid_t edtShutGuid;
    ocrEdtCreate(&edtShutGuid, templShutGuid, 0, NULL, 1, &depResEvt[size], EDT_PROP_NONE, NULL_HINT, NULL);

    return NULL_GUID;
}

#else

ocrGuid_t mainEdt(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {
    PRINTF("Test disabled - ENABLE_EXTENSION_RESERVATION not defined\n");
    ocrShutdown();
    return NULL_GUID;
}

#endif

