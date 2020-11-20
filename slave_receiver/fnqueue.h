#ifndef FNQUEUE_H
#define FNQUEUE_H

#include <stdbool.h>
#include <stdint.h>
#include <Arduino.h>
#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif


bool fnqueue_init(void);
bool fnqueue_run(void);
bool fnqueue_add(void (*function)(void));


#ifdef __cplusplus
}
#endif

#endif
