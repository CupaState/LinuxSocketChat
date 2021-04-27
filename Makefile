all: server client

server:
	g++ ./src/server.cpp -o server

client:
	g++ ./src/client.cpp -o client