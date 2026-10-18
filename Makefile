CC = gcc
CFLAGS = -Wall -Wextra -g -Isrc
LDFLAGS =

# Core foundation objects implemented by Dilna
DILNA_OBJS = src/tac.o src/tacprint.o src/tacread.o src/fold.o src/blocks.o src/stats.o

# Check whether Kavitha's and Kalpana's real modules exist, otherwise use stubs
ifneq ($(wildcard src/parser.y src/lexer.l),)
  FRONT_OBJS = src/lexer.o src/parser.o src/ast.o src/symtab.o src/tacgen.o
else
  FRONT_OBJS = src/stub_gen.o
endif

ifneq ($(wildcard src/optimize.c),)
  OPT_OBJS = src/dag.o src/cse.o src/optimize.o
else
  OPT_OBJS = src/stub_opt.o
endif

OBJS = src/main.o $(DILNA_OBJS) $(FRONT_OBJS) $(OPT_OBJS)

TARGET = expltac

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

src/parser.c src/parser.h: src/parser.y
	bison -d -o src/parser.c $<

src/lexer.c: src/lexer.l src/parser.h
	flex -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	@mkdir -p tests/dilna
	@bash scripts/run_all_tests.sh

package: clean
	@bash scripts/package.sh

clean:
	rm -f src/*.o $(TARGET) expl-tac.tar.gz
	rm -f src/parser.c src/parser.h src/lexer.c
	rm -f sample/*.unopt.tac sample/*.opt.tac sample/*.stats.txt
