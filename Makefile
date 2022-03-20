CC		= gcc
CFLAGS	= -Wall -Wextra -pedantic

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

ansieve: ansieve.o
	$(CC) -o $@ $< $(CFLAGS)

clean:
	rm *.o ansieve

run: ansieve
	./ansieve

