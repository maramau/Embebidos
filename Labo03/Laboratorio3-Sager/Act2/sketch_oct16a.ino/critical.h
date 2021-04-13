#ifndef CRITICAL_C
#define CRITICAL_C
#include <Arduino.h>
#include <string.h>
#ifdef __cplusplus
extern "C" {
#endif
void critical_begin(void);
void critical_end(void);
#ifdef __cplusplus
}
#endif

#endif
