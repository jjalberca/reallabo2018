# List of available make goals:
#
# all                     Default target, builds the project
# clean                   Clean up the project
# binary                  Build project binary
# flash	                  Upload project to the board

#-------------------------------------------------------------------------------
# User config
#-------------------------------------------------------------------------------

SHELL = /bin/bash
include config.mk


#-------------------------------------------------------------------------------
# Board Config
#-------------------------------------------------------------------------------

# Defines
CPPFLAGS += \
	-D ARM_MATH_CM3=true                               \
	-D BOARD=ARDUINO_DUE_X                             \
	-D __SAM3X8E__                                     \
	-D printf=iprintf                                  \
	-D scanf=iscanf

# Upload flags
BOSSA_FLAGS = -U false -e -w -v -b

# Target CPU architecture: cortex-m3, cortex-m4
ARCH       = cortex-m3

# Target part: none, sam3n4 or sam4l4aa
PART       = sam3x8e

# Path relative to top level directory pointing to a linker script.
LINKER_SCRIPT = sam/utils/linker_scripts/sam3x/sam3x8/gcc/flash.ld

# Use pipes instead of temporary files for communication between processes
CFLAGS    += -pipe
ASFLAGS   += -pipe
LDFLAGS   += -pipe

# Archiver flags.
ARFLAGS   += rcs

# Always enable warnings. And be very careful about implicit
# declarations.
CFLAGS    += -Wall -Wstrict-prototypes -Wmissing-prototypes
CFLAGS    += -Werror-implicit-function-declaration
CXXFLAGS  += -Wall

# IAR doesn't allow arithmetic on void pointers, so warn about that.
CFLAGS    += -Wpointer-arith
CXXFLAGS  += -Wpointer-arith

# Preprocessor flags.
INCLUDES  += $(foreach INC,$(addprefix $(ASF_PATH)/,$(ASF_INC)),-I$(INC))
INCLUDES  += -I$(INCLUDE_DIR)
CPPFLAGS  += $(INCLUDES)
ASFLAGS   += $(foreach INC, $(INCLUDES),'-Wa,$(INC)')

# CPU specific flags.
CPUFLAGS  += -mcpu=$(ARCH) -mthumb -D=__$(PART)__

# Dependency file flags.
DEPFLAGS   = -MD -MP -MQ $@

# Optimization specific flags.
OPTFLAGS  = $(OPTIMIZATION)

# Always preprocess assembler files.
ASFLAGS   += -x assembler-with-cpp
# Compile C files using the GNU99 standard.
#CFLAGS    += -std=gnu99
# Compile C++ files using the GNU++98 standard.
#CXXFLAGS  += -std=gnu++98

# Don't use strict aliasing (very common in embedded applications).
CFLAGS          += -fno-strict-aliasing
cxxflags-gnu-y  += -fno-strict-aliasing

# Separate each function and data into its own separate section to allow
# garbage collection of unused sections.
CFLAGS    += -ffunction-sections -fdata-sections
CXXFLAGS  += -ffunction-sections -fdata-sections

# Various cflags.
CFLAGS    += -Wchar-subscripts -Wcomment -Wformat=2 -Wimplicit-int
CFLAGS    += -Wmain -Wparentheses
CFLAGS    += -Wsequence-point -Wreturn-type -Wswitch -Wtrigraphs -Wunused
CFLAGS    += -Wuninitialized -Wunknown-pragmas -Wfloat-equal -Wundef
CFLAGS    += -Wshadow -Wbad-function-cast -Wwrite-strings
CFLAGS    += -Wsign-compare -Waggregate-return
CFLAGS    += -Wmissing-declarations
CFLAGS    += -Wformat -Wmissing-format-attribute -Wno-deprecated-declarations
CFLAGS    += -Wpacked -Wredundant-decls -Wnested-externs -Winline -Wlong-long
CFLAGS    += -Wunreachable-code
CFLAGS    += -Wcast-align
CFLAGS    += --param max-inline-insns-single=500

# Garbage collect unreferred sections when linking.
LDFLAGS   += -Wl,--gc-sections

# Use the linker script.
LDFLAGS   += -Wl,-T $(ASF_PATH)/$(LINKER_SCRIPT)

# Output a link map file and a cross reference table
LDFLAGS   += -Wl,-Map=$(OUTPUT_DIR)/$(TARGET).map,--cref

# Add library search paths relative to the top level directory.
LDFLAGS   += $(foreach _LIB_PATH,$(LIB_PATH),-L$(_LIB_PATH))
LIBFLAGS  := $(foreach LIB,$(LIBS),-l$(LIB))

#-------------------------------------------------------------------------------
#  Command Output
#-------------------------------------------------------------------------------
ifeq ($(VERBOSE), 1)
  Q =
else
  Q = @
endif

#-------------------------------------------------------------------------------
#  Toolchain
#-------------------------------------------------------------------------------
AR        := $(Q)$(CROSS)ar
AS        := $(Q)$(CROSS)as
CC        := $(Q)$(CROSS)gcc
CPP       := $(Q)$(CROSS)gcc -E
CXX       := $(Q)$(CROSS)g++
LD        := $(Q)$(CROSS)g++
NM        := $(Q)$(CROSS)nm
OBJCOPY   := $(Q)$(CROSS)objcopy
OBJDUMP   := $(Q)$(CROSS)objdump
SIZE      := $(Q)$(CROSS)size
GDB       := $(Q)$(CROSS)gdb

ECHO      := $(Q)echo
RM        := $(Q)rm -Rf
MKDIR     := $(Q)mkdir -p
BOSSA     := $(Q)bossac

MAKEFLAGS += -rR

a_flags  = $(CPUFLAGS) $(DEPFLAGS) $(CPPFLAGS) $(ASFLAGS) -D__ASSEMBLY__
c_flags  = $(CPUFLAGS) $(DBGFLAGS) $(DEPFLAGS) $(OPTFLAGS) $(CPPFLAGS) $(CFLAGS)
cxx_flags= $(CPUFLAGS) $(DBGFLAGS) $(DEPFLAGS) $(OPTFLAGS) $(CPPFLAGS) $(CXXFLAGS)
l_flags  = -Wl,--entry=Reset_Handler -Wl,--cref $(CPUFLAGS) $(OPTFLAGS) $(LDFLAGS)
ar_flags = $(ARFLAGS)

# Source files list and part informations must already be included before
# running this makefile
#-------------------------------------------------------------------------------
# Create object files
#-------------------------------------------------------------------------------
# ASF objects
ASF_CSRC  := $(filter %.c,$(ASF_SRC))
ASF_ASRC  := $(filter %.S,$(ASF_SRC))
ASF_COBJS := $(addprefix $(OBJ_DIR)/, $(addsuffix .o,$(basename $(ASF_CSRC))))
ASF_AOBJS := $(addprefix $(OBJ_DIR)/, $(addsuffix .o,$(basename $(ASF_ASRC))))
ASF_DEP   := $(foreach f,$(ASF_COBJS) ,$(basename $(f)).d)
ASF_DEP   += $(foreach f,$(ASF_AOBJS) ,$(basename $(f)).d)
# Project objects
CSRCS     := $(wildcard $(SRC_DIR)/*.c)
COBJS     := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(CSRCS))
DEP       := $(foreach f,$(COBJS), $(basename $(f)).d)

OBJS      := $(COBJS) $(ASF_COBJS) $(ASF_AOBJS)
DEP_FILES := $(wildcard $(DEP) $(ASF_DEP))

# Default target.
.PHONY: all
all: autocomplete binary

.PHONY: binary
binary: $(OUTPUT_DIR)/$(TARGET).bin

.PHONY: clean
clean:
	$(ECHO) "Cleaning: $(OUTPUT_DIR)"
	$(RM) $(OUTPUT_DIR)
	$(RM) .clang_complete


# Create object files from C source files.
$(ASF_COBJS): $(OBJ_DIR)/%.o: $(ASF_PATH)/%.c | $(OBJ_DIR)
	$(ECHO) "C compiling: $@"
	$(MKDIR) -- $(patsubst %$(notdir $(@)),%,$(@))
	$(CC) $(c_flags) -c $< -o $@

$(COBJS): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(ECHO) "C compiling: $@"
	$(CC) $(c_flags) -c $< -o $@

# Preprocess and assemble: create object files from assembler source files.
$(ASF_AOBJS): $(OBJ_DIR)/%.o: $(ASF_PATH)/%.S | $(OBJ_DIR)
	$(ECHO) "AS compiling: $@"
	$(MKDIR) -- $(patsubst %$(notdir $(@)),%,$(@))
	$(CC) $(a_flags) -c $< -o $@

# Include all dependency files to add depedency to all header files in use.
include $(DEP_FILES)

# Create elf file
$(OUTPUT_DIR)/$(TARGET).elf : $(OBJS) | $(OUTPUT_DIR)
	$(ECHO) "Linking: $@"
	$(LD) $(l_flags) $(OBJS) $(libflags-gnu-y) -o $@
	$(ECHO) ELF Size:
	$(SIZE) -Ax $@
	$(SIZE) -Bx $@

# Create extended function listing from target output file.
%.lss: $(OUTPUT_DIR)/$(TARGET).elf
	$(ECHO) "Extended listing: $@"
	$(OBJDUMP) -h -S $< > $@

# Create symbol table from target output file.
%.sym: $(OUTPUT_DIR)/$(TARGET).elf
	$(ECHO) "Symbol table: $@"
	$(NM) -n $< > $@

# Create Intel HEX image from ELF output file.
%.hex: $(OUTPUT_DIR)/$(TARGET).elf
	$(ECHO) "HEX image: $@"
	$(OBJCOPY) -O ihex $(flashflags-gnu-y)  $< $@

# Create binary image from ELF output file.
%.bin: $(OUTPUT_DIR)/$(TARGET).elf
	$(ECHO) "Binary image: $@"
	$(OBJCOPY) -O binary $< $@

$(OUTPUT_DIR):
	$(MKDIR) $@

$(OBJ_DIR):
	$(MKDIR) $@

#-------------------------------------------------------------------------------
# Flash
#-------------------------------------------------------------------------------
flash: binary
	$(ECHO) "Touch programming port ..."
	-@printf "\x00" > "/dev/$(UPLOAD_PORT)"
	$(ECHO) "Waiting before uploading ..."
	-@sleep 1
	$(ECHO) "Uploading ..."
	$(BOSSA) --port=$(UPLOAD_PORT) $(BOSSA_FLAGS) $(OUTPUT_DIR)/$(TARGET).bin -R

#-------------------------------------------------------------------------------
# Autocomplete
#-------------------------------------------------------------------------------
.PHONY: autocomplete
autocomplete: .clang_complete
.clang_complete:
	$(ECHO) "Generating $@"
	$(ECHO) $(INCLUDES) | sed -E -e 's/[[:blank:]]+/\n/g' > .clang_complete
