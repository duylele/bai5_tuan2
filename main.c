#include "stm32f10x.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void SystemInit(void) {
    // Để trống theo yêu cầu của file khởi động
}

#define BUFFER_SIZE 50
volatile char rx_buffer[BUFFER_SIZE];
volatile uint8_t rx_index = 0;
volatile uint8_t cmd_complete = 0;

uint8_t led_state = 0;      // 0: OFF, 1: ON
uint16_t current_pwm = 0;   // 0 - 1000 (tương ứng 0 - 100%)
uint16_t saved_pwm = 500;   // Mặc định lưu mức PWM 50% = 500

// Hàm gửi 1 ký tự qua UART1
void UART_SendChar(char c) {
    while (!(USART1->SR & (1 << 7))); // Chờ cờ TXE (Transmit Data Register Empty)
    USART1->DR = (uint8_t)c;
}

// Hàm gửi chuỗi ký tự qua UART1
void UART_SendString(char* str) {
    while (*str) {
        UART_SendChar(*str++);
    }
}

// Chương trình phục vụ ngắt nhận UART1
void USART1_IRQHandler(void) {
    if (USART1->SR & (1 << 5)) { // Kiểm tra cờ RXNE (có dữ liệu đến)
        char c = (char)USART1->DR;
        
        if (c == '!') { // Gặp dấu chấm than là kết thúc lệnh
            rx_buffer[rx_index] = '\0';
            cmd_complete = 1;
            rx_index = 0;
        } else {
            if (rx_index < BUFFER_SIZE - 1) {
                rx_buffer[rx_index++] = c;
            } else {
                rx_index = 0;
            }
        }
    }
}

// Cấu hình phần cứng trực tiếp bằng thanh ghi (Bare-metal)
void System_Init_Register(void) {
    // 1. Bật Clock cho GPIOA, USART1, AFIO và TIM3
    RCC->APB2ENR |= (1 << 2) | (1 << 4) | (1 << 0); // IOPAEN, USART1EN, AFIOEN
    RCC->APB1ENR |= (1 << 1);                        // TIM3EN

    // 2. Cấu hình chân PA6 (TIM3_CH1 - PWM Output): Alternate Function Push-Pull (MODE=11, CNF=10 -> 0xB)
    GPIOA->CRL &= ~(0xF << 24);
    GPIOA->CRL |=  (0xB << 24);

    // 3. Cấu hình chân PA9 (TX) và PA10 (RX) cho UART1
    GPIOA->CRH &= ~((0xF << 4) | (0xF << 8));
    GPIOA->CRH |=  (0xB << 4);  // PA9 (TX): Alt Func Push-Pull (0xB)
    GPIOA->CRH |=  (0x4 << 8);  // PA10 (RX): Input Floating (0x4)

    // 4. Cấu hình Timer 3 phát xung PWM (Tần số 1kHz)
    TIM3->PSC = 72 - 1;
    TIM3->ARR = 1000 - 1;
    
    TIM3->CCMR1 &= ~(0x7F);
    TIM3->CCMR1 |= (6 << 4) | (1 << 3); // PWM mode 1 và bật preload
    
    TIM3->CCER |= (1 << 0); // Bật ngõ ra Channel 1
    TIM3->CCR1 = 0;         // Duty cycle ban đầu = 0
    TIM3->CR1 |= (1 << 0);  // Bật Timer 3

    // 5. Cấu hình UART1 (Baudrate 9600 với PCLK2 = 72MHz -> BRR = 7500 = 0x1D4C)
    USART1->BRR = 0x1D4C;
    USART1->CR1 |= (1 << 2) | (1 << 3) | (1 << 5) | (1 << 13); // TE, RE, RXNEIE, UE

    // 6. Bật ngắt UART1 trên NVIC
    NVIC_EnableIRQ(USART1_IRQn);
}

// Xử lý logic các lệnh nhận được từ PC
void Process_Command(void) {
    char response[60];

    if (strcmp((char*)rx_buffer, "ON") == 0) {
        led_state = 1;
        current_pwm = saved_pwm;
        TIM3->CCR1 = current_pwm;
        UART_SendString("OK: LED ON\r\n");
    } 
    else if (strcmp((char*)rx_buffer, "OFF") == 0) {
        led_state = 0;
        saved_pwm = current_pwm;
        current_pwm = 0;
        TIM3->CCR1 = 0;
        UART_SendString("OK: LED OFF\r\n");
    } 
    else if (strncmp((char*)rx_buffer, "PWM:", 4) == 0) {
        int percent = atoi((char*)rx_buffer + 4);
        if (percent >= 0 && percent <= 100) {
            saved_pwm = percent * 10; 
            if (led_state == 1) {
                current_pwm = saved_pwm;
                TIM3->CCR1 = current_pwm;
            }
            sprintf(response, "OK: PWM set to %d%%\r\n", percent);
            UART_SendString(response);
        } else {
            UART_SendString("ERROR: Invalid PWM (0-100)\r\n");
        }
    } 
    else if (strcmp((char*)rx_buffer, "Status") == 0) {
        sprintf(response, "STATUS: State = %s, PWM = %d%%\r\n", 
                (led_state ? "ON" : "OFF"), current_pwm / 10);
        UART_SendString(response);
    } 
    else {
        UART_SendString("ERROR: Unknown Command\r\n");
    }
}

int main(void) {
    SystemInit();
    System_Init_Register();
    UART_SendString("STM32F103 Bare-metal Ready!\r\n");

    while (1) {
        if (cmd_complete) {
            Process_Command();
            cmd_complete = 0;
        }
    }
}

int _write(int file, char *ptr, int len) {
    return len;
}
