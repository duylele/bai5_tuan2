#ifndef __SYSTEM_STM32F10X_H__
#define __SYSTEM_STM32F10X_H__

#ifdef __cplusplus
 extern "C" {
#endif

extern uint32_t SystemCoreClock;

void SystemInit(void);
void SystemCoreClockUpdate(void);

#ifdef __cplusplus
}
#endif

#endif
