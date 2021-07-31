//
// Created by jesse on 031, Jul 31.
//

#ifndef ECHOSERVER_WORLD_H
#define ECHOSERVER_WORLD_H

#include <list>
#include <string>
#include "Player.h"

class World {
public:
    static std::list<Player*> dataSet;
    static void addPlayer(std::string username);
    static Player getPlayer(std::string username);
};



#endif //ECHOSERVER_WORLD_H
