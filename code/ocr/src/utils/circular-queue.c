/*
 * This file is subject to the license agreement located in the file LICENSE
 * and cannot be distributed without it. This notice cannot be
 * removed or modified.
 */

#include "utils/circular-queue.h"

/**
 * @brief Enqueue an element into the circular queue
 */
u8 c_enqueue(ocrCircularQueueData_t *arr, ocrCircularQueue_t *params, ocrCircularQueueData_t elem)
{
    if(params->front  == ((params->rear+1) % params->capacity)) //full
        return 1;
    else
    {
        params->rear = (params->rear+1) % params->capacity;
	arr[params->rear] = elem;
	if(params->front == -1)
	    params->front = params->rear;
	return 0;
    }
}

/**
 * @brief Dequeue an element from the circular queue
 */
u8 c_dequeue(ocrCircularQueueData_t *arr, ocrCircularQueue_t *params, ocrCircularQueueData_t *elem)
{
    if(params->front == -1) //empty
        return 1;
    else
    {
        *elem = arr[params->front];
	if(params->front == params->rear)
	    params->front = params->rear = -1;
	else
	    params->front = (params->front+1) % params->capacity;
	return 0;
    }
}

