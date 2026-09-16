TARGET = bai5

CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
RM = rm -f

CFLAGS = -mcpu=cortex-m3 -mthumb -O0 -Wall -g \
         -I. \
         -ICMSIS \
         -DSTM32F10X_MD

LDFLAGS = -mcpu=cortex-m3 -mthumb -specs=nosys.specs -T stm32_flash.ld -Wl,-Map=$(TARGET).map,--gc-sections

SRCS = main.c startup_stm32f10x_md.s

all: $(TARGET).elf $(TARGET).hex $(TARGET).bin

$(TARGET).elf: $(SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

clean:
	$(RM) $(TARGET).elf $(TARGET).hex $(TARGET).bin $(TARGET).map
