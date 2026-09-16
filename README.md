# STM32F103 Bare-Metal UART & PWM Project
Dự án phát triển vi điều khiển **STM32F103 (Medium Density - C8T6)** trên nền tảng Linux (Ubuntu) sử dụng phương pháp lập trình **Bare-metal** (truy cập trực tiếp thanh ghi phần cứng) hoàn toàn không phụ thuộc vào Standard Peripheral Library hay HAL, giúp tối ưu hóa tốc độ và hiểu sâu về cấu trúc vi điều khiển.
---
## 🛠 Tính năng chính
1. **Giao tiếp UART1 (Ngắt phần cứng - Interrupt-based):**
   - Nhận lệnh từ máy tính/terminal theo thời gian thực sử dụng cơ chế ngắt `USART1_IRQHandler`.
   - Hỗ trợ xử lý chuỗi lệnh kết thúc bằng ký tự `!`.
2. **Điều khiển PWM (TIM3 - Channel 1 trên chân PA6):**
   - Cấu hình Timer 3 phát xung PWM tần số 1kHz.
   - Cho phép thay đổi Duty Cycle linh hoạt từ 0% đến 100%.
3. **Bộ xử lý lệnh (Command Parser):**
   - `ON`: Bật LED/PWM theo mức giá trị đã lưu trước đó.
   - `OFF`: Tắt LED/PWM (đưa Duty Cycle về 0%).
   - `PWM:<giá trị>`: Thiết lập tỷ lệ xung PWM (ví dụ: `PWM:50!`).
   - `Status`: Phản hồi trạng thái hoạt động hiện tại (ON/OFF và mức phần trăm PWM) qua UART.
---
## 📂 Cấu trúc thư mục dự án
```text
bai5_tuan2/
│
├── CMSIS/                   # Thư mục chứa các file header lõi ARM Cortex-M3
│   ├── core_cm3.h
│   ├── stm32f10x.h
│   └── system_stm32f10x.h
│
├── main.c                   # Mã nguồn chính (Cấu hình ngoại vi, ngắt, logic lệnh)
├── startup_stm32f10x_md.s   # File khởi động hợp ngữ, định nghĩa bảng vector ngắt
├── stm32_flash.ld           # Linker script phân chia vùng nhớ Flash (64K) và RAM (20K)
├── Makefile                 # Kịch bản biên dịch tự động bằng GNU Make
└── README.md                # Tài liệu hướng dẫn dự án
