CC = gcc
CFLAGS = -Wall -I.
LIBS =
DEPS = defs.h
ODIR = obj
OFILES = main.o tokenizer.o parser.o utils.o
OBJ = $(patsubst %,$(ODIR)/%,$(OFILES))
PROG = decl

$(PROG): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(ODIR)/%.o: %.c $(DEPS)
	@mkdir -p $(ODIR)
	$(CC) $(CFLAGS) -c -o $@ $<


.PHONY: clean

clean:
	rm -r $(ODIR)
	rm $(PROG)
