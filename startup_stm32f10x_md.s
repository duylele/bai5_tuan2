.syntax unified
.cpu cortex-m3
.thumb

.global g_pfnVectors
.global Reset_Handler

.section .text.Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
    /* Khởi tạo dữ liệu vùng nhớ .data và .bss nếu cần, hoặc gọi trực tiếp main */
    ldr   r0, =_estack
    mov   sp, r0
    bl    SystemInit
    bl    main
.L_Halt:
    b     .L_Halt

/* Bảng Vector ngắt tối thiểu */
.section .isr_vector,"a",%progbits
g_pfnVectors:
    .word   _estack
    .word   Reset_Handler
    .word   NMI_Handler
    .word   HardFault_Handler
    .word   MemManage_Handler
    .word   BusFault_Handler
    .word   UsageFault_Handler
    .word   0
    .word   0
    .word   0
    .word   0
    .word   SVC_Handler
    .word   DebugMon_Handler
    .word   0
    .word   PendSV_Handler
    .word   SysTick_Handler
    /* Các ngắt ngoại vi, bao gồm USART1_IRQn tại vị trí tương ứng */
    .space  148*4                   /* Đệm khoảng trống vector */
    .word   USART1_IRQHandler       /* Vị trí ngắt UART1 */

/* Các hàm yếu (weak) để tránh lỗi biên dịch nếu chưa dùng */
.thumb_set NMI_Handler,Default_Handler
.thumb_set HardFault_Handler,Default_Handler
.thumb_set MemManage_Handler,Default_Handler
.thumb_set BusFault_Handler,Default_Handler
.thumb_set UsageFault_Handler,Default_Handler
.thumb_set SVC_Handler,Default_Handler
.thumb_set DebugMon_Handler,Default_Handler
.thumb_set PendSV_Handler,Default_Handler
.thumb_set SysTick_Handler,Default_Handler
.thumb_set USART1_IRQHandler,Default_Handler

.section .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
    b       Infinite_Loop
