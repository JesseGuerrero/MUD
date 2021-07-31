//
// Created by jesse on 031, Jul 31.
//

#include "global_message.h"
#include "../network.h"

void global_message(std::string userName, int sockfd) {
    int ret;
    char buffer[BUF_SIZE];

    std::cout<<"Global message: ";
    fgets(buffer, BUF_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0;

    strcpy(buffer, ("1-" + std::string(buffer)).c_str());
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
