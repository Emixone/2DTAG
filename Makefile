LINKFLAGS += -lraylib -lm
CLIENT = client.c
SERVER = server.c

client:
	cd client
	clang -o client $(CLIENT) $(LINKFLAGS)
	cd ..
server:
	cd server
	clang -o server $(SERVER)
	cd ..
all:
	cd client
	clang -o client $(CLIENT) $(LINKFLAGS)
	cd ../server
	clang -o server $(SERVER)		
