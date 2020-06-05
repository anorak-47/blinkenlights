#ifndef _UTILS_H_
#define _UTILS_H_

#include <inttypes.h>

int freeRam(void);

// Deals with the messy details of incrementing an integer
uint8_t increment(uint8_t value, uint8_t step, uint8_t min, uint8_t max);
uint8_t decrement(uint8_t value, uint8_t step, uint8_t min, uint8_t max);
// Deals with the messy details of incrementing an integer
uint16_t increment16(uint16_t value, uint8_t step, uint16_t min, uint16_t max);
uint16_t decrement16(uint16_t value, uint8_t step, uint16_t min, uint16_t max);

#endif
