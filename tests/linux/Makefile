######################################
# target
######################################
TARGET = test-$(HOST_ARCH)

######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og

#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

TOP_PATH = ../..

######################################
# source
######################################
# C sources
C_SOURCES =  \
$(TOP_PATH)/src/pbm_graphics.c \
$(TOP_PATH)/src/sdl2_displayPbm.c \
$(wildcard *.c) 

######################################
# Compiler
######################################
HOST_ARCH := $(shell arch)
# C compiler
CC = gcc

# Arguments
C_ARG = -Wall -Wextra -Wpedantic

# C Defines
C_DEFS = 

# C Includes
C_INC = \
-I$(TOP_PATH)/inc \
-Ifonts

#C Libs
C_LIBS = -lSDL2 -lm #$$(sdl-config --cflags --libs)

CFLAGS = $(C_ARG) $(C_DEFS) $(C_INC)

######################################
# Objects
######################################
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
_DIR_GUARD = @mkdir -p $(@D)

.phony: run clean

all: $(C_SOURCES) $(OBJECTS) $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/%.o: %.c
	$(_DIR_GUARD)
	$(CC) $(CFLAGS) -c $< -o $@ 

$(BUILD_DIR)/$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(C_LIBS)

run: $(BUILD_DIR)/$(TARGET)
	@$(BUILD_DIR)/$(TARGET)

clean:
	rm -rf $(BUILD_DIR)
