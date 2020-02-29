#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h" //added
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream
#include <string>
#include <iomanip>
using namespace std;

//Fix bacteria goin crazy when overlapping with socrates

//Condense init functions
GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    flamesActivate = false; //initialize each member variable to default
    sprayActivate = false;
    player = nullptr;
}

StudentWorld::~StudentWorld()
{
    cleanUp(); //Call cleanup function
}

int StudentWorld::init()
{
    pitNum = getLevel(); //set the number of pits to the current level
    player = new Socrates(this); //Dynamically allocate a new player
    int numObjects = getLevel(); //Level number is the number of pits to create
    for (int i = 0; i < numObjects; i++)
    {
        int xVal = 0;
        int yVal = 0;
        initHelper(xVal, yVal);
        members.push_back(new Pit(xVal, yVal, this)); //Create a new pit at the new X and Y coordinates
    }
    
    numObjects = min(5 * getLevel(), 25); //Determine number of Food objects to create
    for (int i = 0; i < numObjects; i++)
    {
        int xVal = 0;
        int yVal = 0;
        initHelper(xVal, yVal);
        members.push_back(new Food(xVal, yVal, this));
    }
    
    numObjects = max(180 - 20 * getLevel(), 20); //Determine number of Dirt Objects to create
    for (int i = 0; i < numObjects; i++)
    {
        int xVal = 0;
        int yVal = 0;
        initHelper(xVal, yVal);
        members.push_back(new Dirt(xVal, yVal, this));
    }
    setGameStatText(gameText()); //Set the initial gameText at the stop
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::initHelper(int& x, int& y)
{
    bool done = false;
    while (done == false) //While a valid x, y pairing has not been found
    {
        x = randInt(VIEW_WIDTH/2 - 120, VIEW_WIDTH/2 + 120); //Determine random x, y vals
        y = randInt(VIEW_HEIGHT/2 - 120, VIEW_HEIGHT/2 + 120);
        if (distance(false, x, y, VIEW_WIDTH/2, VIEW_HEIGHT/2) > 120) //Make sure that the distance to center is within range
        {
            continue;
        }
        if(checkInitOverlap(x, y)) //Check that it does not overlap with anything it shouldnt overlap with
        {
            done = true;
        }
    }
}

int StudentWorld::move()
{
    player -> doSomething(); //Have the player do something
    
    if (sprayActivate) //If the player decides to fire the spray
    {
        double x = player -> getX();
        double y = player -> getY();
        player -> getPositionInThisDirection(player -> getDirection(), SPRITE_WIDTH, x, y);
        createSpray(x, y, player -> getDirection()); //create a spray object in the direction Socrates is facing
    }
    sprayActivate = false;
    
    if(flamesActivate) //If the player decides to fire the flames
    {
        for (int i = 0; i < 16; i++)
        {
            double x = player -> getX();
            double y = player -> getY();
            player -> getPositionInThisDirection(i*22, SPRITE_WIDTH, x, y);
            createFlame(x, y, i*22); //Create 16 flame Actors around Socrates
        }
    }
    flamesActivate = false;
    
    list<Actor*>::iterator iterate;
    iterate = members.begin();
    
    while (iterate != members.end())
    {
        if ((*iterate) -> isAlive()) //Check if it is alive
        {
            (*iterate) -> doSomething(); //Have each Actor do something
            
            if (player -> getHitPoints() <= 0) //Check if the player is dead
            {
                playSound(SOUND_PLAYER_DIE); //Play the sound for the player's death
                decLives(); //Decrease number of lives player has
                delete player;
                return GWSTATUS_PLAYER_DIED;
            }
        
            bool allDead = true;
            list<Actor*>::iterator iterate1;
            iterate1 = members.begin();
            while (iterate1 != members.end())
            {
                if ((*iterate1) -> isAttacker()) //Check if any of the Actors are attackers (ie. bacterium still exist)
                {
                    allDead = false; //If so, not all are dead
                }
                iterate1++;
            }
            
            if (allDead && pitNum == 0) //If all bacteria are dead and if there are no pits left
            {
                delete player;
                playSound(SOUND_FINISHED_LEVEL); //The player has finished the level
                return GWSTATUS_FINISHED_LEVEL;
            }
        }
        iterate++;
    }
    
    //Delete any dead actors
    iterate = members.begin();
    list<Actor*>::iterator iterate2;
    iterate2 = members.begin();
    if (iterate2 != members.end())
    {
        iterate2++;
    }
    while (iterate != members.end())
    {
        if (!((*iterate) -> isAlive())) //If an Actor is declared no longer alive
        {
            if ((*iterate) -> isAttacker())
            {
                increaseScore(100);
            }
            delete (*iterate); //Destroy it
            members.erase(iterate);
            iterate = iterate2;
        }
        else
        {
            iterate++;
        }
        if (iterate2 != members.end())
        {
            iterate2++;
        }
    }
    
    //ADD NEW ACTORS
    int chanceFungus = max(510 - getLevel() * 10, 200); //Randomly create Fungus Actors
    int randFungus = randInt(0, chanceFungus - 1);
    if (randFungus == 0)
    {
        double randXCoord, randYCoord;
        genRandCircularCoord(randXCoord, randYCoord);
        createFungus(randXCoord, randYCoord); //Create the new Fungus
    }
    
    int chanceGoodie = max(510 - getLevel() * 10, 250); //Randomly create Goodies
    int randGoodie = randInt(0, chanceGoodie - 1);
    if (randGoodie == 0)
    {
        int randNum = randInt(0, 9); //Randomly decide which Goodie to create
        double randXCoord, randYCoord;
        genRandCircularCoord(randXCoord, randYCoord);
        if (randNum >= 0 && randNum <= 5)
        {
            createHealthGoodie(randXCoord, randYCoord); //Create a health goodie
        }
        
        else if (randNum >= 6 && randNum <= 8)
        {
            createFlameGoodie(randXCoord, randYCoord); //Create a flame goodie
        }
        
        else if (randNum == 9)
        {
            createExtraLifeGoodie(randXCoord, randYCoord); //Create extra life goodie
        }
    }
    setGameStatText(gameText()); //Update the game text
    return GWSTATUS_CONTINUE_GAME; //modified from everyone died
}

void StudentWorld::cleanUp()
{
    list<Actor*>::iterator iterate;
    list<Actor*>::iterator iterate1;
    
    iterate = members.begin();
    iterate1 = members.begin();
    if (iterate1 != members.end())
    {
        iterate1++;
    }
    
    while (iterate != members.end()) //Go through and delete every single Actor in the list
    {
        delete (*iterate);
        members.erase(iterate);
        iterate = iterate1;
        if (iterate1 != members.end())
        {
            iterate1++;
        }
    }
}

bool StudentWorld::generalOverlap(Actor& source, double x, double y, int hurt)
{
    list<Actor*>::iterator iterate;
    iterate = members.begin();
    while (iterate != members.end())
    {
        if (source.canDamage()) //If the Actor is one of Socrates' damaging sources
        {
            if ((*iterate) -> isDamageable()) //If the receiving Actor can be Damaged
            {
                if (overlap(false, x, y, (*iterate) -> getX(), (*iterate) -> getY())) //Check whether they overlap each other
                {
                    (*iterate) -> decreaseHitPoints(hurt); //Hurt the receiving end
                    if ((*iterate) -> getHitPoints() <= 0)
                    {
                        (*iterate) -> setAlive(false); //If it's dead do the following
                        (*iterate) -> deathActions();
                    }
                    else
                    {
                        (*iterate) -> hurtSound(); //Play the hurt sound
                    }
                    return true;
                }
            }
            
        }
        
        if (source.isAttacker())
        {
            if ((*iterate) -> isEdible()) //If the interaction is between an attacker and something edible
            {
                if (overlap(false, x, y, (*iterate) -> getX(), (*iterate) -> getY()))
                {
                    (*iterate) -> setAlive(false); //Destroy the edible object
                    return true;
                }
            }
        }
        iterate++;
    }
    return false;
}

double StudentWorld::distance(bool soc, double x1, double y1, double x2, double y2)
{
    if (soc == true)
    {
        x2 = player -> getX();
        y2 = player -> getY();
    }
    return sqrt(pow(x2 - x1, 2.0) + pow(y2 - y1, 2.0)); //Return the distance between two Actors
}

bool StudentWorld::overlap(bool soc, double x1, double y1, double x2, double y2)
{
    if (soc == true)
    {
        x2 = player -> getX();
        y2 = player -> getY();
    }
    
    if (distance(soc, x1, y1, x2, y2) <= SPRITE_WIDTH) //verify if overlap is less than or equal to
    {
        return true; //THEY DO OVERLAP
    }
    return false;
}

bool StudentWorld::movementOverlap(double x, double y)
{ //return true if there is anything wrong with the movement
    if (petriOverlap(x, y))
    {
        return true;
    }
    list<Actor*>::iterator iterate;
    iterate = members.begin();
    while (iterate != members.end())
    {
        if ((*iterate) -> canBlock()) //If the Actor is blocked in this scenario
        {
            if (distance(false, x, y, (*iterate) -> getX(), (*iterate) -> getY()) <= SPRITE_WIDTH/2)
            {
                return true; //Return saying it was blocked
            }
        }
        iterate++;
    }
    return false;
}

bool StudentWorld::petriOverlap(double x, double y)
{
    if (distance(false, x, y, VIEW_WIDTH/2, VIEW_HEIGHT/2) >= VIEW_RADIUS)
    {
        return true; //Check for going outside the bounds of the petri dish
    }
    return false;
}

void StudentWorld::findClosestFood(double currX, double currY, double& x, double& y)
{
    list<Actor*>::iterator iterate;
    iterate = members.begin();
    double minDist = VIEW_RADIUS;
    while (iterate != members.end())
    {
        if ((*iterate) -> isEdible()) //For every edible Actor
        {
            double foodX = (*iterate) -> getX();
            double foodY = (*iterate) -> getY();
            double currDist = distance(false, currX, currY, foodX, foodY);
            
            if (currDist <= VIEW_RADIUS) //Compare the distance to the current food with the current min distance
            {
                if (currDist < minDist)
                {
                    minDist = currDist; //Set the new Distance
                    x = foodX;
                    y = foodY;
                }
            }
        }
        iterate++;
    }
}

void StudentWorld::createFlame(double xLoc, double yLoc, double direction)
{
    members.push_back(new Flame(xLoc, yLoc, direction, this)); //Create new Flame object
}

void StudentWorld::createSpray(double xLoc, double yLoc, double direction)
{
    members.push_back(new Spray(xLoc, yLoc, direction, this)); //Create new Spray object
}

void StudentWorld::createHealthGoodie(double xLoc, double yLoc)
{
    int lifetime = max(randInt(0, 300 - 10 * getLevel() - 1), 50);
    members.push_back(new RestoreHealthGoodie(xLoc, yLoc, lifetime, this)); //Create new Restore Health Goodie
}

void StudentWorld::createFlameGoodie(double xLoc, double yLoc)
{
    int lifetime = max(randInt(0, 300 - 10 * getLevel() - 1), 50);
    members.push_back(new FlameThrowerGoodie(xLoc, yLoc, lifetime, this)); //Create new Flame Thrower Goodie
}

void StudentWorld::createExtraLifeGoodie(double xLoc, double yLoc)
{
    int lifetime = max(randInt(0, 300 - 10 * getLevel() - 1), 50);
    members.push_back(new ExtraLifeGoodie(xLoc, yLoc, lifetime, this)); //Create new Extra Life Goodie
}

void StudentWorld::createFungus(double xLoc, double yLoc)
{
    int lifetime = max(randInt(0, 300 - 10 * getLevel() - 1), 50);
    members.push_back(new Fungus(xLoc, yLoc, lifetime, this)); //Create new Fungus
}

void StudentWorld::createRegular(double xLoc, double yLoc)
{
    members.push_back(new RegularSalmonella(xLoc, yLoc, this)); //Create new Regular Salmonella
}

void StudentWorld::createAggressive(double xLoc, double yLoc)
{
    members.push_back(new AggressiveSalmonella(xLoc, yLoc, this)); //Create new Aggressive Salmonella
}

void StudentWorld::createEColi(double xLoc, double yLoc)
{
    members.push_back(new EColi(xLoc, yLoc, this)); //Create new EColi
}

void StudentWorld::createFood(double xLoc, double yLoc)
{
    members.push_back(new Food(xLoc, yLoc,this)); //Create new Food Actor
}

void StudentWorld::damageSocrates(int amount)
{
    player -> decreaseHitPoints(amount); //Decrease Socrates' hit points
    if (player -> getHitPoints() <= 0) 
    {
        player -> setAlive(false); //Set state to dead
    }
    else
    {
        playSound(SOUND_PLAYER_HURT); //Play Socrates' hurt sound
    }
}

void StudentWorld::activateFlames() //Activate flames
{
    flamesActivate = true;
}

void StudentWorld::activateSpray() //Activate sprays
{
    sprayActivate = true;
}

double StudentWorld::angleToSoc(double x, double y)
{
    double newAngle = (atan((player -> getY() - y)/(player -> getX() - x)))*180/PI;
    if (player -> getX() < x) //Find the angle to Socrates involving discrepancies with atan
    {
        newAngle = newAngle + 180;
    }
    return newAngle;
}

void StudentWorld::restoreHealth() //Restore Socrates to full health
{
    player -> setHitPoints(100);
}

void StudentWorld::addFlames()
{
    static_cast<Socrates*>(player) -> addFlames(5); //Add flames to current Stock
}

void StudentWorld::decreasePitNum() //Decrease the number of still-active pits
{
    pitNum = pitNum - 1;
}

//------------PRIVATE FUNCTIONS
void StudentWorld::genRandCircularCoord(double& randX, double& randY)
{
    int randDeg = randInt(0, 360);
    double randRad = randDeg * PI / 180;
    randX = VIEW_WIDTH/2 + VIEW_RADIUS*cos(randRad); //Generate a random circular coordinate for the goodie
    randY = VIEW_HEIGHT/2 + VIEW_RADIUS*sin(randRad);
}


bool StudentWorld::checkInitOverlap(double x, double y)
{
    std::list<Actor*>::iterator iterateMem;
    iterateMem = members.begin();
    
    while (iterateMem != members.end())
    {
        if (!((*iterateMem) -> canOverlap())) //Check whether the specific Actors are allowed to overlap
        {
            if (initDistOverlap(x, y, (*iterateMem) -> getX(), (*iterateMem) -> getY())) //Return false is they do
            {
                return false;
            }
        }
        iterateMem++;
    }
    return true;
}

bool StudentWorld::initDistOverlap(double x1, double y1, double x2, double y2)
{
    if (distance(false, x1, y1, x2, y2) < SPRITE_RADIUS*2) //Make sure the distance between two Actors is valid
    {
        return true;
    }
    return false;
}

std::string StudentWorld::gameText()
{
    ostringstream oss;
    oss.fill('0');
    
    if (getScore() < 0) //Set game text for negative score
    {
        oss << "Score: -" << setw(5) << (getScore() * -1) << "  Level: " << getLevel() << "  Lives: " << getLives() << "  health: " << player -> getHitPoints() << "  Sprays: " << static_cast<Socrates*>(player) -> getSprays() << "  Flames: " << static_cast<Socrates*>(player) -> getFlames();
    }
    
    else //Set game text for positive score
    {
        oss.fill('0');
        oss << "Score: " << setw(6) << getScore() << "  Level: " << getLevel() << "  Lives: " << getLives() << "  health: " << player -> getHitPoints() << "  Sprays: " << static_cast<Socrates*>(player) -> getSprays() << "  Flames: " << static_cast<Socrates*>(player) -> getFlames();
    }
    string s = oss.str();
    return s;
}
