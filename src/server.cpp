#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>

#define DEFAULT_PORT 1602
#define SERVER_ERROR "SERVER_ERROR"
#define SERVER_SUCCESS "SERVER_SUCCESS"
#define BUFFER_SIZE 1024
#define CLIENT_CLOSE_CONNECTION_SYMBOL '#'
#define MAX_SOCKETS_CONNECTIONS 1000

int connect;
int* all_connections;

bool is_client_close_connection(const char* msg)
{
    for(unsigned i = 0; i < strlen(msg); ++i)
        if(msg[i] == CLIENT_CLOSE_CONNECTION_SYMBOL)
            return true;
    return false;       
}

int main(int argc, char* argv[])
{
    int client;
    int server;

    struct sockaddr_in server_address;
    client = socket(AF_INET, SOCK_STREAM, 0);

    if(client < 0)
    {
        std::cout << SERVER_ERROR << ": Socket count not be established\n";
        exit(client);
    }
    std::cout << SERVER_SUCCESS <<": socket for server was created\n";

    server_address.sin_port = htons(DEFAULT_PORT);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htons(INADDR_ANY);

    if(bind(client, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) < 0)
    {
        std::cout << SERVER_ERROR << "binding connention\n";
        return -1;
    }
    std::cout << SERVER_SUCCESS << ": socket bound\n";

    std::cout << "SERVER: listening socket...\n";

    if(listen(client, 1) != 0)
        std::cout << SERVER_ERROR << ": could not listen client socket\n";
    std::cout << SERVER_SUCCESS << ": listening client socket\n";

    socklen_t size = sizeof(server_address);
    server = accept(client, reinterpret_cast<sockaddr*>(&server_address), &size);

    if(server < 0)
        std::cout << SERVER_ERROR << ": could not accept client socket\n";
    std::cout << SERVER_SUCCESS << ": accepted\n";

    char buffer[BUFFER_SIZE];

    while(server > 0)
    {
        bool is_exit = false;

        strcpy(buffer, " => server connected\n");
        if(send(server, buffer, BUFFER_SIZE, 0) < 0)
            std::cout << SERVER_ERROR << ": could not send message to the client\n";
        std::cout << "Connected to the client #1\nEnter " << CLIENT_CLOSE_CONNECTION_SYMBOL << "for disconnect\n";

        std::cout << "Client: ";
        if(recv(server, buffer, BUFFER_SIZE, 0) < 0)
            std::cout << SERVER_ERROR << ": could not recieve message\n";
        
        if(is_client_close_connection(buffer))
            is_exit = true;

        while(!is_exit)
        {        
            std::cout << "Server: ";
            std::cin.getline(buffer, BUFFER_SIZE);
            send(server, buffer, BUFFER_SIZE, 0);
            if(is_client_close_connection(buffer))
                break;
            
            std::cout << "Client: ";
            recv(server, buffer, BUFFER_SIZE, 0);
            std::cout << buffer << std::endl;
            if(is_client_close_connection(buffer))
                break;
        }

        close(server);
        std::cout << "Good Bye\n";
        is_exit = false;
        exit(1);
    }
    return 0;
}