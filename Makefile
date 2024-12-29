LDFLAGS += -lraylib -lm
HEADER = client/client.h
CLIENT = client/client.c client/entity.c server/entity.c server/entity.h
SERVER = server/server.c
TEST = test/main.c client/entity.c

all: 2dtag-client 2dtag-server 2dtag-test

$(HEADER):

2dtag-client: $(CLIENT) $(HEADER)
	clang -o $@ $(CLIENT) $(LDFLAGS)

2dtag-server: $(SERVER) $(HEADER)
	clang -o $@ $(SERVER)

2dtag-test: $(TEST) $(HEADER)
	clang -o $@ $(TEST) $(LDFLAGS)

.PHONY: all
