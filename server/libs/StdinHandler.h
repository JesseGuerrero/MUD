#ifndef STDINHANDLER_H_
#define STDINHANDLER_H_

#include <iostream>
#include <string>
#include <map>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cerrno>

#include <sys/socket.h>

#include "HostInfo.h"

/* Thread standard input handler */
void* StdinHandler(void* thread_arg);

#endif /* STDINHANDLER_H_ */
