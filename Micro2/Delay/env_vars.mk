# Generic config file
include ../arm-none-makefile.conf
#==! MCU !==#
CORTEX_M=3
STM_TYPE=F1
VERSION=1.8.5
MCU_FLOAT_ABI = softfp
#===========#
#==! Files ==!#
# C++ Headers
CXX_HEADS		= $(wildcard $(INCLUDE_DIR)/*.hpp)
CXX_HEADS		+= $(wildcard $(COMM_DIR)/Inc/*.hpp)
# C Headers
CC_HEADS		= $(wildcard $(INCLUDE_DIR)/*.h)
CC_HEADS		+= $(wildcard $(COMM_DIR)/Inc/*.h)
# ASM Headers
ASM_HEADS		= $(wildcard $(INCLUDE_DIR)/*.S)
ASM_HEADS		+= $(wildcard $(COMM_DIR)/Inc/*.S)
# C++ Sources
CXX_SRCS		= $(wildcard $(SOURCE_DIR)/*.cpp)
# C Sources
CC_SRCS			= $(wildcard $(SOURCE_DIR)/*.c)
# Must be added manually, unfortunately
CC_SRCS			+= \
$(HAL_DRIVERS)/Src/stm32f1xx_hal_gpio_ex.c \
$(HAL_DRIVERS)/Src/stm32f1xx_hal_tim.c \
$(HAL_DRIVERS)/Src/stm32f1xx_hal_tim_ex.c \
$(HAL_DRIVERS)/Src/stm32f1xx_hal.c \
$(HAL_DRIVERS)/Src/stm32f1xx_hal_rcc.c \
$(HAL_DRIVERS)/Src/stm32f1xx_hal_rcc_ex.c \
$(HAL_DRIVERS)/Src/stm32f1xx_hal_gpio.c \
$(HAL_DRIVERS)/Src/stm32f1xx_hal_dma.c \
$(HAL_DRIVERS)/Src/stm32f1xx_hal_cortex.c \
$(HAL_DRIVERS)/Src/stm32f1xx_hal_pwr.c \
$(HAL_DRIVERS)/Src/stm32f1xx_hal_flash.c \
$(HAL_DRIVERS)/Src/stm32f1xx_hal_flash_ex.c \
$(HAL_DRIVERS)/Src/stm32f1xx_hal_exti.c \
$(HAL_DRIVERS)/Src/stm32f1xx_hal_uart.c \
$(COMM_DIR)/Src/atraso.c \

# Add dir to search
vpath %.c $(sort $(dir $(CC_SRCS)))
# ASM Sources
ASM_SRCS		= $(wildcard $(SOURCE_DIR)/*.s)
ASM_SRCS		= $(wildcard *.s)

vpath %.s $(sort $(dir $(ASM_SRCS)))
# All Sources
SOURCES			:= $(CXX_SRCS)
SOURCES			+= $(CC_SRCS)
SOURCES			+= $(ASM_SRCS)
# All Headers
HEADS			:= $(CXX_HEADS)
HEADS			+= $(CC_HEADS)
HEADS			+= $(ASM_HEADS)
# C++ Objects
OBJECTS			= $(addprefix $(BUILD_DIR)/,$(notdir $(CXX_SRCS:.cpp=.o)))
# C Objects
OBJECTS			+= $(addprefix $(BUILD_DIR)/,$(notdir $(CC_SRCS:.c=.o)))
# ASM Objects
OBJECTS			+= $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SRCS:.s=.o)))
# Dependencies
DEP_FILES		= $(addprefix $(DEP_DIR)/, $(addsuffix .d, $(basename $(notdir $(OBJECTS)))))
# All Objects Needed For Compiling
NEEDED			:= $(SOURCES)
NEEDED			+= $(HEADS)
NEEDED			+= $(OBJECTS)
#=============#
#==! Build Config !==#
# Exec name
APP				= $(BIN_DIR)/app
#====================#
#==! Project Specific Flags ==!
# User Defines
USER_DEFS		+= -D_DEBUG -DSTM32F103x6 -DUSE_HAL_DRIVERS

# Linker Script
LINKER_SCRIPT	= $(wildcard *.ld)

# Optimization/debugging Flags
ifneq (, $(findstring -D_DEBUG, $(USER_DEFS)))
	OPT_F		+= -Og -ggdb
endif
OPT_F			?= -Os
FLAGS			+= -mfloat-abi=$(MCU_FLOAT_ABI)
#=============================#
#== Finishing Linker Flags ==#
LDSCRIPTS		= -L. $(LINKER_DIR) -T$(LINKER_SCRIPT)
LDFLAGS			+= $(USE_NANO) $(USE_NOHOST) $(LDSCRIPTS)
LDFLAGS			+= $(MAP),--cref $(GC) -lnosys
#============================#
