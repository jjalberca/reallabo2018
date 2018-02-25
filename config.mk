#-------------------------------------------------------------------------------
# Related directories and files
#-------------------------------------------------------------------------------
ASF_PATH       ?= xdk-asf-3.37
CROSS          ?= arm-none-eabi-
UPLOAD_PORT    ?= ttyACM0
TARGET         ?= firmware

SRC_DIR        = src
INCLUDE_DIR    = include


#-------------------------------------------------------------------------------
# Target selection
#-------------------------------------------------------------------------------
OPTIMIZATION  = -Os
OUTPUT_DIR    = release
OBJ_DIR       = $(OUTPUT_DIR)/build

VERBOSE      ?= 0

#-------------------------------------------------------------------------------
# ASF Includes
#-------------------------------------------------------------------------------
ASF_INC = \
	common/boards                          \
	common/services/clock                  \
	common/services/gpio                   \
	common/services/ioport                 \
	common/services/serial                 \
	common/services/serial/sam_uart        \
	common/utils                           \
	common/utils/stdio/stdio_serial        \
	common/services/delay                  \
	sam/boards                             \
	sam/boards/arduino_due_x               \
	sam/drivers/pio                        \
	sam/drivers/pmc                        \
	sam/drivers/pwm                        \
	sam/drivers/uart                       \
	sam/drivers/usart                      \
	sam/drivers/tc                         \
	sam/utils                              \
	sam/utils/cmsis/sam3x/include          \
	sam/utils/cmsis/sam3x/source/templates \
	sam/utils/header_files                 \
	sam/utils/preprocessor                 \
	thirdparty/CMSIS/Include

#-------------------------------------------------------------------------------
# ASF Additional sources
#-------------------------------------------------------------------------------
ASF_SRC = \
	common/services/clock/sam3x/sysclk.c                       \
	common/services/serial/usart_serial.c                      \
	common/utils/interrupt/interrupt_sam_nvic.c                \
	common/utils/stdio/read.c                                  \
	common/utils/stdio/write.c                                 \
	common/services/delay/sam/cycle_counter.c                  \
	sam/boards/arduino_due_x/init.c                            \
	sam/drivers/pio/pio.c                                      \
	sam/drivers/pio/pio_handler.c                              \
	sam/drivers/pmc/pmc.c                                      \
	sam/drivers/pmc/sleep.c                                    \
	sam/drivers/pwm/pwm.c                                      \
	sam/drivers/uart/uart.c                                    \
	sam/drivers/usart/usart.c                                  \
	sam/drivers/tc/tc.c                                        \
	sam/utils/cmsis/sam3x/source/templates/exceptions.c        \
	sam/utils/cmsis/sam3x/source/templates/gcc/startup_sam3x.c \
	sam/utils/cmsis/sam3x/source/templates/system_sam3x.c      \
	sam/utils/syscalls/gcc/syscalls.c

#-------------------------------------------------------------------------------
# Libraries
#-------------------------------------------------------------------------------
LIB_PATH =  \
	$(ASF_PATH)/thirdparty/CMSIS/Lib/GCC

LIBS =  \
	arm_cortexM3l_math        \
	m

#-------------------------------------------------------------------------------
# Flags
#-------------------------------------------------------------------------------
ARFLAGS =

ASFLAGS =

CFLAGS =

CPPFLAGS =

LDFLAGS =
