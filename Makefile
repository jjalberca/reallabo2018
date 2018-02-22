#-------------------------------------------------------------------------------
# Related directories and files
#-------------------------------------------------------------------------------

UPLOAD_PORT_DEF=ttyACM0
SHELL=/bin/bash
OUTPUT_NAME=firmware
SRC_DIR=src

INCLUDES =
INCLUDES += -I./include
INCLUDES += -I./hardware/libsam
INCLUDES += -I./hardware/CMSIS/Include
INCLUDES += -I./hardware/Device/ATMEL

#-------------------------------------------------------------------------------
# Target selection
#-------------------------------------------------------------------------------
ifdef DEBUG
OPTIMIZATION = -g -O0 -DDEBUG
OUTPUT_DIR=debug
else
OPTIMIZATION = -Os
OUTPUT_DIR=release
endif

OBJ_DIR=$(OUTPUT_DIR)/build

ifndef UPLOAD_PORT
UPLOAD_PORT = $(UPLOAD_PORT_DEF)
endif

#-------------------------------------------------------------------------------
#  Toolchain
#-------------------------------------------------------------------------------
CROSS_COMPILE = /opt/gcc-arm-none-eabi/bin/arm-none-eabi-
AR = $(CROSS_COMPILE)ar
CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
AS = $(CROSS_COMPILE)as
NM = $(CROSS_COMPILE)nm
LKELF = $(CROSS_COMPILE)g++
OBJCP = $(CROSS_COMPILE)objcopy
RM=rm -Rf
MKDIR=mkdir -p
UPLOAD_BOSSA=bossac


#-------------------------------------------------------------------------------
#  Flags
#-------------------------------------------------------------------------------
CFLAGS += -Wall --param max-inline-insns-single=500 -mcpu=cortex-m3 -mthumb -mlong-calls
CFLAGS += -ffunction-sections -fdata-sections -nostdlib -std=c99
CFLAGS += $(OPTIMIZATION) $(INCLUDES)
#CFLAGS += -Dprintf=iprintf
CPPFLAGS += -Wall --param max-inline-insns-single=500 -mcpu=cortex-m3 -mthumb -mlong-calls -nostdlib
CPPFLAGS += -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -std=c++98
CPPFLAGS += $(OPTIMIZATION) $(INCLUDES)
#CPPFLAGS += -Dprintf=iprintf
ASFLAGS = -mcpu=cortex-m3 -mthumb -Wall -g $(OPTIMIZATION) $(INCLUDES)
ARFLAGS = rcs
#TODO LDFLAGS

LNK_SCRIPT=./hardware/ldscripts/flash.ld
LIBSAM_ARCHIVE=./hardware/libsam/lib/libsam_sam3x8e_gcc_rel.a

#-------------------------------------------------------------------------------
# High verbosity flags
#-------------------------------------------------------------------------------
ifdef VERBOSE
CFLAGS += -Wall -Wchar-subscripts -Wcomment -Wformat=2 -Wimplicit-int
CFLAGS += -Werror-implicit-function-declaration -Wmain -Wparentheses
CFLAGS += -Wsequence-point -Wreturn-type -Wswitch -Wtrigraphs -Wunused
CFLAGS += -Wuninitialized -Wunknown-pragmas -Wfloat-equal -Wundef
CFLAGS += -Wshadow -Wpointer-arith -Wbad-function-cast -Wwrite-strings
CFLAGS += -Wsign-compare -Waggregate-return -Wstrict-prototypes
CFLAGS += -Wmissing-prototypes -Wmissing-declarations
CFLAGS += -Wformat -Wmissing-format-attribute -Wno-deprecated-declarations
CFLAGS += -Wredundant-decls -Wnested-externs -Winline -Wlong-long
CFLAGS += -Wunreachable-code
CFLAGS += -Wcast-align
CFLAGS += -Wmissing-noreturn
CFLAGS += -Wconversion
CPPFLAGS += -Wall -Wchar-subscripts -Wcomment -Wformat=2
CPPFLAGS += -Wmain -Wparentheses -Wcast-align -Wunreachable-code
CPPFLAGS += -Wsequence-point -Wreturn-type -Wswitch -Wtrigraphs -Wunused
CPPFLAGS += -Wuninitialized -Wunknown-pragmas -Wfloat-equal -Wundef
CPPFLAGS += -Wshadow -Wpointer-arith -Wwrite-strings
CPPFLAGS += -Wsign-compare -Waggregate-return -Wmissing-declarations
CPPFLAGS += -Wformat -Wmissing-format-attribute -Wno-deprecated-declarations
CPPFLAGS += -Wpacked -Wredundant-decls -Winline -Wlong-long
CPPFLAGS += -Wmissing-noreturn
CPPFLAGS += -Wconversion
UPLOAD_VERBOSE_FLAGS += -i -d
endif

#-------------------------------------------------------------------------------
# Source files and objects
#-------------------------------------------------------------------------------
C_SRC=$(wildcard $(SRC_DIR)/*.c)
C_OBJ=$(patsubst %.c, $(OBJ_DIR)/%.o, $(notdir $(C_SRC)))
CPP_SRC=$(wildcard $(SRC_DIR)/*.cpp)
CPP_OBJ=$(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(CPP_SRC)))
A_SRC=$(wildcard $(SRC_DIR)/*.s)
A_OBJ=$(patsubst %.s, $(OBJ_DIR)/%.o, $(notdir $(A_SRC)))

#-------------------------------------------------------------------------------
# Rules
#-------------------------------------------------------------------------------


#-------------------------------------------------------------------------------
all: binary

#-------------------------------------------------------------------------------
.PHONY: clean
clean:
	-@$(RM) $(OBJ_DIR)

#-------------------------------------------------------------------------------
$(OUTPUT_DIR):
	-@$(MKDIR) $(OUTPUT_DIR)

$(OBJ_DIR): $(OUTPUT_DIR)
	-@$(MKDIR) $(OBJ_DIR)

#-------------------------------------------------------------------------------
.PHONY: binary
binary:  $(OUTPUT_DIR)/$(OUTPUT_NAME).bin

#-------------------------------------------------------------------------------
# .bin ------> UPLOAD TO CONTROLLER
.PHONY: install
install: binary
	-@echo "Touch programming port ..."
	-@printf "\x00" > "/dev/$(UPLOAD_PORT)"
	-@echo "Waiting before uploading ..."
	-@sleep 1
	-@echo "Uploading ..."
	$(UPLOAD_BOSSA) $(UPLOAD_VERBOSE_FLAGS) --port="$(UPLOAD_PORT)" -U false -e -w -v -b "$(OUTPUT_DIR)/$(OUTPUT_NAME).bin" -R
	@echo "Done."

#-------------------------------------------------------------------------------
# .c -> .o
$(C_OBJ): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	"$(CC)" -c $(CFLAGS) $< -o $@

# .cpp -> .o
$(CPP_OBJ): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	"$(CXX)" -c $(CPPFLAGS) $< -o $@

# .s -> .o
$(A_OBJ): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.s | $(OBJ_DIR)
	"$(AS)" -c $(ASFLAGS) $< -o $@

# .o -> .a
$(OUTPUT_DIR)/$(OUTPUT_NAME).a: $(C_OBJ) $(CPP_OBJ) $(A_OBJ)
	"$(AR)" $(ARFLAGS) $@ $^
	"$(NM)" $@ > $@.txt

#  -> .elf
$(OUTPUT_DIR)/$(OUTPUT_NAME).elf: $(OUTPUT_DIR)/$(OUTPUT_NAME).a
	"$(LKELF)" -Os -Wl,--gc-sections -mcpu=cortex-m3 \
	  "-T$(LNK_SCRIPT)" "-Wl,-Map,$(OUTPUT_DIR)/$(OUTPUT_NAME).map" \
	  -o $@ \
	  "-L$(OUTPUT_DIR)" \
	  -lm -lgcc -mthumb -Wl,--cref -Wl,--check-sections -Wl,--gc-sections \
	  -Wl,--entry=Reset_Handler -Wl,--unresolved-symbols=report-all -Wl,--warn-common \
	  -Wl,--warn-section-align -Wl,--warn-unresolved-symbols \
	  -Wl,--start-group \
	  $^ $(LIBSAM_ARCHIVE) \
	  -Wl,--end-group

# .elf -> .bin
$(OUTPUT_DIR)/$(OUTPUT_NAME).bin: $(OUTPUT_DIR)/$(OUTPUT_NAME).elf
	"$(OBJCP)" -O binary $(OUTPUT_DIR)/$(OUTPUT_NAME).elf $(OUTPUT_DIR)/$(OUTPUT_NAME).bin
