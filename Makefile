.PHONY: all
.PHONY: clean

PORT=6969
SERVER_ADDRESS='"0.0.0.0"'
SERVER_BIND_ADDRESS='"0.0.0.0"'
DEFINES=-DPORT=${PORT} -DSERVER_ADDRESS=${SERVER_ADDRESS} -DSERVER_BIND_ADDRESS=${SERVER_BIND_ADDRESS}
HEADERS=network_utils.h

all: server

run: all
	./server

server: server.cpp ${HEADERS}
	g++ -pthread -std=c++11 ${DEFINES} server.cpp -o server

clean:
	rm -f server

