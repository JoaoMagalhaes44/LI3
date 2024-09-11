CC = gcc

CFLAGS =-Wall -Iinclude `pkg-config --cflags glib-2.0` -g 

LDFLAGS=`pkg-config --libs glib-2.0` -lm

#-------------------------------------------------------------------------
BIN_DIR = bin
BLD_DIR = build
SRC_DIR = src
SAIDA = Resultados
TESTS = Testes
#--------------------------------------------------------------------------
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BLD_DIR)/%.o,$(SRC))
#--------------------------------------------------------------------------
PROGRAM = programa-principal
TESTES = programa-testes
#==========================================================================
.DEFAULT_GOAL = build

build: setup $(PROGRAM)

$(PROGRAM): $(OBJS)
	gcc $^ $(LDFLAGS) $(CFLAGS) -o $@

build: setup $(TESTES)

$(TESTES): $(OBJS)
	gcc $^ $(LDFLAGS) $(CFLAGS) -o $@

$(BLD_DIR)/%.o: $(SRC_DIR)/%.c
	gcc -c $^ $(LDFLAGS) $(CFLAGS) -o $@

.PHONY: setup
setup:
	@mkdir -p $(BLD_DIR)
	@mkdir -p $(SAIDA)
	@mkdir -p $(TESTS)

.PHONY: clean

clean:
	@-rm -r $(BLD_DIR)
	@-rm -r $(SAIDA)
	@-rm -r $(TESTS)
	@-rm $(PROGRAM)
	@-rm $(TESTES)