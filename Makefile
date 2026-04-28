# Определяем компилятор
CC ?= arm-none-eabi-gcc
OBJCOPY ?= arm-none-eabi-objcopy
SIZE ?= arm-none-eabi-size
CPPCHECK = cppcheck

# Флаги зависят от компилятора
ifeq ($(USE_CLANG),1)
    CC = clang-18
    OBJCOPY = llvm-objcopy-18
    SIZE = llvm-size-18
    CPUFLAGS = --target=arm-none-eabi -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
    CFLAGS += -fno-common
    LDFLAGS += -nostdlib
else
    CPUFLAGS = -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mthumb
    LDFLAGS += -specs=nosys.specs
endif

# Общие флаги
CFLAGS += $(CPUFLAGS) -Wall -Wextra -O0 -g3 -ffunction-sections -fdata-sections
LDFLAGS += $(CPUFLAGS) -T linker/MK64FX512.ld -Wl,--gc-sections -nostartfiles

# Директории
SRCDIR = src
INCDIR = inc
BUILDDIR = build
TARGET = $(BUILDDIR)/k64-blinky

# Исходники
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(SRCS:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

# Правила
.PHONY: all check clean

all: $(BUILDDIR) $(TARGET).elf $(TARGET).hex $(TARGET).bin
	@echo "Build complete with $(CC)!"
	$(SIZE) $(TARGET).elf

check:
	@echo "Running cppcheck..."
	$(CPPCHECK) --enable=all \
	            --inconclusive \
	            --std=c11 \
	            --suppress=missingIncludeSystem \
	            --suppress=unusedFunction \
	            --error-exitcode=1 \
	            -I$(INCDIR) \
	            $(SRCDIR)/
	@echo "Static analysis passed!"

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -rf $(BUILDDIR)
