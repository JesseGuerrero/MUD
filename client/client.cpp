#include "networking/encode/login.h"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <unistd.h>
#include <netdb.h>
#include <thread>
#include <functional>
#include "networking/network.cpp"

#define BUF_SIZE 100

char logout[BUF_SIZE]="exit";

int main(int argc, char**argv) {
    char buffer[BUF_SIZE];
    int sockfd, ret;
    setbuf(stdout, 0);

    std::vector <std::string> destinationAddress; destinationAddress.push_back("mudgame.tk");
    std::string port = "5000";
    std::string userName;

    sockfd = CreateAndConnect(destinationAddress[0], port);
    memset(buffer, 0, BUF_SIZE);

    //creating a new thread for receiving messages from the server
    std::thread t(std::bind(ReceiveMessage, sockfd, port, destinationAddress));
    login(userName, sockfd);

    std::cout<<"Enter your messages one by one and press return key"<<std::endl;
    while (fgets(buffer, BUF_SIZE, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;
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

        if(strcasecmp(buffer, logout) == 0)
            break;


        memset(&buffer, 0, sizeof buffer);
    }

    std::cout<<"Exiting & closing"<<std::endl;
    close(sockfd);

    return 0;
}



