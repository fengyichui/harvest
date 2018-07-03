
# makefile

############################################################################################

# Project
PROJECT		= hs6620_driver

# Base
BASE_DIR	= .

# Sources
SOURCES		= $(BASE_DIR)/main.c \
			  $(BASE_DIR)/retarget.c \
			  $(BASE_DIR)/startup.c

# Includes
INCLUDES	= -I$(BASE_DIR)/ \
			  -I$(BASE_DIR)/include/ \
			  -I$(BASE_DIR)/cmsis/ \

# Defines
DEFINES		=

# Link script
LDSCRIPT	= $(BASE_DIR)/link.ld

# Optimization: 0, 1, 2, 3, size
OPTIMIZE	= 1

# CPU
CPU			= cortex-m3

############################################################################################

# verbose mode
ifeq ($(V),1)
TRACE_CC  =
TRACE_LD  =
TRACE_AS  =
Q=
else
TRACE_CC  = @echo "CC    $<"
TRACE_AS  = @echo "AS    $<"
TRACE_LD  = @echo "Linking $@ ..."
Q=@
endif

# Target thing
TARGET					:= $(PROJECT)
TARGET_ELF	 			:= $(TARGET).elf
TARGET_BIN				:= $(TARGET).bin
TARGET_HEX				:= $(TARGET).hex
TARGET_MAP				:= $(TARGET).map
OBJCPFLAGS_ELF_TO_BIN	 = -Obinary
OBJCPFLAGS_ELF_TO_HEX	 = -Oihex
OBJCPFLAGS_HEX_TO_BIN	 = -Obinary -Iihex

# Tools
CC			= arm-none-eabi-gcc
AS			= arm-none-eabi-as
LD			= arm-none-eabi-ld
AR			= arm-none-eabi-ar
OBJCP		= arm-none-eabi-objcopy
OBJSIZE		= arm-none-eabi-size

# Optimize
ifeq ($(OPTIMIZE), size)
CC_OPTIMIZE = -Os -fdata-sections -ffunction-sections
LD_OPTIMIZE = -Wl,--gc-sections
else
CC_OPTIMIZE = -O$(OPTIMIZE)
LD_OPTIMIZE =
endif

# Warnings
WARNINGS 	= -Wall -Wformat=0 -Wstrict-aliasing=0

# Flags
CPPFLAGS	= $(INCLUDES) $(DEFINES)
CCFLAGS		= -mcpu=$(CPU) -mthumb -g3 -std=gnu99 $(WARNINGS) $(CC_OPTIMIZE)
ASFLAGS		= -mcpu=$(CPU) -mthumb
LDFLAGS		= -nostartfiles -static $(CCFLAGS) $(LD_OPTIMIZE) -T$(LDSCRIPT) -Wl,-Map=$(TARGET_MAP)

# Librarys
LDLIBS		= --specs=nano.specs -lc -lnosys -lm

# PHONY
.PHONY: all clean print

# Target
all: $(TARGET_HEX) $(TARGET_BIN)

$(TARGET_BIN) : $(TARGET_HEX)
	$(Q)$(OBJCP) $(OBJCPFLAGS_HEX_TO_BIN) $< $@
	@echo "Build done"

$(TARGET_HEX) : $(TARGET_ELF)
	$(Q)$(OBJCP) $(OBJCPFLAGS_ELF_TO_HEX) $< $@

$(TARGET_ELF) : $(filter %.o, $(SOURCES:%.c=%.o)) $(filter %.o, $(SOURCES:%.s=%.o))
	$(TRACE_LD)
	$(Q)$(CC) $+ $(LDFLAGS) $(LDLIBS) -o $@
	$(Q)$(OBJSIZE) -B -d $@

# c -> o (c -> d)
%.o : %.c
	$(TRACE_CC)
	$(Q)$(CC) $(CCFLAGS) $(CPPFLAGS) -c $< -o $@
	@$(CC) -MM $(CCFLAGS) $(CPPFLAGS) $*.c > $*.d
	@mv -f $*.d $*.d.tmp
	@sed -i 's/\r$$//' $*.d.tmp
	@sed 's|.*:|$*.o:|' $*.d.tmp > $*.d
	@sed 's/.*://; s/\\$$//' $*.d.tmp | fmt -1 | sed 's/^ *//; s/$$/:/' >> $*.d
	@rm -f $*.d.tmp

# s -> o
%.o : %.s
	$(TRACE_AS)
	$(Q)$(AS) $(ASFLAGS) -c $< -o $@

# Include dependent (*.d)
-include $(filter %.d, $(SOURCES:%.c=%.d))

# some info
print:
	@echo $(SOURCES)

# Clean
clean:
	$(Q)find -type f -regex ".*\.\(hex\|bin\|elf\|map\|o\|d\)$$" | xargs rm -f

# KConfig
%config:
	@$(MAKE) --no-print-directory -f script/kconfig/Makefile $@

