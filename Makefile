###############################################################################
# Project properties
###############################################################################

PROJECT_NAME = ustring
PROJECT_VERSION = 0.1.0
PROJECT_TYPE = SLIB

###############################################################################
# Project directory structure variables and rules
###############################################################################

# Project directories
SRC_DIR = ./src
INCLUDE_DIR = ./include
BUILD_DIR = ./build
BIN_DIR = $(BUILD_DIR)/bin
OBJ_DIR = $(BUILD_DIR)/obj

# Search directories
vpath %.o $(OBJ_DIR)
vpath %.c $(SRC_DIR)
vpath %.h $(INCLUDE_DIR)

# Rules for creating a build directory tree
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BIN_DIR): $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)

$(OBJ_DIR): $(BUILD_DIR)
	@mkdir -p $(OBJ_DIR)

###############################################################################
# Build properties
###############################################################################

# Build type: DEBUG, RELEASE
BUILD_TYPE = DEBUG

# Compiler
CC = gcc

# Archiver
AR = ar

# Compiler flags
CC_FLAGS = -std=c11 -Wall -Wpedantic -Wextra -I $(INCLUDE_DIR)

ifeq ($(BUILD_TYPE), DEBUG)
CC_FLAGS += -O0 -ggdb
else ifeq ($(BUILD_TYPE), RELEASE)
CC_FLAGS += -O2 -D NDEBUG
else
$(error Invalid BUILD_TYPE. Possible values: DEBUG, RELEASE)
endif

ifeq ($(PROJECT_TYPE), BIN)
TARGET_RULE = build-bin
else ifeq ($(PROJECT_TYPE), SLIB)
TARGET_RULE = build-static-lib
else
$(error Invalid PROJECT_TYPE. Possible values: BIN, SLIB)
endif

###############################################################################
# Build rules
###############################################################################

# List of source files
SOURCES := $(notdir $(wildcard $(SRC_DIR)/*.c))

# List of object files
OBJECTS := $(SOURCES:%.c=$(OBJ_DIR)/%.o)

# Build object file from source
$(OBJ_DIR)/%.o: %.c
	@echo
	@echo "Building target: $(notdir $@)"
	$(CC) -c $(CC_FLAGS) -o $@ $<

# Build binary from object files
.PHONY: build-bin
build-bin: $(OBJECTS)
	@echo
	@echo "Building target: $(PROJECT_NAME)"
	$(CC) $(CC_FLAGS) -o $(BIN_DIR)/$(PROJECT_NAME) $^

# Build static library from object files
.PHONY: build-static-lib
build-static-lib: $(OBJECTS)
	@echo
	@echo "Building target: lib$(PROJECT_NAME).a"
	$(AR) crs $(BIN_DIR)/lib$(PROJECT_NAME).a $^

# Build all
.PHONY: all
all: $(OBJ_DIR) $(BIN_DIR) $(TARGET_RULE)

ifeq ($(PROJECT_TYPE), BIN)
.PNONY: run
run: all
	@echo
	@echo "Running program $(PROJECT_NAME)"
	$(BIN_DIR)/$(PROJECT_NAME) $(ARGS)
endif

###############################################################################
# Utility rules
###############################################################################

.PHONY: clean
clean:
	@rm -f -r $(BUILD_DIR)
