#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>

//Feb 22, 2019 TO DO: Check sounds
//Check display
//Report


//--------ACTOR
Actor::Actor(int imageID, double startX, double startY, StudentWorld* myWorld, int startDirection, int depth)
:GraphObject(imageID, startX, startY, startDirection, depth) //Initialize the Actor with all its properties
{
    alive = true; //Set status to alive
    this -> myWorld = myWorld; //Store a pointer to the StudentWorld
    hitPoints = 0;
}

StudentWorld* Actor::returnPointerToWorld() const //Return the pointer to StudentWorld
{
    return myWorld;
}

void Actor::setAlive(bool life) //Set status dead or alive
{
    alive = life;
}

bool Actor::isAlive() const //Return whether the current Actor is still alive
{
    return alive;
}

void Actor::setHitPoints(int hp) //Initialize an actor with a certain amount of HP
{
    hitPoints = hp;
}

void Actor::decreaseHitPoints(int hp) //Decrease hit points
{
    hitPoints = hitPoints - hp;
}

int Actor::getHitPoints() const //Return amount of hit points
{
    return hitPoints;
}

void Actor::hurtSound() //Play sound when an Actor gets hurt, default (most common) is Salmonella Hurt
{
    returnPointerToWorld() -> playSound(SOUND_SALMONELLA_HURT);
}

void Actor::deathActions() //default is that an Actor does not have any extra actions to perform at its death
{
    return;
}

bool Actor::isDamageable() //Return whether a certain Actor can be damaged outside Socrates
{
    return false;
}

bool Actor::isAttacker() //Return whether a certain Actor can attack the player
{
    return false;
}

bool Actor::isEdible() //Return whether an Actor can be consumed by an Attacker
{
    return false;
}
bool Actor::canDamage() //Return whether an Actor can damage an Attacker
{
    return false;
}
bool Actor::canBlock() //Return whether an Actor can block movement
{
    return false;
}

bool Actor::canOverlap() //Return whether an Actor can be overlapped
{
    return true;
}

void Actor::doSomething() //Default is that an Actor doesn't do anything
{
    return;
}

Actor::~Actor()
{
    
}


//----------ATTACKER
Attacker::Attacker(int imageID, double x, double y, StudentWorld* myWorld)
:Actor(imageID, x, y, myWorld, 90, 0)
{
    movementPlanDistance = 0; //All Attackers start out with no movement plan distance and having eaten nothing
    foodEaten = 0;
}

bool Attacker::isAttacker() //All Attackers are considered enemies
{
    return true;
}

bool Attacker::isDamageable() //All Attackers are damagable by Socrates' damage
{
    return true;
}

void Attacker::doSomethingCommon(StudentWorld* myWorld, double& xSpawn, double& ySpawn, int damage, bool& createNew)
{
    if (myWorld -> overlap(true, getX(), getY())) //Check if it has run into Socrates
    {
        myWorld -> damageSocrates(damage); //If so, damage Socrates
    }
    
    else if (getFood() >= 3) //If the Bacterium has eaten 3 foods
    {
        xSpawn = processCoord(getX()); //Start the creatiion of a new Bacterium
        ySpawn = processCoord(getY());
        createNew = true;
        resetFood();
    }
    
    else if (myWorld -> generalOverlap(*this, getX(), getY())) //If it has run into food, increment the amount of food it has
    {
        addFood();
    }
}

void Attacker::salmonellaMovement(StudentWorld* pointToWorld) //Salmonella have similar movement algorithms
{
    if (getPlanDistance() > 0) //Check if the planned distance is still greater than 0
    {
        decrementPlanDistance(); //Decrease the planned distance
        double proposedX, proposedY;
        getPositionInThisDirection(getDirection(), 3, proposedX, proposedY);
        if (!(pointToWorld -> movementOverlap(proposedX, proposedY))) //Check if moving 3 pixels in current direction will cause movement issues
        {
            moveTo(proposedX, proposedY); //Move to the new location
        }
        
        else
        {
            int direction = randInt(0, 359); //Choose a new random direction
            setDirection(direction);
            resetPlanDistance();
        }
        return;
    }
    
    else
    {
        double x = -1;
        double y = -1;
        pointToWorld -> findClosestFood(getX(), getY(), x, y); //Find the location of the closest food Actor, if available
        if (x == -1 || y == -1)
        {
            int direction = randInt(0, 359); //If no food is found, set bacterium in a random direction
            setDirection(direction);
            resetPlanDistance();
            return;
        }
        
        else
        {
            double proposedX, proposedY;
            double newAngle = (atan((y - getY())/(x - getX())))*180/PI;
            if (x < getX())
            {
                newAngle = newAngle + 180;
            } //Calculate angle to face Food
            getPositionInThisDirection(newAngle, 3, proposedX, proposedY);
            if (!(pointToWorld -> movementOverlap(proposedX, proposedY))) //Check if movement is blocked in this direction
            {
                setDirection(newAngle);
            }
            
            else
            {
                int direction = randInt(0, 359); //If blocked, move in a random direction
                setDirection(direction);
            }
            resetPlanDistance();
            return;
            
        }
    }
}

void Attacker::deathActions() //Default death action is for the most common bacteria, the Salmonella
{
    returnPointerToWorld() -> playSound(SOUND_SALMONELLA_DIE); //Death sound
    int randFoodChance = randInt(1, 2);
    if (randFoodChance == 1) //There is a chance it might turn into food
    {
        returnPointerToWorld() -> createFood(getX(), getY()); //Start the creation of food
    }
}

int Attacker::getFood() const //Return the amount of food a bacterium has eaten
{
    return foodEaten;
}

void Attacker::resetFood() //Reset the amount of foodeaten to 0
{
    foodEaten = 0;
}

void Attacker::addFood() //Incrememnt up the amount of food eaten
{
    foodEaten = foodEaten + 1;
}

int Attacker::getPlanDistance() const //Return how much the Bacteria is planning to move
{
    return movementPlanDistance;
}

void Attacker::resetPlanDistance() //Set the planned distance to max
{
    movementPlanDistance = 10;
}

void Attacker::decrementPlanDistance() //Decrement the planned distance
{
    movementPlanDistance = movementPlanDistance - 1;
}

double Attacker::processCoord(int coord)
{
    if (coord < VIEW_WIDTH/2) //Check if the coord is below or to the left of center
    {
        return (coord + SPRITE_WIDTH/2);
    }
    else if (coord > VIEW_WIDTH/2) //Check if the coord is above or to the right of center
    {
        return (coord - SPRITE_WIDTH/2);
    }
    return coord;
}

Attacker::~Attacker()
{
    
}


//--------REGULAR SALMONELLA
RegularSalmonella::RegularSalmonella(double x, double y, StudentWorld* myWorld)
:Attacker(IID_SALMONELLA, x, y, myWorld)
{
    setHitPoints(4); //Starts out with 4 points of health
}

void RegularSalmonella::doSomething()
{
    if (isAlive()) //Check if it is alive
    {
        StudentWorld* pointToWorld = returnPointerToWorld();
        bool makeNew = false;
        double x = 0;
        double y = 0;
        doSomethingCommon(pointToWorld, x, y, 1, makeNew); //Check for usual overlap and do 1 point of damage when necessary
        if (makeNew == true)
        {
            pointToWorld -> createRegular(x, y); //Create a new Regular Salmonella if told to
        }
        salmonellaMovement(pointToWorld); //Move
    }
}

RegularSalmonella::~RegularSalmonella()
{
    
}



//--------AGGRESSIVE SALMONELLA
AggressiveSalmonella::AggressiveSalmonella(double x, double y, StudentWorld* myWorld)
:Attacker(IID_SALMONELLA, x, y, myWorld)
{
    setHitPoints(10); //Aggressive salmonella start out with 10 hit points
}

void AggressiveSalmonella::doSomething()
{
    if (isAlive()) //Check if it is alive
    {
        StudentWorld* pointToWorld = returnPointerToWorld();
        bool moveOn = false;
        
        if (pointToWorld -> distance(true, getX(), getY()) <= 72) //If its distance to Socrates is less than 72
        {
            double proposedX, proposedY;
            double newAngle = pointToWorld -> angleToSoc(getX(), getY());
            
            getPositionInThisDirection(newAngle, 3, proposedX, proposedY);
            setDirection(newAngle);
            if (pointToWorld -> movementOverlap(proposedX, proposedY)) //Check if movement is blocked here
            {
                 
            }
            
            else
            {
                moveTo(proposedX, proposedY);
            }
            
            moveOn = true;
        }
        
        bool makeNew = false;
        double x = 0;
        double y = 0;
        doSomethingCommon(pointToWorld, x, y, 2, makeNew); //Check for overlap and do 2 points of damage when necessary
        if (makeNew == true)
        {
            pointToWorld -> createAggressive(x, y); //Create a new Aggressive Salmonella
        }
        
        if (moveOn == true)
        {
            return;
        }
        salmonellaMovement(pointToWorld); //Move ONLY if it doesn't chase after Socrates
    }
}

AggressiveSalmonella::~AggressiveSalmonella()
{
    
}



//--------EColi
EColi::EColi(double x, double y, StudentWorld* myWorld)
:Attacker(IID_ECOLI, x, y, myWorld)
{
    setHitPoints(5); //EColi begins with 5 hit points
}

void EColi::hurtSound()
{
    returnPointerToWorld() -> playSound(SOUND_ECOLI_HURT); //Play sound if damaged
}

void EColi::deathActions()
{
    returnPointerToWorld() -> playSound(SOUND_ECOLI_DIE); //Play sound if died
    int randFoodChance = randInt(1, 2);
    if (randFoodChance == 1) //Chance that it turns into a food object
    {
        returnPointerToWorld() -> createFood(getX(), getY());
    }
}

void EColi::doSomething()
{
    if (isAlive()) //Check if it is alive
    {
        StudentWorld* pointToWorld = returnPointerToWorld();
        bool makeNew = false;
        double x = 0;
        double y = 0;
        doSomethingCommon(pointToWorld, x, y, 4, makeNew); //Check for overlap and do 4 points of damage when necessary
        if (makeNew == true)
        {
            pointToWorld -> createEColi(x, y); //Make a new EColi
        }
    
        if (pointToWorld -> distance(true, getX(), getY()) <= 256)
        {
            double direction = pointToWorld -> angleToSoc(getX(), getY()); //Chase after Socrates
            for (int i = 0; i < 10; i++)
            {
                double proposedX = 0;
                double proposedY = 0;
                getPositionInThisDirection(direction, 2, proposedX, proposedY); //Move 2 pixels at a time
                if (!(pointToWorld -> movementOverlap(proposedX, proposedY)))
                {
                    setDirection(direction);
                    moveTo(proposedX, proposedY);
                    return;
                }
                else
                {
                    direction = direction + 10; //Otherwise just try a new direction
                    setDirection(direction);
                }
            }
        }
    }
}

EColi::~EColi()
{

}



//----------GOODIE BASE CLASS
Goodie::Goodie(int imageID, double x, double y, int life, StudentWorld* myWorld)
:Actor(imageID, x, y, myWorld, 0, 1)
{
    lifetime = life; //Set a Goodie's lifetime
    setHitPoints(1); //It is either alive or dead
}

void Goodie::goodieActorsDoSomething(int score, int lifetime)
{
    if (isAlive()) //Check whether the goodie still exists
    {
        StudentWorld* pointerToWorld = returnPointerToWorld();
        if (pointerToWorld -> overlap(true, getX(), getY())) //If Socrates comes in contact with the goodie
        {
            pointerToWorld -> increaseScore(score); //Increase the player's score
            setAlive(false); //destroy the goodie and notify through sound
            static_cast<Goodie*>(this) -> uniqueGoodieAction(); //Perform each goodie's unique action
            return;
        }
        
        if (lifetime <= 0) //If the goodie's lifetime exceeds its preset lifespan, destroy it
        {
            setAlive(false);
        }
        decreaseLife(); //Tell program that the Goodie has lived another lifetime "point" longer
    }
}

int Goodie::returnLifeTime() const //Return the lifetime it has left
{
    return lifetime;
}

void Goodie::decreaseLife() //Decrement lifetime
{
    lifetime = lifetime - 1;
}


bool Goodie::isDamageable() //All Goodies are damagable
{
    return true;
}

Goodie::~Goodie()
{
    
}

//-----------RESTORE HEALTH GOODIE
RestoreHealthGoodie::RestoreHealthGoodie(double x, double y, int life, StudentWorld* myWorld)
:Goodie(IID_RESTORE_HEALTH_GOODIE, x, y, life, myWorld)
{
}

void RestoreHealthGoodie::doSomething()
{
    goodieActorsDoSomething(250, returnLifeTime()); //Awards 250 points if picked up
}

void RestoreHealthGoodie::uniqueGoodieAction()
{
    returnPointerToWorld() -> playSound(SOUND_GOT_GOODIE);
    returnPointerToWorld() -> restoreHealth(); //Restores player back to full health
}

RestoreHealthGoodie::~RestoreHealthGoodie()
{
    
}

//-----------FLAME THROWER GOODIE
FlameThrowerGoodie::FlameThrowerGoodie(double x, double y, int life, StudentWorld* myWorld)
:Goodie(IID_FLAME_THROWER_GOODIE, x, y, life, myWorld)
{
}

void FlameThrowerGoodie::doSomething()
{
    goodieActorsDoSomething(300, returnLifeTime()); //Flame Goodies award 300 points
}

void FlameThrowerGoodie::uniqueGoodieAction()
{
    returnPointerToWorld() -> playSound(SOUND_GOT_GOODIE);
    returnPointerToWorld() -> addFlames(); //Flamethrower goodies award extra flames
}

FlameThrowerGoodie::~FlameThrowerGoodie()
{
    
}


//-----------EXTRA LIFE GOODIE
ExtraLifeGoodie::ExtraLifeGoodie(double x, double y, int life, StudentWorld* myWorld)
:Goodie(IID_EXTRA_LIFE_GOODIE, x, y, life, myWorld)
{
}

void ExtraLifeGoodie::doSomething()
{
    goodieActorsDoSomething(500, returnLifeTime()); //Extra Life Goodies award 500 points
}

void ExtraLifeGoodie::uniqueGoodieAction()
{
    returnPointerToWorld() -> playSound(SOUND_GOT_GOODIE);
    returnPointerToWorld() -> incLives(); //Increase the number of lives you have
}

ExtraLifeGoodie::~ExtraLifeGoodie()
{
    
}


//--------------FUNGUS
Fungus::Fungus(double x, double y, int life, StudentWorld* myWorld)
:Goodie(IID_FUNGUS, x, y, life, myWorld)
{
}

void Fungus::doSomething()
{
    goodieActorsDoSomething(-50, returnLifeTime()); //Fungus decrease player points by 50
}

void Fungus::uniqueGoodieAction()
{
    returnPointerToWorld() -> damageSocrates(20); //Damage Socrates with 20 hit points
}

Fungus::~Fungus()
{
    
}



//------SOCRATES
Socrates::Socrates(StudentWorld* myWorld)
:Actor(IID_PLAYER,0, VIEW_HEIGHT/2, myWorld, 0, 0)
{
    flameStock = 5; //Start out with 5 flame stock
    disinfectantStock = 20; //Start out with 20 disinfectant sprays
    setHitPoints(100); //Socrates has 100 HP
}

void Socrates::addFlames(int amount) //add flames to Socrates' stock
{
    flameStock = flameStock + amount;
}

void Socrates::doSomething()
{
    if (isAlive()) //IF Socrates is alive
    {
        int keyPress;
        if(returnPointerToWorld() -> getKey(keyPress)) //Get the key that was pressed
        {
            switch(keyPress)
            {
                case KEY_PRESS_SPACE:
                    if (disinfectantStock >= 1) //If player presses space, notify StudentWorld of the action
                    {
                        returnPointerToWorld() -> activateSpray();
                        disinfectantStock = disinfectantStock - 1; //Reduce disinfectant Stock
                        returnPointerToWorld() -> playSound(SOUND_PLAYER_SPRAY);
                    }
                    break;
                case KEY_PRESS_ENTER: //If player presses Enter, notify StudentWorld
                    if (flameStock >= 1)
                    {
                        returnPointerToWorld() -> activateFlames();
                        flameStock = flameStock - 1; //Reduce flame stock
                        returnPointerToWorld() -> playSound(SOUND_PLAYER_FIRE); //play fire sound
                    }
                    break;
                case KEY_PRESS_RIGHT:
                    setDirection(getDirection() - 5); //Move 5 degrees
                    moveTo(moveXCircle(), moveYCircle());
                    break;
                case KEY_PRESS_LEFT:
                    setDirection(getDirection() + 5); //Move 5 degrees in opposite direction
                    moveTo(moveXCircle(), moveYCircle());
                    break;
            }
        }
        else if (disinfectantStock != 20) //Keep adding disinfectant sprays to Socrates
        {
            disinfectantStock++;
        }
    }
    else
    {
        return;
    }
}

double Socrates::moveXCircle() const //Find the new X value around the circle
{
    return VIEW_WIDTH/2 + VIEW_RADIUS*cos(PI + (PI*(getDirection()))/180);
}

double Socrates::moveYCircle() const //Find the new Y value around the circle
{
    return VIEW_HEIGHT/2 + VIEW_RADIUS*sin(PI + (PI*(getDirection()))/180);
}

int Socrates::getFlames() const //Return the number of flames Socrates has
{
    return flameStock;
}

int Socrates::getSprays() const //Return the number of sprays Socrates has
{
    return disinfectantStock;
}

Socrates::~Socrates()
{
    
}


//-------DIRT
Dirt::Dirt(double x, double y, StudentWorld* myWorld)
:Actor(IID_DIRT, x, y, myWorld, 0, 1)
{
    setHitPoints(1); //Either dead or alive
}


bool Dirt::isDamageable() //Dirt is damagable
{
    return true;
}

bool Dirt::canBlock() //Can block movement
{
    return true;
}

Dirt::~Dirt()
{
    
}


//------PIT
Pit::Pit(double x, double y, StudentWorld* myWorld)
:Actor(IID_PIT, x, y, myWorld, 0, 1)
{
    regularSal = 5; //Start out with certain amounts of each bacteria
    aggressiveSal = 3;
    eColi = 2;
    totalInventory = 10; //Starts out with 10 total
}

void Pit::doSomething()
{
    if (totalInventory <= 0) //If the Pit still has bacteria
    {
        returnPointerToWorld() -> decreasePitNum(); //Tell StudentWorld that one pit has become empty
        setAlive(false);
    }
    
    if (isAlive() && totalInventory >= 0)
    {
        int chance = randInt(1, 50); //There is a 1 in 50 chance that a bacterium is released
        if (chance == 2)
        {
            bool regsLeft = true;
            bool aggsLeft = true;
            bool coliLeft = true;
            if (regularSal == 0) //Determine which of the three bacterium are still left
            {
                regsLeft = false;
            }
            
            if (aggressiveSal == 0)
            {
                aggsLeft = false;
            }
            
            if (eColi == 0)
            {
                coliLeft = false;
            }
            
            int totalPossible = regsLeft + aggsLeft + coliLeft; //Determine how many types are left
            int chance1 = 0;
            bool done = false;
            while (done == false)
            {
                int chance = randInt(0, totalPossible); //Find a random integer among the number of categories left
                if (chance == 0 && regsLeft) //Release bacteria based on which are left
                {
                    chance1 = chance;
                    done = true;
                }
                else if (chance == 1 && aggsLeft)
                {
                    chance1 = chance;
                    done = true;
                }
                else if (chance == 2 && coliLeft)
                {
                    chance1 = chance;
                    done = true;
                }
            }
            if (chance1 == 0) //Release the type of Attacker according to the the determined chance value
            {
                returnPointerToWorld() -> createRegular(getX(), getY());
                regularSal = regularSal - 1;
            }
            else if (chance1 == 1)
            {
                returnPointerToWorld() -> createAggressive(getX(), getY());
                aggressiveSal = aggressiveSal - 1;
            }
            else if (chance1 == 2)
            {
                returnPointerToWorld() -> createEColi(getX(), getY());
                eColi = eColi - 1;
            }
            totalInventory = totalInventory - 1; //Decrement the inventory
            returnPointerToWorld() -> playSound(SOUND_BACTERIUM_BORN); //Sound that a bacterium has been born
        }
    }
}

bool Pit::canOverlap() //Pits cannot be overlapped
{
    return false;
}

Pit::~Pit()
{
    
}



//------FOOD
Food::Food(double x, double y, StudentWorld* myWorld)
:Actor(IID_FOOD, x, y, myWorld, 90, 1)
{
    setHitPoints(1); //Either alive or dead
}

bool Food::isEdible() //Food is always edible
{
    return true;
}

bool Food::canOverlap() //Food cannot be overlapped
{
    return false;
}

Food::~Food()
{
    
}


//----------DAMAGE - BASE CLASS FOR FLAME AND SPRAY
Damage::Damage(int imageID, double x, double y, double direction, StudentWorld* myWorld)
:Actor(imageID, x, y, myWorld, direction, 1)
{
}

void Damage::damageActorsDoSomething(double degrees, double& travel, int hurt) //Meant for Socrates' forms of damage
{
    if (isAlive()) //Check whether the damage is still available
    {
        if (returnPointerToWorld() -> generalOverlap(*this, getX(), getY(), hurt)) //If it damages something, it's use is up
        {
            setAlive(false);
            return;
        }
        else
        {
            moveAngle(degrees, SPRITE_WIDTH); //Otherwise, move in the direction it is currently going
            travel = travel - SPRITE_WIDTH;
            if (travel <= 0) //If its travel distance exceeds the limit, destroy itself
            {
                setAlive(false);
            }
        }
    }
}

void Damage::setDegreeDirection(double degree) //Set the direction a certain damage actor should face
{
    degreeDirection = degree;
}

void Damage::setTravelDistance(double travel) //Set total travel distance
{
    travelDistance = travel;
}

double Damage::returnDegree() const //Return current facing direction
{
    return degreeDirection;
}

double& Damage::returnTravel() //Modify the travel distance inserted
{
    return travelDistance;
}

bool Damage::canDamage() //All damage actors can damage
{
    return true;
}

Damage::~Damage()
{
    
}

//----------FLAME
Flame::Flame(double x, double y, double direction, StudentWorld* myWorld)
:Damage(IID_FLAME, x, y, direction, myWorld)
{
    setTravelDistance(32); //Flames travel 32 pixels
    setDegreeDirection(direction);
}

void Flame::doSomething()
{
    damageActorsDoSomething(returnDegree(), returnTravel(), 5); //Flames do 5 damage
}

Flame::~Flame()
{
    
}

//------------SPRAY
Spray::Spray(double x, double y, double direction, StudentWorld* myWorld)
:Damage(IID_SPRAY, x, y, direction, myWorld)
{
    setTravelDistance(112); //Sprays can travel 112 pixels
    setDegreeDirection(direction);
}

void Spray::doSomething()
{
    damageActorsDoSomething(returnDegree(), returnTravel(), 2); //Sprays do 2 damage
}

Spray::~Spray()
{
    
}
