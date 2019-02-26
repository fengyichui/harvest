
# makefile

############################################################################################

# Include other makefile files
include .config
#include hardware/hardware.mk

# Project
PROJECT		= hs6620

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

# build obj files direcotry
BUILDDIR	= build

############################################################################################

# verbose mode
ifeq ($(V),1)
TRACE_CC  =
TRACE_LD  =
TRACE_AS  =
Q=
else
TRACE_CC  = @echo "	CC    $<"
TRACE_AS  = @echo "	AS    $<"
TRACE_LD  = @echo "Linking $@ ..."
Q=@
endif

# Target thing
TARGET					:= $(PROJECT)
TARGET_ELF	 			:= $(TARGET).elf
TARGET_BIN				:= $(TARGET).bin
TARGET_HEX				:= $(TARGET).hex
TARGET_MAP				:= $(TARGET).map
TARGET_DIS				:= $(TARGET).dis
OBJCPFLAGS_ELF_TO_BIN	 = -Obinary
OBJCPFLAGS_ELF_TO_HEX	 = -Oihex
OBJCPFLAGS_HEX_TO_BIN	 = -Obinary -Iihex

# Tools
CC			= arm-none-eabi-gcc
AS			= arm-none-eabi-as
LD			= arm-none-eabi-ld
AR			= arm-none-eabi-ar
OBJCP		= arm-none-eabi-objcopy
OBJDUMP		= arm-none-eabi-objdump
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
CCFLAGS		= -mcpu=$(CPU) -mthumb -g3 -std=gnu99 $(WARNINGS) $(CC_OPTIMIZE) #-fPIC #-mno-pic-data-is-text-relative -msingle-pic-base -mpic-register=r9
ASFLAGS		= -mcpu=$(CPU) -mthumb
LDFLAGS		= -nostartfiles -static $(CCFLAGS) $(LD_OPTIMIZE) -T$(LDSCRIPT) -Wl,-Map=$(TARGET_MAP)

# Librarys
LDLIBS		= --specs=nano.specs -lc -lnosys -lm

# Files
FILES_C_OBJ = $(patsubst %,$(BUILDDIR)/%,$(filter %.o, $(SOURCES:%.c=%.o)))
FILES_S_OBJ = $(patsubst %,$(BUILDDIR)/%,$(filter %.o, $(SOURCES:%.s=%.o)))
FILES_DEPEND = $(patsubst %,$(BUILDDIR)/%,$(filter %.d, $(SOURCES:%.c=%.d)))

# PHONY
.PHONY: all clean print

# Target
all: $(TARGET_HEX) $(TARGET_BIN)

# bin
$(TARGET_BIN) : $(TARGET_HEX)
	$(Q)$(OBJCP) $(OBJCPFLAGS_HEX_TO_BIN) $< $@
	@echo "Build done"

# hex
$(TARGET_HEX) : $(TARGET_ELF)
	$(Q)$(OBJCP) $(OBJCPFLAGS_ELF_TO_HEX) $< $@

# elf
$(TARGET_ELF) : $(FILES_C_OBJ) $(FILES_S_OBJ)
	$(TRACE_LD)
	$(Q)$(CC) $+ $(LDFLAGS) $(LDLIBS) -o $@
	$(Q)$(OBJSIZE) -B -d $@
	$(Q)$(OBJDUMP) -d $@ > $(TARGET_DIS)

#
# %.o : %.c
#   Delete the built-in rules for building object files from .c files, so that our
#   rule is used instead. Do the same for the other built-in rules.
#
# ... %.d
#   Declare the generated dependency file as a prerequisite of the target, so that
#   if it’s missing the target will be rebuilt.
#
# Dependency flag:
# -MT $@
#   Set the name of the target in the generated dependency file.
# -MMD
#   Generate dependency information as a side-effect of compilation, not instead of
#   compilation. This version omits system headers from the generated dependencies:
#   if you prefer to preserve system headers as prerequisites, use -MD.
# -MP
#   Adds a target for each prerequisite in the list, to avoid errors when deleting files.
# -MF $*.Td
#   Write the generated dependency file to a temporary location $(DEPDIR)/$*.Td.
#
# @mkdir -p $(@D) >/dev/null
#   Create building direcory tree
#
# @mv -f $*.Td $*.d && touch $@ :
#   First rename the generated temporary dependency file to the real dependency file.
#   We do this in a separate step so that failures during the compilation won’t leave
#   a corrupted dependency file. Second touch the object file; it’s been reported that
#   some versions of GCC may leave the object file older than the dependency file,
#   which causes unnecessary rebuilds.
#
$(BUILDDIR)/%.o : %.c
$(BUILDDIR)/%.o : %.c $(BUILDDIR)/%.d
	$(TRACE_CC)
	@mkdir -p $(@D) >/dev/null
	$(Q)$(CC) -MT $@ -MMD -MP -MF $(BUILDDIR)/$*.Td $(CCFLAGS) $(CPPFLAGS) -c $< -o $@
	@mv -f $(BUILDDIR)/$*.Td $(BUILDDIR)/$*.d

# s -> o
$(BUILDDIR)/%.o : %.s
	$(TRACE_AS)
	@mkdir -p $(@D) >/dev/null
	$(Q)$(AS) $(ASFLAGS) -c $< -o $@

# Create a pattern rule with an empty recipe,
# so that make won’t fail if the dependency file doesn’t exist.
$(BUILDDIR)/%.d: ;
# Mark the dependency files precious to make,
# so they won’t be automatically deleted as intermediate files.
.PRECIOUS: $(BUILDDIR)/%.d

# Include dependent (*.d)
include $(FILES_DEPEND)

# some info
print:
	@echo $(SOURCES)
	@echo $(FILES_DEPEND)

# Clean
clean:
	$(Q)rm -rf $(BUILDDIR)
	$(Q)find -type f -regex ".*\.\(hex\|bin\|elf\|map\|o\|d\|Td\)$$" | xargs rm -f

# KConfig
%config:
	@$(MAKE) --no-print-directory -f script/kconfig/Makefile $@

