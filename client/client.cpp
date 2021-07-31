#include "networking/encode/login.h"
#include "networking/encode/global_message.h"

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

    while (true) {
        global_message(buffer, sockfd);
    }

    std::cout<<"Exiting & closing"<<std::endl;
    close(sockfd);

    return 0;
}



