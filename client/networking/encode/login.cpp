//
// Created by jesse on 023, Jul 23.
//

#include "login.h"
#include "../network.h"

void login(std::string userName, int sockfd) {
    int ret;
    char buffer[BUF_SIZE];

    std::cout<<"Username: ";
    fgets(buffer, BUF_SIZE, stdin);

    strcpy(buffer, ("0-" + std::string(buffer)).c_str());
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
        printf("Error sending data!\n%s", buffer);
    }
    memset(&buffer, 0, sizeof buffer);
}