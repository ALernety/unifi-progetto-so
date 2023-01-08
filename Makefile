# Paths of the project structure, relative to the folder where the Makefile
# is located (in this case, the root of project)
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
LOG_DIR := log
TMP_DIR := tmp

# Set default shell
SHELL   := /bin/sh

# Compiler to use and related compiler and linker flags
CC	    := gcc
CFLAGS  := $(shell awk 'BEGIN {ORS=" "} {print $0}' compile_flags.txt)
DEBUG ?= 1
ifneq ($(DEBUG), 0)
    CFLAGS += -g
endif
LDFLAGS := -lm

# Functions for resolving component source and object files
SRC_FILES = $(wildcard $(SRC_DIR)/$(1)/*.c) $(wildcard $(SRC_DIR)/common/*.c)
OBJ_FILES = $(subst \
				$(SRC_DIR)/,\
				$(OBJ_DIR)/,\
				$(patsubst %.c,%.o,$(call SRC_FILES,$(1)))\
			)

# Reference to the unique object file generation command
define BUILD_O
	$(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS)
endef

# Makefile internal options
.NOTPARALLEL:          # Disable parallel execution (`--jobs=N` is ignored)
# 					     due to conflicts with output path creation
.DEFAULT_GOAL := help  # When no target is specified, `help` is executed
#						 (unnecessary since `help` is the first target listed anyway)

### Auxiliary/cosmetic targets

.PHONY: help clean clean_tmp
help: ## Show this help
	@sed -e':a' -e'N' -e'$$!ba' -e's/\\\n//g' $(MAKEFILE_LIST)\
	| grep -E '^[a-zA-Z\._-]+:.*?## .*$$'\
	| awk \
		'BEGIN {FS = ":.*?## "};\
		{printf "\033[36m%-25s\033[0m %s\n", $$1, $$2}'
clean: ## Clean up object files, binaries, logs and temporary files
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(LOG_DIR) $(TMP_DIR)
clean_tmp: ## Clean up logs and temporary files
	rm -rf $(TMP_DIR)/* $(LOG_DIR)/*

### Entry targets

.PHONY: all PADRE_TRENI REGISTRO RBC railway_manager
all: PADRE_TRENI REGISTRO RBC railway_manager ## Compile all the components
PADRE_TRENI: $(call OBJ_FILES,PADRE_TRENI)\
  bindir bin.PADRE_TRENI ## Compile the PADRE_TRENI
REGISTRO: $(call OBJ_FILES,REGISTRO)\
  bindir bin.REGISTRO ## Compile the REGISTRO
RBC: $(call OBJ_FILES,RBC)\
  bindir bin.RBC ## Compile the RBC
railway_manager: | PADRE_TRENI REGISTRO $(call OBJ_FILES,railway_manager)\
  bindir bin.railway_manager ## Compile the railway_manager

### Object file compilation targets

$(OBJ_DIR)/common/%.o:\
  $(SRC_DIR)/common/%.c cpplint/common/%.c | objdir/common
	$(BUILD_O)
$(OBJ_DIR)/PADRE_TRENI/%.o:\
  $(SRC_DIR)/PADRE_TRENI/%.c cpplint/PADRE_TRENI/%.c | objdir/PADRE_TRENI
	$(BUILD_O)
$(OBJ_DIR)/REGISTRO/%.o:\
  $(SRC_DIR)/REGISTRO/%.c cpplint/REGISTRO/%.c | objdir/REGISTRO
	$(BUILD_O)
$(OBJ_DIR)/RBC/%.o:\
  $(SRC_DIR)/RBC/%.c cpplint/RBC/%.c | objdir/RBC
	$(BUILD_O)
$(OBJ_DIR)/railway_manager/%.o:\
  $(SRC_DIR)/railway_manager/%.c cpplint/railway_manager/%.c | objdir/railway_manager
	$(BUILD_O)

### Creation of output target paths

.PHONY: objdir bindir tmpdir logdir cpplint
objdir:
	@mkdir -p $(OBJ_DIR)
objdir/%:
	@mkdir -p $(OBJ_DIR)/$(subst objdir/,,$@)
bindir:
	@mkdir -p $(BIN_DIR)
tmpdir:
	@mkdir -p $(TMP_DIR)
logdir:
	@mkdir -p $(LOG_DIR)
cpplint: ## Analyze all source code
	cpplint $(shell find $(SRC_DIR) -type f -name "*.[c\|h]")
cpplint/%: ## Analyze source code of directory or specifed file
	-cpplint $(shell find \
					$(SRC_DIR)/$(subst cpplint/,,$@) \
					-type f \
					-name "*.[c\|h]"\
				)

### Binary build targets

bin.%: | bindir tmpdir logdir
	$(CC) \
		$(CFLAGS) \
		$(shell find \
			$(OBJ_DIR)/$(subst bin.,,$@) \
			$(OBJ_DIR)/common \
			-type f \
			-name "*.o") \
		-o $(BIN_DIR)/$(subst bin.,,$@) \
		$(LDFLAGS)
