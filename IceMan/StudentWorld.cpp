#include "StudentWorld.h"
#include <string>
#include <sstream>
#include <queue>
#include <stack>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

int StudentWorld::init()
{
    //creates the ice
    for (int y = 0; y < VIEW_HEIGHT - 4; y++)
    {
        for (int x = 0; x < VIEW_WIDTH; x++)
        {
            ice_array[x][y] = make_shared<Ice>(x,y,this);
            if ((x >= 30 && x <= 33) && y >= 4) {
                if (ice_array[x][y] != nullptr)
                     ice_array[x][y].reset();
            }
        }
    }
    //creates the Iceman
    iceman_ptr = make_shared<Iceman>(this);
    actors.emplace_back(make_shared<RegularProtester>(60, 60, this));
    numofProtesters = 0;
    numofProtesters++;
    int level = getLevel();
    protesterTicks = max(25, 200 - level);
    int temp = 2 + level*1.5;
    maxNumofProtesters = min(15, temp);
    //boulders
    B = min(level/2 + 2, 9);
    while (B > 0)
    {
        int randX = rand() % (VIEW_WIDTH - 3) , randY = rand() % (VIEW_HEIGHT - 3);
        //if inside ice field
        if ((randX <= 25 && randY <= 56 && randY >= 20) || (randX >= 35 && randY <= 56 && randY >= 20))
        {
            bool createBoulder = false;
            for (auto a: actors)
            {
                int X = a->getX(), Y = a->getY();
                if (objectRadius(randX, randY, X, Y) && abs(X - randX) > 30)
                {
                    createBoulder = true;
                    break;
                }
            }
            if (!createBoulder)
            {
                //not near any other boulders
                actors.emplace_back(make_shared<Boulder>(randX, randY, this));
                B--;
                //clear ice
                for (int i = randX; i < randX + 4; i++)
                {
                    for (int j = randY; j < randY + 4; j++)
                    {
                        if (ice_array[i][j] != nullptr)
                            ice_array[i][j].reset();
                    }
                }
            }
        }
            
    }
    //gold nuggets
    G = max(5 - level/2, 2);
    int gold = G;
    while (gold > 0)
    {
        int randX = rand() % (VIEW_WIDTH - 3) , randY = rand() % (VIEW_HEIGHT - 3);
        //if inside ice field
        if ((randX <= 25 && randY <= 56 && randY >= 0) || (randX >= 34 && randY <= 56 && randY >= 0))
        {
            bool createGoldNugget = false;
            for (auto a: actors)
            {
                int X = a->getX(), Y = a->getY();
                if (objectRadius(randX, randY, X, Y))
                {
                    createGoldNugget = true;
                    break;
                }
            }
            if (!createGoldNugget)
            {
                actors.emplace_back(make_shared<GoldNugget>(randX, randY, false, this));
                gold--;
            }
        }
    }
    L = min(2 + level, 21);
    int oil = L;
    while (oil > 0)
    {
        int randX = rand() % (VIEW_WIDTH - 3) , randY = rand() % (VIEW_HEIGHT - 3);
        //if inside ice field
        if ((randX <= 25 && randY <= 56 && randY >= 0) || (randX >= 34 && randY <= 56 && randY >= 0))
        {
            bool createOil = false;
            for (auto a: actors)
            {
                int X = a->getX(), Y = a->getY();
                if (objectRadius(randX, randY, X, Y))
                {
                    createOil = true;
                    break;
                }
            }
            if (!createOil)
            {
                //not near any other boulders
                actors.emplace_back(make_shared<Barrel>(randX, randY, this));
                oil--;
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    string message;
    if (iceman_ptr->isAlive() == false){
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    if (L == 0)
        return GWSTATUS_FINISHED_LEVEL;
    ostringstream oss;
    oss << "Lvl: " << getLevel() << " Lives: " << getLives() << " Hlth: " << iceman_ptr->getHealth() << "% Wtr: " << iceman_ptr->getWater() << " Gld: " << iceman_ptr->getGold() << " Oil Left: " << L << " Sonar: " << iceman_ptr->getSonar() << " Scr: " << setfill('0') << setw(5) << getScore();
    setGameStatText(oss.str());
    if (protesterTicks > 0)
        protesterTicks--;
    int level = getLevel();
    if (protesterTicks == 0 && numofProtesters < maxNumofProtesters)
    {
        protesterTicks = max(25, 200 - level);
        probofHardcore = min(90, level * 10 + 30);
        if (probofHardcore < (rand() % 50))
        {
            actors.emplace_back(make_shared<RegularProtester>(60, 60, this));
        }
        else
            actors.emplace_back(make_shared<HardcoreProtester>(60, 60, this));
        numofProtesters++;
    }
    int G = level * 25 + 300;
    int random = rand() % G;
    if (random == 1)
    {
        int goodie = rand() % 5;
        if (goodie == 0) {
            actors.emplace_back(make_shared<SonarKit>(0, 60, this));
        }
        else {
            bool create = false;
            while (!create)
            {
                int X1 = rand() % (VIEW_WIDTH);
                int Y1 = rand() % (VIEW_HEIGHT - 4);
                bool check = true;
                for (int i = X1; i < X1 + 4; i++)
                    for (int j = Y1; j < Y1 + 4; j++) {
                        if (ice_array[i][j] != nullptr)
                            check = false;
                    }
                if (check == true) {
                    for (auto p = actors.begin(); p != actors.end(); p++)
                    {
                        int X2 = (*p)->getX();
                        int Y2 = (*p)->getY();
                        if (objectRadius(X1, Y1, X2, Y2))
                        {
                            check = false;
                            break;
                        }
                        
                    }
                }
                
                if (check == true)
                {
                    actors.emplace_back(make_shared<WaterPool>(X1, Y1, this));
                    create = true;
                }
            }
        }
    }
    actorsDoSomething();
    removeDeadActors();
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    //removes the ice
    for (int x = 0; x < VIEW_WIDTH; x++)
    {
        for (int y = 0; y < VIEW_HEIGHT - 4; y++)
        {
            if (ice_array[x][y] != nullptr)
                ice_array[x][y].reset();
        }
    }
    //removes the Iceman
    iceman_ptr.reset();
    vector<shared_ptr<Actor>>::iterator a = actors.begin();
    while (a != actors.end())
    {
        if ((*a)->isAlive())
            a = actors.erase(a);
        else
            a++;
    }
    numofProtesters = 0;
}

bool StudentWorld::objectRadius(int X1, int Y1, int X2, int Y2)
{
    int x = abs(X2 - X1), y = abs(Y2 - Y1);
    int z = sqrt(pow(x, 2) + pow(y, 2));
    if (z < 6)
        return true;
    else
        return false;
}



void StudentWorld::makeSquirt()
{
    Actor::Direction direction = iceman_ptr->getDirection();
    switch (direction)
    {
        case Actor::up:
            actors.emplace_back(make_shared<Squirt>(iceman_ptr->getX(), iceman_ptr->getY() + 2,  iceman_ptr->getDirection(), this));
            break;
        case Actor::down:
            actors.emplace_back(make_shared<Squirt>(iceman_ptr->getX(), iceman_ptr->getY() - 2,  iceman_ptr->getDirection(), this));
            break;
        case Actor::right:
            actors.emplace_back(make_shared<Squirt>(iceman_ptr->getX() + 2, iceman_ptr->getY(),  iceman_ptr->getDirection(), this));
            break;
        case Actor::left:
            actors.emplace_back(make_shared<Squirt>(iceman_ptr->getX() - 2, iceman_ptr->getY(),  iceman_ptr->getDirection(), this));
            break;
        default:
            break;
    }
}

void StudentWorld::useSonar(int x, int y)
{
    for(auto p: actors)
    {
        int A = abs(x - p->getX()), B = abs(y - p->getY());
        int C = sqrt(pow(A, 2) + pow(B, 2));
        if (C <= 12 && p->isVisible() == false)
            p->setVisible(true);
    }
}

bool StudentWorld::icemanCheck(int X, int Y, int R)
{
    int x = abs(X - iceman_ptr->getX()), y = abs(Y - iceman_ptr->getY());
    if (x <= R && y <= R)
        return true;
    else
        return false;
}

void StudentWorld::annoyIceman(int damage)
{
    iceman_ptr->annoyed(damage);
}

void StudentWorld::increaseSonar()
{
    iceman_ptr->setSonar();
}

void StudentWorld::increaseWater()
{
    iceman_ptr->setWater();
}

bool StudentWorld::boulderCheck(int x, int y)
{
    for (auto a: actors)
    {
        shared_ptr<Boulder> b(dynamic_pointer_cast<Boulder>(a));
        if (b)
        {
            int X1 = abs(x - b->getX()), Y1 = abs(y - b->getY());
            if (X1 <= 3 && Y1 <= 3)
                return true;
        }
    }
    return false;
}

bool StudentWorld::BoulderIceCheck(int x, int y)
{
    if (y == getIcemanY())
    {
        int Xdistance = abs(getIcemanX() - x);
        for (int i = 0; i < Xdistance; i++)
        {
            if (x < getIcemanX() && actualCheck(x + 1, y))
                return true;
            if (x > getIcemanX() && actualCheck(x - 1, y))
                return true;
        }
    }
    if (x == getIcemanX())
    {
        int Ydistance = abs(getIcemanY() - y);
        for (int j = 0; j < Ydistance; j++)
        {
            if (y < getIcemanY() && actualCheck(x, y + 1))
                return true;
            if (y > getIcemanY() && actualCheck(x, y - 1))
                return true;
        }
    }
    return false;
}

//return true there is boulder/ice
//return false if there is nothing there
bool StudentWorld::actualCheck(int x, int y)
{
    for (int i = x; i < x + 4; i++)
    {
        for (int j = y; j < y + 4; j++)
        {
            if (i < 64 && j < 60)
            {
                if (ice_array[i][j] != nullptr)
                {
                    return true;
                }
            }
        }
    }
    if (boulderCheck(x, y))
        return true;
    return false;
}

int StudentWorld::protesterCheck(int x, int y)
{
    for (auto a: actors)
    {
        shared_ptr<RegularProtester> b(dynamic_pointer_cast<RegularProtester>(a));
        if (b)
        {
            int X = abs(x - b->getX()), Y = abs(y - b->getY());
            if (X <= 3 && Y <= 3)
            {
                return 1;
            }
        }
        shared_ptr<HardcoreProtester> c(dynamic_pointer_cast<HardcoreProtester>(a));
        if (c)
        {
            int X = abs(x - c->getX()), Y = abs(y - c->getY());
            if (X <= 3 && Y <= 3)
            {
                return 2;
            }
        }
    }
    return 0;
}

void StudentWorld::annoyProtester(int damage, int x, int y)
{
    for (auto a: actors)
    {
        shared_ptr<RegularProtester> b(dynamic_pointer_cast<RegularProtester>(a));
        if (b)
        {
            int X = abs(x - b->getX()), Y = abs(y - b->getY());
            if (X <= 3 && Y <= 3)
            {
                b->annoyed(damage);
            }
        }
        shared_ptr<HardcoreProtester> c(dynamic_pointer_cast<HardcoreProtester>(a));
        if (c)
        {
            int X = abs(x - c->getX()), Y = abs(y - c->getY());
            if (X <= 3 && Y <= 3)
            {
                c->annoyed(damage);
            }
        }
    }
}

void StudentWorld::dropGold(int x, int y)
{
    actors.emplace_back(make_shared<GoldNugget>(x, y, true, this));
}

void StudentWorld::icemanGold()
{
    iceman_ptr->pickUpGold();
}

void StudentWorld::actorsDoSomething()
{
    iceman_ptr->doSomething();
    for (auto a: actors)
    {
        if (a->isAlive())
            a->doSomething();
    }
}

void StudentWorld::removeDeadActors()
{
    vector<shared_ptr<Actor>>::iterator a = actors.begin();
    while (a != actors.end())
    {
        if ((*a)->isAlive() == false)
            a = actors.erase(a);
        else
            a++;
    }
}

void StudentWorld::firstBreadthSearch(int x, int y)
{
    for (int i = 0; i < 61; i++)
    {
        for (int j = 0; j < 61; j++)
        {
            if (!actualCheck(i, j))
                path[i][j] = 10000; //4x4 square from bottom left is valid move
            else
                path[i][j] = -1; //not valid path, has boulder/ice
        }
    }
    path[x][y] = 0; //start distance = 0
    Node current;
    queue<Node> p;
    p.push(Node(x, y, 0)); //push start node
    int distance;
    while (!p.empty())
    {
        current = p.front();
        p.pop();
        distance = current.getDistance() + 1;
        //if valid move (above)
        if (path[current.getX()][current.getY() + 1] > 0 && current.getY() <= 60)
        {
            //if not been visited
            if (path[current.getX()][current.getY() + 1] == 10000)
            {
                //distance = pervious distance + 1
                path[current.getX()][current.getY() + 1] = distance;
                //push valid move
                p.push(Node(current.getX(), current.getY() + 1, distance));
            }
            //if been visited but has greater distance than current distance
            else if (path[current.getX()][current.getY() + 1] > distance)
                //distance = current distance
                path[current.getX()][current.getY() + 1] = distance;
        }
        if (path[current.getX()][current.getY() - 1] > 0 && current.getY() >= 0)
        {
            if (path[current.getX()][current.getY() - 1] == 10000)
            {
                path[current.getX()][current.getY() - 1] = distance;
                p.push(Node(current.getX(), current.getY() - 1, distance));
            }
            else if (path[current.getX()][current.getY() - 1] > distance)
                path[current.getX()][current.getY() - 1] = distance;
        }
        if (path[current.getX() + 1][current.getY()] > 0 && current.getX() <= 60)
        {
            if (path[current.getX() + 1][current.getY()] == 10000)
            {
                path[current.getX() + 1][current.getY()] = distance;
                p.push(Node(current.getX() + 1, current.getY(), distance));
            }
            else if (path[current.getX() + 1][current.getY()] > distance)
                path[current.getX() + 1][current.getY()] = distance;
        }
        if (path[current.getX() - 1][current.getY()] > 0 && current.getX() >= 0)
        {
            if (path[current.getX() - 1][current.getY()] == 10000)
            {
                path[current.getX() - 1][current.getY()] = distance;
                p.push(Node(current.getX() - 1, current.getY(), distance));
            }
            else if (path[current.getX() - 1][current.getY()] > distance)
                path[current.getX() - 1][current.getY()] = distance;
        }
    }
}

string StudentWorld::leave()
{
    //start from exit
    int distance =  path[60][60];
    stack<string> shortestPath;
    Node current(60, 60, distance);
    while (distance > 0)
    {
        //find path from exit to protester
        distance--;
        if (path[current.getX()][current.getY() + 1] == distance && current.getY() + 1 <= 60)
        {
            shortestPath.push("down");
            current = Node(current.getX(), current.getY() + 1, distance);
        }
        else if (path[current.getX()][current.getY() - 1] == distance && current.getY() - 1 >= 0){
            shortestPath.push("up");
            current = Node(current.getX(), current.getY() - 1, distance);
        }
        else if (path[current.getX() + 1][current.getY()] == distance && current.getX() + 1 <= 60){
            shortestPath.push("left");
            current = Node(current.getX() + 1, current.getY(), distance);
        }
        else if (path[current.getX() - 1][current.getY()] == distance && current.getX() - 1 >= 0){
            shortestPath.push("right");
            current = Node(current.getX() - 1, current.getY(), distance);
        }
    }
    return shortestPath.top();
}

int StudentWorld::movesToIceman()
{
    return path[getIcemanX()][getIcemanY()];
}

string StudentWorld::moveToIceman()
{
    int distance =  path[getIcemanX()][getIcemanY()];
    stack<string> shortestPath;
    Node current(getIcemanX(), getIcemanY(), distance);
    while (distance > 0)
    {
        //find path from iceman to hardcore protester
        distance--;
        if (path[current.getX()][current.getY() + 1] == distance && current.getY() + 1 <= 60)
        {
            shortestPath.push("down");
            current = Node(current.getX(), current.getY() + 1, distance);
        }
        else if (path[current.getX()][current.getY() - 1] == distance && current.getY() - 1 >= 0){
            shortestPath.push("up");
            current = Node(current.getX(), current.getY() - 1, distance);
        }
        else if (path[current.getX() + 1][current.getY()] == distance && current.getX() + 1 <= 60){
            shortestPath.push("left");
            current = Node(current.getX() + 1, current.getY(), distance);
        }
        else if (path[current.getX() - 1][current.getY()] == distance && current.getX() - 1 >= 0){
            shortestPath.push("right");
            current = Node(current.getX() - 1, current.getY(), distance);
        }
    }
    return shortestPath.top();
}

void StudentWorld::bribe(int x, int y)
{
    for (auto a: actors)
    {
        shared_ptr<RegularProtester> b(dynamic_pointer_cast<RegularProtester>(a));
        if (b)
        {
            int X = abs(x - b->getX()), Y = abs(y - b->getY());
            if (X <= 3 && Y <= 3)
            {
                b->setLeaveOilField();
            }
        }
        shared_ptr<HardcoreProtester> c(dynamic_pointer_cast<HardcoreProtester>(a));
        if (c)
        {
            int X = abs(x - c->getX()), Y = abs(y - c->getY());
            if (X <= 3 && Y <= 3)
            {
                c->setStareTicks();
            }
        }
    }
}
