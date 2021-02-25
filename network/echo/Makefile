.PHONY:clean
CC = g++
CPPFLAGS = -Wall -g
BIN = echoClient echoPollServer echoEpollServer
all: $(BIN)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o $(BIN)
