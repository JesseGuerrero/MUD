add_executable(server Server.cpp libs/IPv4Parse.cpp libs/StdinHandler.cpp libs/HostInfo.cpp libs/ConnectHandler.cpp
        libs/multithread.h libs/ConnectHandler.h libs/HostInfo.h libs/StdinHandler.h libs/IPv4Parse.h
        Player.cpp Player.h World.cpp World.h)