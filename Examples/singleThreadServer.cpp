#include <iostream>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <port number>" << std::endl;
        return 0;
    }
    int port = atoi(argv[1]);

    // 1 socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    // 2 bind
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    int bind_value = bind(sock, (struct sockaddr *) &address, sizeof(address));
    if (bind_value < 0) {
        perror("Could not bind");
        return 1;
    }

    // 3 listen
    int listen_value = listen(sock, 1);
    if (listen_value < 0) {
        perror("Could not listen");
        return 1;
    }

    // 4 accept
    struct sockaddr_in remote_address;
    memset(&address, 0, sizeof(address));
    socklen_t remote_addrlen = sizeof(address);

    std::cout << "Waiting for new connection" << std::endl;
    int client_socket = accept(sock, (struct sockaddr *) &remote_address, &remote_addrlen);

    std::string client_ip = inet_ntoa(remote_address.sin_addr);
    int remote_port = ntohs(remote_address.sin_port);

    std::cout << "Accepted new client @ " << client_ip << ":" << remote_port << std::endl;

    int BUFFLEN = 1024;
    char buffer[BUFFLEN];

    while (1) {
        memset(buffer, 0, BUFFLEN);
        // 5 recv
        int bytes_recieved = recv(client_socket, buffer, BUFFLEN-1, 0);
        if(bytes_recieved < 0) {
            perror("Could not recieve");
            return 1;
        }
        if(bytes_recieved == 0) {
            std::cout << "Client at " << client_ip << ":" << remote_port << " has disconnected." << std::endl;
            break;
        }

        if(buffer[bytes_recieved-1] == '\n')
            buffer[bytes_recieved-1] = 0;

        std::cout << "Client message: \"" << buffer << "\"" << std::endl;

        // 6 send
        std::string response = "Hello client at " + client_ip + ":" + std::to_string(remote_port) +
                ". Your message was \n\"" + std::string(buffer) + "\"\n";

        int bytes_sent = send(client_socket, response.c_str(), response.length(), 0);
        if(bytes_sent < 0) {
            perror("Could not send");
            return 1;
        }
    }
    std::cout << "Shutting down socket." << std::endl;
    shutdown(client_socket, SHUT_RDWR);

    // 7 goto 5
}
