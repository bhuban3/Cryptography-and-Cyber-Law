CC=gcc
CFLAGS=-Wall -Wextra -O2 -std=c11
BIN=build
all: $(BIN)/osi_client $(BIN)/osi_server
$(BIN):
	mkdir -p $(BIN)
$(BIN)/osi_client: c_src/client.c c_src/osi_common.c c_src/osi_common.h | $(BIN)
	$(CC) $(CFLAGS) c_src/client.c c_src/osi_common.c -o $@
$(BIN)/osi_server: c_src/server.c c_src/osi_common.c c_src/osi_common.h | $(BIN)
	$(CC) $(CFLAGS) c_src/server.c c_src/osi_common.c -o $@
clean:
	rm -rf $(BIN)
.PHONY: all clean
