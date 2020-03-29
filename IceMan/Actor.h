#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor: public GraphObject
{
private:
    StudentWorld *S;
protected:
    bool alive, visible;
public:
    Actor(int ID, int X, int Y, Direction sDirection, float s, unsigned int d, bool vis, StudentWorld *world): GraphObject(ID, X, Y, sDirection, s, d) {
        setVisible(vis);
        visible = vis;
        S = world;
        alive = true;
    }
    ~Actor() {}
    virtual void doSomething() = 0;
    bool isAlive() {return alive;}
    StudentWorld *getWorld() {
        return S;
    }
    bool isVisible() {return visible;}
};

class Ice: public Actor
{
public:
    Ice (int x, int y, StudentWorld *world): Actor(IID_ICE, x, y, right, 0.25, 3, true, world) {}
    void doSomething() {}
    ~Ice() {}
};

class Iceman: public Actor
{
private:
    int h_pts, water, sonar, gold;
public:
    Iceman(StudentWorld *world): Actor(IID_PLAYER, 30, 60, right, 1, 0, true, world), h_pts(10), water(5), sonar(1), gold(0) {}
    void doSomething();
    void annoyed(int damage);
    int getHitPoints() {
        return h_pts;
    }
    int getHealth()
    {
        return h_pts * 10;
    }
    int getWater()
    {
        return water;
    }
    int getGold()
    {
        return gold;
    }
    int getSonar()
    {
        return sonar;
    }
    void setSonar()
    {
        sonar++;
    }
    void setWater()
    {
        water += 5;
    }
    void pickUpGold()
    {
        gold++;
    }
};

class Boulder: public Actor
{
private:
    bool stable;
    int waiting;
public:
    Boulder(int x, int y, StudentWorld *world): Actor(IID_BOULDER, x, y, down, 1, 1, true, world), stable(true), waiting(0) {}
    ~Boulder() {}
    void doSomething();
    void fall();
};

class Squirt: public Actor
{
private:
    int distance;
public:
    Squirt(int x, int y, Direction I_Direction, StudentWorld *world): Actor(IID_WATER_SPURT, x, y, I_Direction, 1, 1, true, world), distance(4) {}
    ~Squirt() {}
    void doSomething();
};

class Barrel: public Actor
{
private:

public:
    Barrel(int x, int y, StudentWorld *world): Actor(IID_BARREL, x, y, right, 1, 2, false, world){visible = false;}
    ~Barrel() {}
    void doSomething();
};

class GoldNugget: public Actor
{
private:
    bool temp;
    int waiting;
public:
    GoldNugget(int x, int y, bool t, StudentWorld *world) : Actor(IID_GOLD, x, y, right, 1, 2, t, world), waiting(100){
        temp = t;
        //temporary = dropped, can be picked up by protester
        //non-temp = in oil field, can be picked up by iceman
        if (temp){
            setVisible(true);
            visible = true;
        }
        else{
            setVisible(false);
            visible = false;
        }
    }
    ~GoldNugget() {}
    void doSomething();
};

class SonarKit: public Actor
{
private:
    int ticks;
public:
    SonarKit(int x, int y, StudentWorld *world) : Actor(IID_SONAR, x, y, right, 1, 2, true, world){}
    ~SonarKit() {}
    void doSomething();
};

class WaterPool: public Actor
{
private:
    int waiting, ticks;
public:
    WaterPool(int x, int y, StudentWorld *world): Actor(IID_WATER_POOL, x, y, right, 1, 2, true, world), waiting(0) {}
    ~WaterPool() {}
    void doSomething();
};

class RegularProtester : public Actor
{
private:
    int h_points, numSquares, restTicks, annoyTicks, perpendicularTicks;
    bool leaveOilField;
public:
    RegularProtester(int x, int y, StudentWorld *world);
    void doSomething();
    void annoyed(int damage);
    int getHPoints() {return h_points;}
    bool isFacing();
    void newDirection();
    void atIntersection();
    void setLeaveOilField();
    ~RegularProtester() {}
};

class HardcoreProtester : public Actor
{
private:
    int h_points, numSquares, restTicks, annoyTicks, perpendicularTicks;
    bool leaveOilField;
public:
    HardcoreProtester(int x, int y, StudentWorld *world);
    void doSomething();
    void annoyed(int damage);
    int getHPoints() {return h_points;}
    bool isFacing();
    void newDirection();
    void atIntersection();
    void setLeaveOilField() {leaveOilField = true;}
    void setStareTicks();
    ~HardcoreProtester() {}
};
#endif // ACTOR_H_
