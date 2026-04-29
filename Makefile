# Компилятор и утилиты
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size
CPPCHECK = cppcheck
LUAJIT = luajit
RENODE = renode

# Использовать ccache если доступен
CCACHE := $(shell which ccache 2>/dev/null)
ifdef CCACHE
    CC := $(CCACHE) $(CC)
endif

# Конфигурация Gitea
GITEA_SERVER ?= gitea.example.com
GITEA_ORG ?= actions

# Флаги зависят от компилятора
ifeq ($(USE_CLANG),1)
    # Отменяем предыдущее обертывание
    ifdef CCACHE
        CC = $(CCACHE) clang-18
    else
        CC = clang-18
    endif
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
.PHONY: all check test clean ccache-stats

all: $(BUILDDIR) $(TARGET).elf $(TARGET).hex $(TARGET).bin
	@echo "Build complete with $(CC)!"
	$(SIZE) $(TARGET).elf

ccache-stats:
ifdef CCACHE
	ccache -s
else
	@echo "ccache not installed"
endif

test: all
	@echo "Running tests in Renode..."
	$(RENODE) test/k64_test.resc --console

check:
	@echo "Running cppcheck..."
	$(CPPCHECK) --enable=all \
	            --inconclusive \
	            --std=c11 \
	            --suppress=missingIncludeSystem \
	            --suppress=unusedFunction \
	            --suppress=unusedStructMember \
	            --suppress=comparePointers \
	            --suppress=constVariablePointer \
	            --suppress=constParameterPointer \
	            --suppress=unmatchedSuppression \
	            --suppress=checkersReport \
	            --inline-suppr \
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
ifdef CCACHE
	ccache -C
endif
