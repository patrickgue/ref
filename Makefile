prog=ref

src=$(wildcard *.c)
obj=$(src:.c=.o)

LDFLAGS=-lncurses

$(prog): $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) $(prog)
