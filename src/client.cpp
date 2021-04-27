#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>

#define SERVER_CLOSE_CONNECTION_SYMBOL '#'
#define SERVER_IP "127.0.0.1"
#define PORT 1602
#define CLIENT_ERROR "SERVER_ERROR"
#define CLIENT_SUCCESS "SERVER_SUCCESS"
#define BUF_SIZE 1024

bool is_client_close_connection(const char* msg)
{
    for(unsigned i = 0; i < strlen(msg); ++i)
        if(msg[i] == SERVER_CLOSE_CONNECTION_SYMBOL)
            return true;
    return false;
}

int main(int argc, const char* argv[])
{
    int client;
    struct sockaddr_in server_address;
    client = socket(AF_INET, SOCK_STREAM, 0);

    if(client < 0)
        std::cout << CLIENT_ERROR << ": socket error\n";
    std::cout << CLIENT_SUCCESS << ": socket for client was created\n";

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);

    if(connect(client, reinterpret_cast<struct sockaddr*>(&server_address), sizeof(server_address)) == 0)
    {
        std::cout << "Connection to server " 
        << inet_ntoa(server_address.sin_addr) << " with port number: " << PORT << "\n";
    }
    std::cout << "Waiting for server information\n";

    char buffer[BUF_SIZE];
    if(recv(client, buffer, BUF_SIZE, 0) < 0)
        std::cout << CLIENT_ERROR << ": could not connection to server\n";
    std::cout << "Connected...\nEnter " << SERVER_CLOSE_CONNECTION_SYMBOL << " for close connection\n";

    while(true)
    {
        std::cout << "Client: \n";
        std::cin.getline(buffer, BUF_SIZE);
        send(client, buffer, BUF_SIZE, 0);

        if(is_client_close_connection(buffer))
            break;

        std::cout << "Server: \n";
        recv(client, buffer, BUF_SIZE, 0);
        std::cout << buffer << std::endl;

        if(is_client_close_connection(buffer))
            break;
        std::cout << std::endl;
    }

    close(client);
    std::cout << "GoodBye\n";
    return 0;
}