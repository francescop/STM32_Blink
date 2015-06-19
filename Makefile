# Sources
SRCS = main.c stm32f4xx_it.c system_stm32f4xx.c syscalls.c utils.c

# Project name

PROJ_NAME=main
OUTPATH=build

# OpenOCD conf file
OPENOCD_CONFIG_PATH=/home/francesco/openocd/stm32f4discovery.cfg
OPENOCD_LOGFILE_PATH=/dev/null

###################################################

# Check for valid float argument
# NOTE that you have to run make clan after
# changing these as hardfloat and softfloat are not
# binary compatible
ifneq ($(FLOAT_TYPE), hard)
	ifneq ($(FLOAT_TYPE), soft)
	override FLOAT_TYPE = hard
	#override FLOAT_TYPE = soft
endif
endif

###################################################

BINPATH=/usr/bin
CC=$(BINPATH)/arm-none-eabi-gcc
OBJCOPY=$(BINPATH)/arm-none-eabi-objcopy
SIZE=$(BINPATH)/arm-none-eabi-size

CFLAGS  = -std=gnu99 -g -O2 -Wall -Tstm32_flash.ld --specs=nano.specs --specs=rdimon.specs -lc -lc -lrdimon
CFLAGS += -mlittle-endian -mthumb -mthumb-interwork -nostartfiles -mcpu=cortex-m4

ifeq ($(FLOAT_TYPE), hard)
	CFLAGS += -fsingle-precision-constant -Wdouble-promotion
	CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
	#CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
else
	CFLAGS += -msoft-float
endif

###################################################

vpath %.c src
vpath %.a lib

ROOT=$(shell pwd)

CFLAGS += -Iinc -Ilib -Ilib/inc 
CFLAGS += -Ilib/inc/core -Ilib/inc/peripherals 

SRCS += lib/startup_stm32f4xx.s # add startup file to build

OBJS = $(SRCS:.c=.o)

###################################################

.PHONY: lib proj

all: lib proj
	$(SIZE) $(OUTPATH)/$(PROJ_NAME).elf

lib:
	$(MAKE) -C lib FLOAT_TYPE=$(FLOAT_TYPE)

proj: 	$(OUTPATH)/$(PROJ_NAME).elf

$(OUTPATH)/$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ -Llib -lstm32f4 -lm
	$(OBJCOPY) -O ihex $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).bin

clean:
	rm -f *.o
	rm -f $(OUTPATH)/$(PROJ_NAME).elf
	rm -f $(OUTPATH)/$(PROJ_NAME).hex
	rm -f $(OUTPATH)/$(PROJ_NAME).bin
	$(MAKE) clean -C lib # Remove this line if you don't want to clean the libs as well

upload: lib proj flash

flash:
	arm-none-eabi-gdb -silent --command=gdb_commands $(OUTPATH)/$(PROJ_NAME).elf

gdb: lib proj flash debug

debug:
	arm-none-eabi-gdb --command=gdb_debug_commands $(OUTPATH)/$(PROJ_NAME).elf

openocd:
	openocd -f $(OPENOCD_CONFIG_PATH) #--log_output $(OPENOCD_LOGFILE_PATH) > $(OPENOCD_LOGFILE_PATH)
