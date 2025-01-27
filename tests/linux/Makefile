######################################
# target
######################################
TARGET = testLinux

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
#Library Path
LIB_PATH = ../../lib
IO_PATH = $(LIB_PATH)/pbmIO
GRAPHICS_PATH = $(LIB_PATH)/pbmGraphic


######################################
# source
######################################
# C sources
C_SOURCES =  \
$(wildcard *.c) \
$(wildcard $(LIB_PATH)/*.c) \
$(wildcard $(IO_PATH)/*.c) \
$(wildcard $(GRAPHICS_PATH)/*.c) \

######################################
# Compiler
######################################
# C compiler
CC = gcc

# Arguments
C_ARG = -Wall -Wextra 

# C Defines
C_DEFS = 

# C Includes
C_INC = \
-I$(LIB_PATH) \
-I$(IO_PATH) \
-I$(GRAPHICS_PATH)

#C Libs
C_LIBS = $$(sdl-config --cflags --libs)

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
	$(CC) $(CFLAGS) -c $< -o $@ $(C_LIBS) 

$(BUILD_DIR)/$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(C_LIBS)

run: $(BUILD_DIR)/$(TARGET)
	$(BUILD_DIR)/$(TARGET)

clean:
	rm -rf $(BUILD_DIR)