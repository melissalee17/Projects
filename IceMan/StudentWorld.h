#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir): GameWorld(assetDir) {}
    ~StudentWorld() {}
    int init();
    int move();
    void cleanUp();
    std::shared_ptr<Ice> getIce(int x, int y)
    {
        return ice_array[x][y];
    }
    void setIce(int x, int y)
    {
        ice_array[x][y]->setVisible(false);
        ice_array[x][y] = nullptr;
    }
    void makeSquirt();
    void useSonar(int x, int y);
    bool icemanCheck(int X, int Y, int R);
    void annoyIceman(int damage);
    void increaseSonar();
    void oilFound() {L--;}
    void increaseWater();
    bool boulderCheck(int x, int y);
    int getIcemanX() {return iceman_ptr->getX();}
    int getIcemanY() {return iceman_ptr->getY();}
    bool BoulderIceCheck(int x, int y);
    bool actualCheck(int x, int y);
    int protesterCheck(int x, int y);
    void annoyProtester(int damage, int x, int y);
    void dropGold(int x, int y);
    void icemanGold();
    void actorsDoSomething();
    void removeDeadActors();
    void firstBreadthSearch(int x, int y);
    std::string leave();
    int movesToIceman();
    std::string moveToIceman();
    bool objectRadius(int X1, int Y1, int X2, int Y2);
    void bribe(int x, int y);
private:
    std::shared_ptr<Ice> ice_array[VIEW_WIDTH][VIEW_HEIGHT];
    std::shared_ptr<Iceman> iceman_ptr;
    std::vector<std::shared_ptr<Actor>> actors;
    int B, G, L, protesterTicks, numofProtesters, maxNumofProtesters, probofHardcore;
    struct Node {
        int x, y, distance;
        Node() {}
        Node(int X1, int Y1, int dis) {
            x = X1;
            y = Y1;
            distance = dis;
        }
        int getX() {return x;}
        int getY() {return y;}
        int getDistance() {return distance;}
    };
    int path[VIEW_WIDTH - 3][VIEW_HEIGHT - 3];
};

#endif // STUDENTWORLD_H_


