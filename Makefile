LDFLAGS += -lraylib -lm
CLIENT = client/client.c
SERVER = server/server.c

all: 2dtag-client 2dtag-server

2dtag-client: $(CLIENT)
	clang -o $@ $(CLIENT) $(LDFLAGS)

2dtag-server: $(SERVER)
	clang -o $@ $(SERVER)

.PHONY: all