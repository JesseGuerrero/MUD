//
// Created by jesse on 031, Jul 31.
//

#include "World.h"

std::list<Player*> World::dataSet;

 void World::addPlayer(std::string username) {
    Player *p = new Player(username);
    dataSet.push_back(p);
}

Player World::getPlayer(std::string username) {
     for(Player *p : dataSet) {
         if(p->username == username)
             return *p;
     }
}