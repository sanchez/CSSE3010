#ifndef HAL_IRCOMS_H
#define HAL_IRCOMS_H

#include "s4396122_util_queue.h"

#define MANCHESTER_BYTE_SIZE 11
#define MANCHESTER_ENCODED_SIZE MANCHESTER_BYTE_SIZE * 4

void s4396122_hal_ircoms_send(int hammingCode);
Queue* s4396122_hal_ircoms_encode(unsigned int hammingCode);
unsigned int s4396122_hal_ircoms_decode(Queue *inQueue);

#endif