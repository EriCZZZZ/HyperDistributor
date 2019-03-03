#ifndef HYPER_DISTRIBUTOR_TYPE_DEF_H
#define HYPER_DISTRIBUTOR_TYPE_DEF_H

#include <sys/types.h>

/**
 * SAE_BITS 32BITS
 * 0xSSSSEEEE
 * S - status
 * E - events
 */

typedef __int32_t SAE_BITS;
typedef __int16_t SAE_STATUS;
typedef __int16_t SAE_EVENTS;
typedef __int16_t SAE_EVENT;

#define NODE_STATUS_MASK 0xFFFF0000
#define NODE_EVENTS_MASK 0x0000FFFF

#define NODE_STATUS(sae) \
    ((__int16_t)(sae >> 16))

#define NODE_EVENTS(sae) \
    ((__int16_t)(sae & NODE_EVENTS_MASK))

#define NODE_STATUS_UN_INIT 0x0000
#define NODE_STATUS_WAIT_SCHEDULING 0x0001
#define NODE_STATUS_IN_QUEUE 0x0002
#define NODE_STATUS_CONSUMER 0x0003
#define NODE_STATUS_WAIT_EVENT 0x0004

#define NODE_EVENTS_NULL 0x0000
#define NODE_EVENTS_READABLE 0x0001
#define NODE_EVENTS_WRITABLE 0x0002
#define NODE_EVENTS_SOME_EVENT 0x0004

#define NODE_ADD_EVENT(sae, event) \
    (sae | event)
#define NODE_REMOVE_EVENT(sae, event) \
    (sae & (!event)) /* todo : re-check */
#define NODE_CLEAR_ALL_EVENTS(sae) \
    (sae & NODE_STATUS_MASK)
#define NODE_CHECK_EVENT(events, event) \
    ((events & event) > 0)

#define NODE_SET_STATUS(sae, status) \
    (((sae << 16) >> 16) | (status << 16))

#define NODE_SAE_INIT NODE_CLEAR_ALL_EVENTS(NODE_SET_STATUS(0, NODE_STATUS_UN_INIT))
#define NODE_SAE_AFTER_DEAL ((NODE_STATUS_CONSUMER << 16) | (NODE_EVENTS_NULL))
#define NODE_SAE_WAIT_EVENT ((NODE_STATUS_WAIT_EVENT << 16) | (NODE_EVENTS_NULL))

#endif /* HYPER_DISTRIBUTOR_TYPE_DEF_H */
