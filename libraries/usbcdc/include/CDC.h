// Callable from C++ to handle CDC ACM
// MPM Aug 2020

#ifndef CDC_H
#define CDC_H
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void CDC_Init();
void CDC_deInit();
size_t CDC_write(uint8_t* buf, size_t size);
size_t CDC_recv(uint8_t* buf);

#ifdef __cplusplus
}
#endif


#endif
