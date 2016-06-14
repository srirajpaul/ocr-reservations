/*
 * This file is subject to the license agreement located in the file LICENSE
 * and cannot be distributed without it. This notice cannot be
 * removed or modified.
 */

#ifndef CIRCULAR_QUEUE_H_
#define CIRCULAR_QUEUE_H_

#include "ocr-types.h"


/**
 * @brief Circular queue data structure
 */
typedef struct
{
    u64 front;
    u64 rear;
    u64 capacity;
} ocrCircularQueue_t;

/**
 * @brief single element in the circular queue
 *
 * The circular queue is an array of such data elements
 */
typedef struct
{
    ocrGuid_t elem;
} ocrCircularQueueData_t;


/**
 * @brief Enqueue an element into the circular queue
 *
 * @param[in,out] arr     Array holding the elements in the queue
 * @param[in,out] params  The queue parameters such as front, rear and so on
 * @param[in] elem        The new element to be enqueued
 * @return a status code
 *     - 0: successful
 *     - !0: error in enqueue
 */
u8 c_enqueue(ocrCircularQueueData_t *arr, ocrCircularQueue_t *params, ocrCircularQueueData_t elem);

/**
 * @brief Dequeue an element from the circular queue
 *
 * @param[in,out] arr     Array holding the elements in the queue
 * @param[in,out] params  The queue parameters such as front, rear and so on
 * @param[in,out] elem    The dequeued element is returned using elem
 * @return a status code
 *     - 0: successful
 *     - !0: error in dequeue
 */
u8 c_dequeue(ocrCircularQueueData_t *arr, ocrCircularQueue_t *params, ocrCircularQueueData_t *elem);

#endif // CIRCULAR_QUEUE_H_

