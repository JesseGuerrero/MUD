#ifndef CONNECTHANDLER_H_
#define CONNECTHANDLER_H_

#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cerrno>

#include <sys/socket.h>

#include "HostInfo.h"

/* Thread connection handler */
void* ConnectHandler(void* thread_arg);

#endif /* CONNECTHANDLER_H_ */
