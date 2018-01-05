MK := mkdir
RM := rm -rf

export OUTPUT_FILENAME
MAKEFILE_NAME := $(MAKEFILE_LIST)
MAKEFILE_DIR := $(dir $(MAKEFILE_NAME) ) 

CC       		:= "arm-none-eabi-gcc"
AS       		:= "arm-none-eabi-as"
AR       		:= "arm-none-eabi-ar" -r
LD       		:= "arm-none-eabi-ld"
NM       		:= "arm-none-eabi-nm"
OBJDUMP  		:= "arm-none-eabi-objdump"
OBJCOPY  		:= "arm-none-eabi-objcopy"
SIZE    		:= "arm-none-eabi-size"

#function for removing duplicates in a list
remduplicates = $(strip $(if $1,$(firstword $1) $(call remduplicates,$(filter-out $(firstword $1),$1))))

# c source files
C_SOURCE_FILES += \
$(abspath ../common/main.c) \
$(abspath system.c) \
$(abspath hal.c) \
$(abspath FreeRTOS/Source/croutine.c) \
$(abspath FreeRTOS/Source/event_groups.c) \
$(abspath FreeRTOS/Source/list.c) \
$(abspath FreeRTOS/Source/queue.c) \
$(abspath FreeRTOS/Source/timers.c) \
$(abspath FreeRTOS/Source/tasks.c) \
$(abspath FreeRTOS/Source/portable/MemMang/heap_4.c) \
$(abspath FreeRTOS/Source/port.c) \
$(abspath SEGGER/SEGGER_RTT.c) \
$(abspath SEGGER/SEGGER_RTT_printf.c)

# asm source files
ASM_SOURCE_FILES += \
$(abspath startup_stm32f103x6.s)

# stm32f103c8t6 linker files
LINKER_SCRIPT += \
$(abspath STM32F103X6_FLASH.ld)

# include paths
INC_PATHS += -I$(abspath CMSIS/Device/ST/)
INC_PATHS += -I$(abspath CMSIS/Device/ST/STM32F1xx/Include/)
INC_PATHS += -I$(abspath CMSIS/Include/)
INC_PATHS += -I$(abspath ../common/)
INC_PATHS += -I$(abspath FreeRTOS/Source/include/)
INC_PATHS += -I$(abspath SEGGER/)

OBJECT_DIRECTORY = .build
LISTING_DIRECTORY = $(OBJECT_DIRECTORY)
OUTPUT_BINARY_DIRECTORY = $(OBJECT_DIRECTORY)

# Sorting removes duplicates
BUILD_DIRECTORIES := $(sort $(OBJECT_DIRECTORY) $(OUTPUT_BINARY_DIRECTORY) $(LISTING_DIRECTORY) )

#flags common to all targets
CFLAGS  = -DSTM32F103x6
CFLAGS += -mcpu=cortex-m3
CFLAGS += -mthumb -mabi=aapcs --std=gnu99
CFLAGS += -Wall -Werror -O0
CFLAGS += -mfloat-abi=soft
# keep every function in separate section. This will allow linker to dump unused functions
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin
CFLAGS += -g

# keep every function in separate section. This will allow linker to dump unused functions
LDFLAGS += -Xlinker -Map=$(LISTING_DIRECTORY)/$(OUTPUT_FILENAME).map
LDFLAGS += -mthumb -mabi=aapcs -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m3
# let linker to dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs -lc -lnosys
LDFLAGS += -g

# Assembler flags
ASMFLAGS += -x assembler-with-cpp
default: clean stm32f103c8t6

all: cleanobj
	$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e cleanobj
	$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e stm32f103c8t6

C_SOURCE_FILE_NAMES = $(notdir $(C_SOURCE_FILES))
ASM_SOURCE_FILE_NAMES = $(notdir $(ASM_SOURCE_FILES))

C_OBJ = $(addprefix $(OBJECT_DIRECTORY)/, $(C_SOURCE_FILE_NAMES:.c=.o) )
C_PATHS = $(call remduplicates, $(dir $(C_SOURCE_FILES) ) )

ASM_OBJ = $(addprefix $(OBJECT_DIRECTORY)/, $(ASM_SOURCE_FILE_NAMES:.s=.o) ) 
ASM_PATHS = $(call remduplicates, $(dir $(ASM_SOURCE_FILES) ))

vpath %.c $(C_PATHS)
vpath %.s $(ASM_PATHS)

OBJECTS = $(C_OBJ) $(ASM_OBJ)

stm32f103c8t6: OUTPUT_FILENAME := stm32f103c8t6
stm32f103c8t6: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo $(C_OBJ)
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e finalize

## Create build directories
$(BUILD_DIRECTORIES):
	echo $(MAKEFILE_NAME)
	$(MK) $@

# Create objects from C SRC files
$(OBJECT_DIRECTORY)/%.o: %.c
	@echo Compiling file: $(notdir $<)
	$(CC) $(CFLAGS) $(INC_PATHS) -c -o $@ $<

# Assemble files
$(OBJECT_DIRECTORY)/%.o: %.s
	@echo Compiling file: $(notdir $<)
	$(CC) $(ASMFLAGS) $(INC_PATHS) -c -o $@ $<


# Link
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out


## Create binary .bin file from the .out file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	@echo Preparing: $(OUTPUT_FILENAME).bin
	$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin

## Create binary .hex file from the .out file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	@echo Preparing: $(OUTPUT_FILENAME).hex
	$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex

finalize: genbin genhex echosize

genbin:
	@echo Preparing: $(OUTPUT_FILENAME).bin
	$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin

## Create binary .hex file from the .out file
genhex: 
	@echo Preparing: $(OUTPUT_FILENAME).hex
	$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex

echosize:
	-@echo ""
	$(SIZE) $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	-@echo ""

clean:
	$(RM) $(BUILD_DIRECTORIES)

cleanobj:
	$(RM) $(BUILD_DIRECTORIES)/*.o

flash:
	openocd -f /usr/local/Cellar/open-ocd/0.10.0/share/openocd/scripts/interface/jlink.cfg -f /usr/local/Cellar/open-ocd/0.10.0/share/openocd/scripts/target/stm32f1x.cfg -c "program .build/stm32f103c8t6.bin 0x08000000 verify" -c "reset run" -c "exit"
