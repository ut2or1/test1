# Компилятор и утилиты
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# Флаги
CPUFLAGS = -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mthumb
CFLAGS = $(CPUFLAGS) -Wall -Wextra -O0 -g3 -ffunction-sections -fdata-sections
LDFLAGS = $(CPUFLAGS) -T linker/MK64FX512.ld -Wl,--gc-sections -nostartfiles -specs=nosys.specs

# Директории
SRCDIR = src
BUILDDIR = build
TARGET = $(BUILDDIR)/k64-blinky

# Исходники
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(SRCS:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

# Правила
.PHONY: all clean

all: $(BUILDDIR) $(TARGET).elf $(TARGET).hex $(TARGET).bin
	@echo "Build complete!"
	$(SIZE) $(TARGET).elf

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -Iinc -c $< -o $@

$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -rf $(BUILDDIR)
