CC = gcc
CFLAGS = -Wall -I.
LIBS =
DEPS = dirent_n.h
ODIR = obj
OFILES = main.o dirent_n.o
OBJ = $(patsubst %,$(ODIR)/%,$(OFILES))
PROG = fsize

$(PROG): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(ODIR)/%.o: %.c $(DEPS)
	@mkdir -p $(ODIR)
	$(CC) $(CFLAGS) -c -o $@ $<


.PHONY: clean

clean:
	rm -r $(ODIR)
	rm $(PROG)
