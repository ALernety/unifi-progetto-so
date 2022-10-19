# Paths della struttura del progetto, relative alla cartella in cui è situato il Makefile (in questo caso, la root)
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
LOG_DIR := log
TMP_DIR := tmp

# Compilatore da utilizzare e relative flag per compilatore e linker
CC	    := gcc
CFLAGS  := -Wall -Wextra -pedantic
DEBUG ?= 1
ifneq ($(DEBUG), 0)
    CFLAGS += -g
endif
LDFLAGS := -lm

# Funzioni per la risoluzione di sorgenti e file oggetto dei componenti
SRC_FILES = $(wildcard $(SRC_DIR)/$(1).c)
OBJ_FILES = $(subst $(SRC_DIR)/,$(OBJ_DIR)/,$(patsubst %.c,%.o,$(call SRC_FILES,$(1))))

# Riferimento unico al comando di generazione dei file oggetto
define BUILD_O
	$(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS)
endef

# Opzioni interne del Makefile
# .NOTPARALLEL:          # Disabilita l'esecuzione parallela (`--jobs=N` viene ignorato) a causa di conflitti con la creazione dei path di output
.DEFAULT_GOAL := help  # Quando non viene specificato alcun target, viene eseguito `help` (superfluo visto che `help` è comunque il primo target elencato)

### Targets ausiliari/cosmetici

.PHONY: help clean
help: ## Mostra questo aiuto
	@grep -E '^[a-zA-Z\._-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-25s\033[0m %s\n", $$1, $$2}'
clean: ## Pulisci i file oggetto, i binari, i log e i file temporanei
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(LOG_DIR) $(TMP_DIR)
clean_tmp: ## Pulisci i log e i file temporanei
	rm -rf $(TMP_DIR) $(LOG_DIR)

### Targets di entrata

# .PHONY: all generatore_fallimenti pfc pfc_disconnect_switch transducers wes main
.PHONY: all PADRE_TRENI REGISTRO TRENO main
all: PADRE_TRENI REGISTRO TRENO main ## Compila tutti i componenti
PADRE_TRENI: $(call OBJ_FILES,PADRE_TRENI) ## Compila il PADRE_TRENI
REGISTRO: $(call OBJ_FILES,REGISTRO) ## Compila il REGISTRO
TRENO: $(call OBJ_FILES,TRENO) ## Compila il TRENO
main: $(call OBJ_FILES,main) bin.railway_manager ## Compila il main

### Targets di compilazione file oggetto

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | objdir
	$(BUILD_O)

### Targets di creazione paths di output

.PHONY: objdir bindir tmpdir logdir
objdir:
	@mkdir -p $(OBJ_DIR)
# objdir/%:
# 	@mkdir -p $(OBJ_DIR)/$(subst objdir/,,$@)
bindir:
	@mkdir -p $(BIN_DIR)
# bindir/%:
# 	@mkdir -p $(BIN_DIR)/$(subst bindir/,,$@)
tmpdir:
	@mkdir -p $(TMP_DIR)
logdir:
	@mkdir -p $(LOG_DIR)
	# @mkdir -p $(LOG_DIR)/components

### Targets di compilazione binari

bin.%: | bindir tmpdir logdir
    # Cache wildcard bypassata tramite utilizzo di ls via shell
	$(CC) $(CFLAGS) $(OBJ_DIR)/* -o $(BIN_DIR)/$(subst bin.,,$@) $(LDFLAGS)
