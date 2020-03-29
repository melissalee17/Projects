#include "Actor.h"
#include "StudentWorld.h"
#include "GameController.h"
#include <algorithm>
using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

void Iceman::doSomething() {
    //check if player is dead
    if (getWorld()->getLives() == 0 && h_pts <= 0){
        alive = false;
        return;
    }
    //removes ice if Iceman clears the ice
    for (int x = getX(); x < getX() + 4; x++)
    {
        for (int y = getY(); y < getY() + 4; y++)
        {
            
            if (getWorld()->getIce(x, y) != nullptr) {
                getWorld()->setIce(x, y);
                GameController::getInstance().playSound(SOUND_DIG);
            }
        }
    }
    int ch;
    //moves the Iceman
    if (getWorld()->getKey(ch) == true)
    {
        switch (ch) {
            case KEY_PRESS_ESCAPE:
                //lose a life and exit level
                alive = false;
                break;
            case KEY_PRESS_SPACE:
                //create a squirt
                if (water > 0){
                    getWorld()->makeSquirt();
                    getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                    water--;

                }
                break;
            case 'Z':
            case 'z':
                if (sonar > 0){
                    getWorld()->useSonar(getX(), getY());
                    getWorld()->playSound(SOUND_SONAR);
                    sonar--;
                }
                break;
            case KEY_PRESS_TAB:
                if (gold > 0){
                    getWorld()->dropGold(getX(), getY());
                    gold--;
                }
            case KEY_PRESS_UP:
                if(getDirection() != up){
                    setDirection(up);
                    break;
                }
                if (getY() != 60)
                {
                    if (!getWorld()->boulderCheck(getX(), getY() + 1))
                        moveTo(getX(), getY() + 1);
                }
                break;
            case KEY_PRESS_DOWN:
                if (getDirection() != down){
                    setDirection(down);
                    break;
                }
                if (getY() != 0)
                {
                    if (!getWorld()->boulderCheck(getX(), getY() - 1))
                        moveTo(getX(), getY() - 1);
                }
                break;
            case KEY_PRESS_RIGHT:
                if (getDirection() != right){
                    setDirection(right);
                    break;
                }
                if (getX() != 60)
                {
                    if (!getWorld()->boulderCheck(getX() + 1, getY()))
                        moveTo(getX() + 1, getY());
                }
                break;
            case KEY_PRESS_LEFT:
                if (getDirection() != left){
                    setDirection(left);
                    break;
                }
                if (getX() != 0)
                {
                    if (!getWorld()->boulderCheck(getX() - 1, getY()))
                        moveTo(getX() - 1, getY());
                }
                break;
            default:
                break;
        }
    }
}

void Iceman::annoyed(int damage)
{
    if (alive)
    {
        h_pts -= damage;
        if (h_pts <= 0)
        {
            alive = false;
            getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
        }
    }
}

void Boulder::doSomething() {
    if (!alive)
        return;
    if (stable)
    {
        for (int x = getX(); x < getX() + 4; x++)
        {
            int y = getY() - 1;
            if (getWorld()->getIce(x, y) != nullptr)
                break;
            if (x - getX() == 3)
                stable = false;
        }
    }
    else
    {
        if (waiting < 30)
            waiting++;
        if (waiting == 30)
            fall();
    }
}

void Boulder::fall()
{
    for (int x = getX(); x < getX() + 4; x++)
    {
        int y = getY() - 1;
        if (getWorld()->getIce(x, y) != nullptr || y == 0)
        {
            alive = false;
            setVisible(false);
        }
        if (x - getX() == 3)
        {
            if (getWorld()->icemanCheck(getX(), getY(), 3))
            {
                getWorld()->annoyIceman(100);
            }
            if (getWorld()->protesterCheck(getX(), getY()) == 1 || getWorld()->protesterCheck(getX(), getY()) == 2)
            {
                getWorld()->annoyProtester(100, getX(), getY());
            }
            moveTo(getX(), getY() - 1);
            GameController::getInstance().playSound(SOUND_FALLING_ROCK);
        }
    }
}

void Squirt::doSomething()
{
    if (!alive)
        return;
    if (distance == 0)
        alive = false;
    else
    {
        if (getWorld()->protesterCheck(getX(), getY()) == 1 || getWorld()->protesterCheck(getX(), getY()) == 2)
        {
            alive = false;
            getWorld()->annoyProtester(2, getX(), getY());
        }
        if (distance == 0){
            alive = false;
            return;
        }
        switch (getDirection())
        {
            case up:
                if (getWorld()->actualCheck(getX(), getY() + 1)){
                    alive = false;
                }
                else {
                    moveTo(getX(), getY() + 1);
                    distance--;
                }
                break;
            case down:
                if (getWorld()->actualCheck(getX(), getY() - 1)){
                    alive = false;
                }
                else {
                    moveTo(getX(), getY() - 1);
                    distance--;
                }
                break;
            case right:
                if (getWorld()->actualCheck(getX() + 1, getY())){
                    alive = false;
                }
                else {
                    moveTo(getX() + 1, getY());
                    distance--;
                }
                break;
            case left:
                if (getWorld()->actualCheck(getX() - 1, getY())){
                    alive = false;
                }
                else {
                    moveTo(getX() - 1, getY());
                    distance--;
                }
                break;
            default:
                break;
            }
    }
}

void Barrel::doSomething()
{
    if (!alive)
        return;
    if (getWorld()->icemanCheck(getX(), getY(), 4) && !visible)
    {
        setVisible(true);
        visible = true;
        return;
    }
    if (getWorld()->icemanCheck(getX(), getY(), 3))
    {
        alive = false;
        getWorld()->playSound(SOUND_FOUND_OIL);
        getWorld()->increaseScore(1000);
        getWorld()->oilFound();
    }
}

void GoldNugget::doSomething()
{
    if (!alive)
        return;
    //iceman gold nugget
    //check if iceman is nearby
    if (!temp && !visible && getWorld()->icemanCheck(getX(), getY(), 4))
    {
        setVisible(true);
        visible = true;
        return;
    }
    //check if picked up by iceman
    if (!temp && visible && getWorld()->icemanCheck(getX(), getY(), 3))
    {
        alive = false;
        getWorld()->icemanGold();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(10);
    }
    
    //protester gold nugget
    if (temp && waiting > 0)
        waiting--;
    if (temp && waiting == 0){
        alive = false;
    }
    if (getWorld()->protesterCheck(getX(), getY()) == 1)
    {
        getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
        getWorld()->bribe(getX(), getY());
        alive = false;
        getWorld()->increaseScore(25);
    }
    else if (getWorld()->protesterCheck(getX(), getY()) == 2)
    {
        getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
        getWorld()->bribe(getX(), getY());
        alive = false;
        getWorld()->increaseScore(50);
    }
}

void SonarKit::doSomething()
{
    if (!alive)
        return;
    int level = getWorld()->getLevel();
    ticks = max(100, 300 - 10 * level);
    if (getWorld()->icemanCheck(getX(), getY(), 3))
    {
        alive = false;
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(75);
        getWorld()->increaseSonar();
    }
    if (ticks > 0)
        ticks--;
    else
        alive = false;
}

void WaterPool::doSomething()
{
    if (!alive)
        return;
    int level = getWorld()->getLevel();
    ticks = max(100, 300 - 10 * level);
    if (getWorld()->icemanCheck(getX(), getY(), 3))
    {
        alive = false;
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseWater();
        getWorld()->increaseScore(100);
    }
    if (waiting < ticks)
        waiting++;
    else
        alive = false;
}

RegularProtester::RegularProtester(int x, int y, StudentWorld *world): Actor(IID_PROTESTER, x, y, left, 1, 0, true, world), h_points(5), leaveOilField(false) , annoyTicks(15) {
    perpendicularTicks = 100;
    int level = getWorld()->getLevel();
    restTicks = std::max(0, 3 - level/4);
    numSquares = rand() % 60 + 8;
}

void RegularProtester::doSomething()
{
    if (!alive)
        return;
    //rest
    if (restTicks > 0){
        restTicks--;
        return;
    }
    //leave oil field
    if (leaveOilField)
    {
        if (getX() == 60 && getY() == 60){
            alive = false;
            return;
        }
        else
        {
            getWorld()->firstBreadthSearch(getX(), getY());
            string direction = getWorld()->leave();
            if (direction == "up"){
                setDirection(up);
                moveTo(getX(), getY() + 1);
            }
            else if (direction == "down"){
                setDirection(down);
                moveTo(getX(), getY() - 1);
            }
            else if (direction == "right"){
                setDirection(right);
                moveTo(getX() + 1, getY());
            }
            else if (direction == "left"){
                setDirection(left);
                moveTo(getX() - 1, getY());
            }
            return;
        }
    }
    if (restTicks == 0)
    {
        if (annoyTicks > 0)
            annoyTicks--;
        if (perpendicularTicks > 0)
            perpendicularTicks--;
        //can annoy iceman
        if (isFacing() && getWorld()->icemanCheck(getX(), getY(), 4))
        {
            if (annoyTicks == 0)
            {
                getWorld()->playSound(SOUND_PROTESTER_YELL);
                getWorld()->annoyIceman(2);
                annoyTicks = 15;
                int level = getWorld()->getLevel();
                restTicks = max(0, 3 - level/4);
                return;
            }
        }
        //in same axis as iceman but too far away to annoy iceman
        if (getWorld()->getIcemanX() == getX() || getWorld()->getIcemanY() == getY())
        {
            //no ice or boulder
            if (!getWorld()->icemanCheck(getX(), getY(), 4) && !getWorld()->BoulderIceCheck(getX(), getY()))
            {
                //iceman in x-axis
                if (getWorld()->getIcemanX() == getX())
                {
                    //above x-axis
                    if (getWorld()->getIcemanY() >= getY()){
                        setDirection(up);
                        moveTo(getX(), getY() + 1);
                    }
                    //below x-axis
                    else
                    {
                        setDirection(down);
                        moveTo(getX(), getY() - 1);
                    }
                }
                //iceman in y-axis
                if (getWorld()->getIcemanY() == getY())
                {
                    //right of y-axis
                    if (getWorld()->getIcemanX() >= getX())
                    {
                        setDirection(right);
                        moveTo(getX() + 1, getY());
                    }
                    //left of y-axis
                    else
                    {
                        setDirection(left);
                        moveTo(getX() - 1, getY());
                    }
                }
                int level = getWorld()->getLevel();
                restTicks = max(0, 3 - level/4);
                return;
            }
        }
        else
        {
            if (numSquares > 0)
                numSquares--;
            if (numSquares == 0)
            {
                newDirection();
                numSquares = rand() % 60 + 8;
            }
        }
        if (perpendicularTicks == 0)
        {
            atIntersection();
            numSquares = rand() % 60 + 8;
        }
        if (numSquares != 0)
        {
            switch (getDirection()) {
                case up:
                    if (!getWorld()->actualCheck(getX(), getY() + 1) && getY() + 1 <= 60){
                        setDirection(up);
                        moveTo(getX(), getY() + 1);
                    }
                    else
                        numSquares = 0;
                    break;
                case down:
                    if (!getWorld()->actualCheck(getX(), getY() - 1) && getY() - 1 >= 0){
                        setDirection(down);
                        moveTo(getX(), getY() - 1);
                    }
                    else
                        numSquares = 0;
                    break;
                case right:
                    if (!getWorld()->actualCheck(getX() + 1, getY()) && getX() + 1 <= 60){
                        setDirection(right);
                        moveTo(getX() + 1, getY());
                    }
                    else
                        numSquares = 0;
                    break;
                case left:
                    if (!getWorld()->actualCheck(getX() - 1, getY()) && getX() - 1 >= 0){
                        setDirection(left);
                        moveTo(getX() - 1, getY());
                    }
                    else
                        numSquares = 0;
                    break;
                default:
                    break;
            }
        }
        int level = getWorld()->getLevel();
        restTicks = max(0, 3 - level/4);
    }
}

void RegularProtester::annoyed(int damage)
{
    if (leaveOilField)
        return;
    h_points -= damage;
    if (h_points > 0)
    {
        getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
        int level = getWorld()->getLevel();
        int N = max(50, 100 - level * 10);
        restTicks = N;
    }
    else
    {
        leaveOilField = true;
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        if (damage == 100){
            getWorld()->increaseScore(500);
            restTicks = 10;
        }
        if (damage == 2){
            getWorld()->increaseScore(100);
            restTicks = 0;
        }
    }
}

bool RegularProtester::isFacing()
{
    //right or above protester
    if (getWorld()->getIcemanX() >= getX() && getWorld()->getIcemanY() >= getY())
    {
        if (getDirection() == up || getDirection() == right)
            return true;
    }
    //left or above protester
    else if (getWorld()->getIcemanX() <= getX() && getWorld()->getIcemanY() >= getY())
    {
        if (getDirection() == up || getDirection() == left)
            return true;
    }
    //right or below protester
    else if (getWorld()->getIcemanX() >= getX() && getWorld()->getIcemanY() <= getY())
    {
        if (getDirection() == down || getDirection() == right)
            return true;
    }
    //left or below protester
    else if (getWorld()->getIcemanX() <= getX() && getWorld()->getIcemanY() <= getY())
    {
        if (getDirection() == down && getDirection() == left)
            return true;
    }
    return false;
}

void RegularProtester::newDirection()
{
    bool changeDirection = true;
    while (changeDirection)
    {
        int randomNumber = rand() % 4; //0 = up, 1 = down, 2 = right, 3 = left
        switch (randomNumber)
        {
            //up
            case 0:
                //if no boulder/ice
                if (!getWorld()->actualCheck(getX(), getY() + 1))
                {
                    //if in playing field
                    if (getY() + 1 <= 60)
                    {
                        //change direction
                        setDirection(up);
                        changeDirection = false;
                    }
                }
                break;
            //down
            case 1:
                //if no boulder/ice
                if (!getWorld()->actualCheck(getX(), getY() - 1))
                {
                    //if in playing field
                    if (getY() - 1 >= 0)
                    {
                        //change direction
                        setDirection(down);
                        changeDirection = false;
                    }
                }
                break;
            //right
            case 2:
                //if no boulder/ice
                if (!getWorld()->actualCheck(getX() + 1, getY()))
                {
                    //if in playing field
                    if (getX() + 1 <= 60)
                    {
                        //change direction
                        setDirection(right);
                        changeDirection = false;
                    }
                }
                break;
            //left
            case 3:
                //if no boulder/ice
                if (!getWorld()->actualCheck(getX() - 1, getY()))
                {
                    //if in playing field
                    if (getX() - 1 >= 0)
                    {
                        //change direction
                        setDirection(left);
                        changeDirection = false;
                    }
                }
                break;
            default:
                break;
        }
    }
}

void RegularProtester::atIntersection()
{
    bool turnUp = false, turnDown = false, turnRight = false, turnLeft = false;
    //no boulder/ice up, not currently facing up or down, and in bounds
    if (!getWorld()->actualCheck(getX(), getY() + 1) && getDirection() != up && getDirection() != down && getY() + 1 <= 60)
        turnUp = true;
    //no boulder/ice go down
    if (!getWorld()->actualCheck(getX(), getY() - 1) && getDirection() != up && getDirection() != down && getY() - 1 >= 0)
        turnDown = true;
    //no boulder/ice go right
    if (!getWorld()->actualCheck(getX() + 1, getY()) && getDirection() != left && getDirection() != right && getX() + 1 <= 60)
        turnRight = true;
    //no boulder/ice go left
    if (!getWorld()->actualCheck(getX() - 1, getY()) && getDirection() != left && getDirection() != right && getX() - 1 >= 0)
        turnLeft = true;
    //if currently facing up or down
    if (getDirection() == up || getDirection() == down)
    {
        //if can turn left or right
        if (turnLeft && turnRight)
        {
            int randomNumber = rand() % 2;
            //0 = right
            if (randomNumber == 0)
                setDirection(right);
            //1 = left
            else
                setDirection(left);
        }
        //if can turn right
        else if (turnRight)
            setDirection(right);
        //if can turn left
        else if (turnLeft)
            setDirection(left);
        perpendicularTicks = 100;
    }
    //if currently facing right or left
    if (getDirection() == right || getDirection() == left)
    {
        //if can go up or down
        if (turnUp && turnDown)
        {
            int randomNumber = rand() % 2;
            //0 = up
            if (randomNumber == 0)
                setDirection(up);
            //1 = down
            else
                setDirection(down);
        }
        //if can go up
        else if (turnUp)
            setDirection(up);
        //if can go down
        else if (turnDown)
            setDirection(down);
        perpendicularTicks = 100;
    }
}

void RegularProtester::setLeaveOilField()
{
    leaveOilField = true;
}

HardcoreProtester::HardcoreProtester(int x, int y, StudentWorld *world) : Actor(IID_HARD_CORE_PROTESTER, x, y, left, 1, 0, true, world)
{
    h_points = 10;
    leaveOilField = false;
    annoyTicks = 15;
    perpendicularTicks = 100;
    numSquares = rand() % 60 + 8;
    int level = getWorld()->getLevel();
    restTicks = max(0, 3 - level/4);
}

void HardcoreProtester::doSomething()
{
    if (!alive)
        return;
    //rest
    if (restTicks > 0){
        restTicks--;
        return;
    }
    //leave oil field
    if (leaveOilField)
    {
        if (getX() == 60 && getY() == 60){
            alive = false;
            return;
        }
        else
        {
            getWorld()->firstBreadthSearch(getX(), getY());
            string direction = getWorld()->leave();
            if (direction == "up"){
                setDirection(up);
                moveTo(getX(), getY() + 1);
            }
            else if (direction == "down"){
                setDirection(down);
                moveTo(getX(), getY() - 1);
            }
            else if (direction == "right"){
                setDirection(right);
                moveTo(getX() + 1, getY());
            }
            else if (direction == "left"){
                setDirection(left);
                moveTo(getX() - 1, getY());
            }
            return;
        }
    }
    if (restTicks == 0)
    {
        if (annoyTicks > 0)
            annoyTicks--;
        if (perpendicularTicks > 0)
            perpendicularTicks--;
        //can annoy iceman
        if (isFacing() && getWorld()->icemanCheck(getX(), getY(), 4))
        {
            if (annoyTicks == 0)
            {
                getWorld()->playSound(SOUND_PROTESTER_YELL);
                getWorld()->annoyIceman(2);
                annoyTicks = 15;
                int level = getWorld()->getLevel();
                restTicks = max(0, 3 - level/4);
                return;
            }
        }
        if (!getWorld()->icemanCheck(getX(), getY(), 4))
        {
            //doesn't work
            /*int level = getWorld()->getLevel();
            int M = 16 + level * 2;
            getWorld()->firstBreadthSearch(getX(), getY());
            if (getWorld()->movesToIceman() <= M)
            {
                string direction = getWorld()->moveToIceman();
                if (direction == "up"){
                    setDirection(up);
                    moveTo(getX(), getY() + 1);
                }
                else if (direction == "down"){
                    setDirection(down);
                    moveTo(getX(), getY() - 1);
                }
                else if (direction == "right"){
                    setDirection(right);
                    moveTo(getX() + 1, getY());
                }
                else if (direction == "left"){
                    setDirection(left);
                    moveTo(getX() - 1, getY());
                }
            }
            restTicks = max( 0, 3 - level/4);
            return;*/
        }
        //6
        //in same axis as iceman but too far away to annoy iceman
        if (getWorld()->getIcemanX() == getX() || getWorld()->getIcemanY() == getY())
        {
            //no ice or boulder
            if (!getWorld()->icemanCheck(getX(), getY(), 4) && !getWorld()->BoulderIceCheck(getX(), getY()))
            {
                //iceman in x-axis
                if (getWorld()->getIcemanX() == getX())
                {
                    //above x-axis
                    if (getWorld()->getIcemanY() >= getY() && !getWorld()->actualCheck(getX(), getY() + 1)){
                        setDirection(up);
                        moveTo(getX(), getY() + 1);
                    }
                    //below x-axis
                    else
                    {
                        if (!getWorld()->actualCheck(getX(), getY() - 1)){
                            setDirection(down);
                            moveTo(getX(), getY() - 1);
                        }
                    }
                }
                //iceman in y-axis
                if (getWorld()->getIcemanY() == getY())
                {
                    //right of y-axis
                    if (getWorld()->getIcemanX() >= getX() && !getWorld()->actualCheck(getX() + 1, getY()))
                    {
                        setDirection(right);
                        moveTo(getX() + 1, getY());
                    }
                    //left of y-axis
                    else
                    {
                        if (!getWorld()->actualCheck(getX() - 1, getY())){
                            setDirection(left);
                            moveTo(getX() - 1, getY());
                        }
                    }
                }
                int level = getWorld()->getLevel();
                restTicks = max(0, 3 - level/4);
                return;
            }
        }
        else
        {
            //7
            //pick new direction
            if (numSquares > 0)
                numSquares--;
            if (numSquares == 0)
            {
                newDirection();
                numSquares = rand() % 60 + 8;
            }
        }
        //8
        //at intersection
        if (perpendicularTicks == 0)
        {
            atIntersection();
            numSquares = rand() % 60 + 8;
        }
        if (numSquares != 0)
        {
            switch (getDirection()) {
                case up:
                    if (!getWorld()->actualCheck(getX(), getY() + 1) && getY() + 1 <= 60){
                        setDirection(up);
                        moveTo(getX(), getY() + 1);
                    }
                    else
                        numSquares = 0;
                    break;
                case down:
                    if (!getWorld()->actualCheck(getX(), getY() - 1) && getY() - 1 >= 0){
                        setDirection(down);
                        moveTo(getX(), getY() - 1);
                    }
                    else
                        numSquares = 0;
                    break;
                case right:
                    if (!getWorld()->actualCheck(getX() + 1, getY()) && getX() + 1 <= 60){
                        setDirection(right);
                        moveTo(getX() + 1, getY());
                    }
                    else
                        numSquares = 0;
                    break;
                case left:
                    if (!getWorld()->actualCheck(getX() - 1, getY()) && getX() - 1 >= 0){
                        setDirection(left);
                        moveTo(getX() - 1, getY());
                    }
                    else
                        numSquares = 0;
                    break;
                default:
                    break;
            }
        }
        int level = getWorld()->getLevel();
        restTicks = max(0, 3 - level/4);
    }
}

void HardcoreProtester::annoyed(int damage)
{
    if (leaveOilField)
        return;
    h_points -= damage;
    if (h_points > 0)
    {
        getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
        int level = getWorld()->getLevel();
        int N = max(50, 100 - level * 10);
        restTicks = N;
    }
    else
    {
        leaveOilField = true;
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        restTicks = 0;
        if (damage == 100){
            getWorld()->increaseScore(500);
            restTicks = 10;
        }
        if (damage == 2)
            getWorld()->increaseScore(250);
    }
}

bool HardcoreProtester::isFacing()
{
    //right or above protester
    if (getWorld()->getIcemanX() >= getX() && getWorld()->getIcemanY() >= getY())
    {
        if (getDirection() == up || getDirection() == right)
            return true;
    }
    //left or above protester
    else if (getWorld()->getIcemanX() <= getX() && getWorld()->getIcemanY() >= getY())
    {
        if (getDirection() == up || getDirection() == left)
            return true;
    }
    //right or below protester
    else if (getWorld()->getIcemanX() >= getX() && getWorld()->getIcemanY() <= getY())
    {
        if (getDirection() == down || getDirection() == right)
            return true;
    }
    //left or below protester
    else if (getWorld()->getIcemanX() <= getX() && getWorld()->getIcemanY() <= getY())
    {
        if (getDirection() == down && getDirection() == left)
            return true;
    }
    return false;
}

void HardcoreProtester::newDirection()
{
    bool changeDirection = true;
    while (changeDirection)
    {
        int randomNumber = rand() % 4; //0 = up, 1 = down, 2 = right, 3 = left
        switch (randomNumber)
        {
                //up
            case 0:
                //if no boulder/ice
                if (!getWorld()->actualCheck(getX(), getY() + 1))
                {
                    //if in playing field
                    if (getY() + 1 <= 60)
                    {
                        //change direction
                        setDirection(up);
                        changeDirection = false;
                    }
                }
                break;
                //down
            case 1:
                //if no boulder/ice
                if (!getWorld()->actualCheck(getX(), getY() - 1))
                {
                    //if in playing field
                    if (getY() - 1 >= 0)
                    {
                        //change direction
                        setDirection(down);
                        changeDirection = false;
                    }
                }
                break;
                //right
            case 2:
                //if no boulder/ice
                if (!getWorld()->actualCheck(getX() + 1, getY()))
                {
                    //if in playing field
                    if (getX() + 1 <= 60)
                    {
                        //change direction
                        setDirection(right);
                        changeDirection = false;
                    }
                }
                break;
                //left
            case 3:
                //if no boulder/ice
                if (!getWorld()->actualCheck(getX() - 1, getY()))
                {
                    //if in playing field
                    if (getX() - 1 >= 0)
                    {
                        //change direction
                        setDirection(left);
                        changeDirection = false;
                    }
                }
                break;
            default:
                break;
        }
    }
}

void HardcoreProtester::atIntersection()
{
    bool turnUp = false, turnDown = false, turnRight = false, turnLeft = false;
    //no boulder/ice up, not currently facing up or down, and in bounds
    if (!getWorld()->actualCheck(getX(), getY() + 1) && getDirection() != up && getDirection() != down && getY() + 1 <= 60)
        turnUp = true;
    //no boulder/ice go down
    if (!getWorld()->actualCheck(getX(), getY() - 1) && getDirection() != up && getDirection() != down && getY() - 1 >= 0)
        turnDown = true;
    //no boulder/ice go right
    if (!getWorld()->actualCheck(getX() + 1, getY()) && getDirection() != left && getDirection() != right && getX() + 1 <= 60)
        turnRight = true;
    //no boulder/ice go left
    if (!getWorld()->actualCheck(getX() - 1, getY()) && getDirection() != left && getDirection() != right && getX() - 1 >= 0)
        turnLeft = true;
    //if currently facing up or down
    if (getDirection() == up || getDirection() == down)
    {
        //if can turn left or right
        if (turnLeft && turnRight)
        {
            int randomNumber = rand() % 2;
            //0 = right
            if (randomNumber == 0)
                setDirection(right);
            //1 = left
            else
                setDirection(left);
        }
        //if can turn right
        else if (turnRight)
            setDirection(right);
        //if can turn left
        else if (turnLeft)
            setDirection(left);
        perpendicularTicks = 100;
    }
    //if currently facing right or left
    if (getDirection() == right || getDirection() == left)
    {
        //if can go up or down
        if (turnUp && turnDown)
        {
            int randomNumber = rand() % 2;
            //0 = up
            if (randomNumber == 0)
                setDirection(up);
            //1 = down
            else
                setDirection(down);
        }
        //if can go up
        else if (turnUp)
            setDirection(up);
        //if can go down
        else if (turnDown)
            setDirection(down);
        perpendicularTicks = 100;
    }
}

void HardcoreProtester::setStareTicks()
{
    int level = getWorld()->getLevel();
    restTicks = max(50, 100 - level * 10);
}
