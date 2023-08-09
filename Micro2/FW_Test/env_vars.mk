# Generic config file
include ../arm-none-makefile.conf
#==! MCU !==#
CORTEX_M=0
STM_TYPE=F0
VERSION=1.11.4
MCU_FLOAT_ABI = softfp
#===========#
#==! Files ==!#
# C++ Headers
CXX_HEADS		= $(wildcard $(INCLUDE_DIR)/*.hpp)
# C Headers
CC_HEADS		= $(wildcard $(INCLUDE_DIR)/*.h)
# ASM Headers
ASM_HEADS		= $(wildcard $(INCLUDE_DIR)/*.S)
# C++ Sources
CXX_SRCS		= $(wildcard $(SOURCE_DIR)/*.cpp)
# C Sources
CC_SRCS			= $(wildcard $(SOURCE_DIR)/*.c)
# Must be added manually, unfortunately
CC_SRCS			+= \
$(HAL_DRIVERS)/Src/stm32f0xx_hal_tim.c \
$(HAL_DRIVERS)/Src/stm32f0xx_hal_tim_ex.c \
$(HAL_DRIVERS)/Src/stm32f0xx_hal_rcc.c \
$(HAL_DRIVERS)/Src/stm32f0xx_hal_rcc_ex.c \
$(HAL_DRIVERS)/Src/stm32f0xx_hal.c \
$(HAL_DRIVERS)/Src/stm32f0xx_hal_i2c.c \
$(HAL_DRIVERS)/Src/stm32f0xx_hal_i2c_ex.c \
$(HAL_DRIVERS)/Src/stm32f0xx_hal_gpio.c \
$(HAL_DRIVERS)/Src/stm32f0xx_hal_dma.c \
$(HAL_DRIVERS)/Src/stm32f0xx_hal_cortex.c \
$(HAL_DRIVERS)/Src/stm32f0xx_hal_pwr.c \
$(HAL_DRIVERS)/Src/stm32f0xx_hal_pwr_ex.c \
$(HAL_DRIVERS)/Src/stm32f0xx_hal_flash.c \
$(HAL_DRIVERS)/Src/stm32f0xx_hal_flash_ex.c \
$(HAL_DRIVERS)/Src/stm32f0xx_hal_exti.c
# Add dir to search
vpath %.c $(sort $(dir $(CC_SRCS)))
# ASM Sources
ASM_SRCS		= $(wildcard $(SOURCE_DIR)/*.s)
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
USER_DEFS		+= -D_DEBUG -DSTM32F030x6 -DUSE_HAL_DRIVER

# Linker Script
LINKER_SCRIPT	= $(wildcard *.ld)

# Optimization/debugging Flags
OPT_F			?= -Os
ifneq (, $(findstring -D_DEBUG, $(USER_DEFS)))
	OPT_F		+= -g -ggdb
endif
FLAGS			+= -mfloat-abi=$(MCU_FLOAT_ABI)
#=============================#
#== Finishing Linker Flags ==#
LDSCRIPTS		= -L. $(LINKER_DIR) -T$(LINKER_SCRIPT)
LDFLAGS			+= $(USE_NANO) $(USE_NOHOST) $(LDSCRIPTS)
LDFLAGS			+= $(MAP),--cref $(GC) -lnosys
#============================#
