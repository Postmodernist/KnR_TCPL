CC = gcc
CFLAGS = -Wall -I.
LIBS =
DEPS = fileio.h
ODIR = obj
OFILES = test_f.o fileio.o
OBJ = $(patsubst %,$(ODIR)/%,$(OFILES))
PROG = test_f

$(PROG): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(ODIR)/%.o: %.c $(DEPS)
	@mkdir -p $(ODIR)
	$(CC) $(CFLAGS) -c -o $@ $<


.PHONY: clean

clean:
	rm -r $(ODIR)
	rm $(PROG)
