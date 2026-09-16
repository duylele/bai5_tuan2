#ifndef __CORE_CM3_H__
#define __CORE_CM3_H__

#include <stdint.h>

#ifndef __INLINE
  #define __INLINE __inline__
#endif

#define __IO volatile

typedef struct
{
  __IO uint32_t ISER[8];
       uint32_t RESERVED0[24];
  __IO uint32_t ICER[8];
       uint32_t RESERVED1[24];
  __IO uint32_t ISPR[8];
       uint32_t RESERVED2[24];
  __IO uint32_t ICPR[8];
       uint32_t RESERVED3[24];
  __IO uint32_t IABR[8];
       uint32_t RESERVED4[56];
  __IO uint8_t  IP[240];
       uint32_t RESERVED5[644];
  __IO uint32_t STIR;
} NVIC_Type;

#define SCS_BASE            (0xE000E000UL)
#define NVIC_BASE           (SCS_BASE + 0x0100UL)
#define NVIC                ((NVIC_Type *)     NVIC_BASE)

#endif
