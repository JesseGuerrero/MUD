//
// Created by jesse on 023, Jul 23.
//

#ifndef ECHOSERVER_NETWORK_H
#define ECHOSERVER_NETWORK_H

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

#define BUF_SIZE 100

static void ShowUsage(std::string name);
//Duplicated the same function from EchoServer
void* GetAddr(struct sockaddr *sa);

int Connect(int sockfd, const std::string& host, const std::string& port);

int CreateAndConnect(std::string& host, std::string& port);

/*
 * Not tested with multiple backends,
 * basically from the lists of address itr is trying to connect
 * to another server.
 */
void GetNewBackendAddress(std::vector <std::string>& destinationAddress,std::string& active);

void ConnectNew(int sockfd, std::string port, std::vector <std::string> destinationAddress, std::string& active);



void ReceiveMessage(int sockfd, std::string port, std::vector <std::string> destinationAddress);

void packi16(unsigned char *buf, unsigned int i);

#endif //ECHOSERVER_NETWORK_H
