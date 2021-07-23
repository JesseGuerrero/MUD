#include "client.h"
#include "networking/encode/login.h"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <string>
#include <utility>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <thread>
#include <functional>
#include "networking/network.cpp"

#define BUF_SIZE 100

int main(int argc, char**argv) {
    char buffer[BUF_SIZE];
    int sockfd, ret;

    if (argc < 3)
    {
        ShowUsage(argv[0]);
        std::exit(EXIT_FAILURE);
    }

    std::vector <std::string> destinationAddress;
    std::string port;
    std::string userName;
    for(int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if((arg == "-h") || (arg == "--help"))
        {
            ShowUsage(argv[0]);
            std::exit(EXIT_SUCCESS);
        }
        else if((arg == "-p") || (arg == "--port"))
        {
            port = argv[++i];
        }
        else if((arg == "-u") || (arg == "--username"))
        {
            userName = argv[++i];
        }
        else if((arg == "-d") || (arg == "--destination"))
        {
            for(int j = i + 1; j < argc; ++j)
            {
                destinationAddress.push_back(argv[j]);
            }

        }
    }

    sockfd = CreateAndConnect(destinationAddress[0], port);


    memset(buffer, 0, BUF_SIZE);

    //creating a new thread for receiving messages from the server
    std::thread t(std::bind(ReceiveMessage, sockfd, port, destinationAddress));
    login(userName, sockfd);

    std::cout<<"Enter your messages one by one and press return key"<<std::endl;
    while (fgets(buffer, BUF_SIZE, stdin) != NULL) {
        unsigned char buf[BUF_SIZE] = {0};
        int msgLen = strlen(buffer);
        int userNameLen = userName.length();

        unsigned int totalLength = msgLen + userNameLen +4;
        packi16(buf,totalLength);
        int offset = 0;
        offset += 2;

        packi16(buf+offset,userNameLen);
        offset += 2;
        memcpy(buf+offset,userName.c_str(),userNameLen);
        offset += userNameLen;
        memcpy(buf+offset,buffer,msgLen);

        ret = send(sockfd, buffer, msgLen, 0);
        if (ret < 0) {
            printf("Error sending data!\n\t-%s", buffer);
        }
        memset(&buffer, 0, sizeof buffer);
    }

    std::cout<<"Exiting & closing"<<std::endl;
    close(sockfd);

    return 0;
}



