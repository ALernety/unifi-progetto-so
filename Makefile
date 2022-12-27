# Paths della struttura del progetto, relative alla cartella in cui è situato il Makefile (in questo caso, la root)
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
LOG_DIR := log
TMP_DIR := tmp
# Set default shell.
SHELL   := /bin/sh
# Compilatore da utilizzare e relative flag per compilatore e linker
CC	    := gcc
CFLAGS  := -Wall -Wextra -pedantic
DEBUG ?= 1
ifneq ($(DEBUG), 0)
    CFLAGS += -g
endif
LDFLAGS := -lm

# Funzioni per la risoluzione di sorgenti e file oggetto dei componenti
SRC_FILES = $(wildcard $(SRC_DIR)/$(1)/*.c) $(wildcard $(SRC_DIR)/common/*.c)
OBJ_FILES = $(subst \
				$(SRC_DIR)/,\
				$(OBJ_DIR)/,\
				$(patsubst %.c,%.o,$(call SRC_FILES,$(1)))\
			)

# Riferimento unico al comando di generazione dei file oggetto
define BUILD_O
	$(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS)
endef

# Opzioni interne del Makefile
# .NOTPARALLEL:          # Disabilita l'esecuzione parallela (`--jobs=N` viene ignorato) a causa di conflitti con la creazione dei path di output
.DEFAULT_GOAL := help  # Quando non viene specificato alcun target, viene eseguito `help` (superfluo visto che `help` è comunque il primo target elencato)

### Targets ausiliari/cosmetici

.PHONY: help clean clean_tmp
help: ## Mostra questo aiuto
	@sed -e':a' -e'N' -e'$$!ba' -e's/\\\n//g' $(MAKEFILE_LIST)\
	| grep -E '^[a-zA-Z\._-]+:.*?## .*$$'\
	| awk \
		'BEGIN {FS = ":.*?## "};\
		{printf "\033[36m%-25s\033[0m %s\n", $$1, $$2}'
clean: ## Pulisci i file oggetto, i binari, i log e i file temporanei
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(LOG_DIR) $(TMP_DIR)
clean_tmp: ## Pulisci i log e i file temporanei
	rm -rf $(TMP_DIR) $(LOG_DIR)

### Targets di entrata

.PHONY: all PADRE_TRENI REGISTRO RBC railway_manager
all: PADRE_TRENI REGISTRO RBC railway_manager ## Compila tutti i componenti
PADRE_TRENI: $(call OBJ_FILES,PADRE_TRENI)\
  bindir bin.PADRE_TRENI ## Compila il PADRE_TRENI
REGISTRO: $(call OBJ_FILES,REGISTRO)\
  bindir bin.REGISTRO ## Compila il REGISTRO
RBC: $(call OBJ_FILES,RBC)\
  bindir bin.RBC ## Compila il RBC
railway_manager: | PADRE_TRENI REGISTRO $(call OBJ_FILES,railway_manager)\
  bindir bin.railway_manager ## Compila il railway_manager

### Targets di compilazione file oggetto

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

### Targets di creazione paths di output

.PHONY: objdir bindir tmpdir logdir cpplint
objdir:
	@mkdir -p $(OBJ_DIR)
objdir/%:
	@mkdir -p $(OBJ_DIR)/$(subst objdir/,,$@)
bindir:
	@mkdir -p $(BIN_DIR)
bindir/%:
	@mkdir -p $(BIN_DIR)/$(subst bindir/,,$@)
tmpdir:
	@mkdir -p $(TMP_DIR)
logdir:
	@mkdir -p $(LOG_DIR)
	# @mkdir -p $(LOG_DIR)/components
cpplint: ## Analyze all source code
	cpplint $(shell find $(SRC_DIR) -type f -name "*.c")
cpplint/%: ## Analyze source code of directory or specifed file
	-cpplint $(shell find \
					$(SRC_DIR)/$(subst cpplint/,,$@) \
					-type f \
					-name "*.c"\
				)

### Targets di compilazione binari

bin.%: | bindir tmpdir logdir
    # Cache wildcard bypassata tramite utilizzo di ls via shell
	$(CC) \
		$(CFLAGS) \
		$(shell find \
			$(OBJ_DIR)/$(subst bin.,,$@) \
			$(OBJ_DIR)/common \
			-type f \
			-name "*.o") \
		-o $(BIN_DIR)/$(subst bin.,,$@) \
		$(LDFLAGS)
