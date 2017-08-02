CC = gcc
CFLAGS = -Wall -I.
LIBS = -lm
DEPS = rpcalc.h
ODIR = obj
OFILES = main.o parser.o stack.o variables.o
OBJ = $(patsubst %,$(ODIR)/%,$(OFILES))
PROG = rpcalc

$(PROG): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(ODIR)/%.o: %.c $(DEPS)
	@mkdir -p $(ODIR)
	$(CC) $(CFLAGS) -c -o $@ $<


.PHONY: clean

clean:
	rm -r $(ODIR)
	rm $(PROG)
