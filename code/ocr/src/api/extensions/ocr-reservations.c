/*
 * This file is subject to the license agreement located in the file LICENSE
 * and cannot be distributed without it. This notice cannot be
 * removed or modified.
 */

#include "ocr-config.h"

#define ENABLE_EXTENSION_RESERVATION //this should be moved to ocr-config.h

#ifdef ENABLE_EXTENSION_RESERVATION

#include <assert.h>

#include "ocr-types.h"
#include "extensions/ocr-reservations.h"

#pragma message "RESERVATION extension is experimental but should be supported on all platforms"


/**
 * @brief Create a Reservation
 **/
u8 ocrReservationCreate(ocrGuid_t *res, void * params)
{
    assert(0);
}

/**
 * @brief Acquire a reservation
 **/
u8 ocrReservationAcquire(ocrGuid_t res, ocrReservationMode_t mode, u32 depc, ocrGuid_t *depv, ocrGuid_t *outputEvent)
{
    assert(0);
}

/**
 * @brief Release a reservation
 *
 * Releasing a reservation triggers the next pending acquire
 **/
u8 ocrReservationRelease(ocrGuid_t res, u32 depc, ocrGuid_t *depv, ocrGuid_t *outputEvent)
{
    assert(0);
}

/**
 * @brief Destroys a reservation
 **/
u8 ocrReservationDestroy(ocrGuid_t res)
{
    assert(0);
}

#endif /* ENABLE_EXTENSION_RESERVATION */

