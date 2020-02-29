#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;
class GameWorld;

const double PI = 3.14159265359;
 //maybe make some functions const?


//----------ACTOR BASE CLASS
class Actor: public GraphObject
{
public:
    Actor(int imageID, double startX, double startY, StudentWorld* myWorld, int startDirection = 0, int depth = 0);
    
    StudentWorld* returnPointerToWorld() const;
    void setAlive(bool life);
    bool isAlive() const;
    void setHitPoints(int hp);
    void decreaseHitPoints(int hp);
    int getHitPoints() const;
    virtual void hurtSound();
    virtual void deathActions();
    
    virtual bool isDamageable();
    virtual bool isAttacker();
    virtual bool isEdible();
    virtual bool canDamage();
    virtual bool canBlock();
    virtual bool canOverlap();

    virtual void doSomething();
    virtual ~Actor();
    
private:
    bool alive;
    StudentWorld* myWorld;
    int hitPoints;
};


//--------Attacker
class Attacker: public Actor
{
public:
    Attacker(int imageID, double x, double y, StudentWorld* myWorld);
    
    virtual bool isAttacker();
    virtual bool isDamageable();
    void doSomethingCommon(StudentWorld* myWorld, double& xSpawn, double& ySpawn, int damage, bool& createNew);
    void salmonellaMovement(StudentWorld* pointToWorld);
    virtual void deathActions();
    virtual void doSomething() = 0;
    
    virtual ~Attacker();
private:
    
    int getFood() const;
    void resetFood();
    void addFood();
    int getPlanDistance() const;
    void resetPlanDistance();
    void decrementPlanDistance();
    double processCoord(int coord);
    
    int movementPlanDistance;
    int foodEaten;
    bool timetoDivide;
};


//--------REGULAR SALMONELLA
class RegularSalmonella: public Attacker
{
public:
    RegularSalmonella(double x, double y, StudentWorld* myWorld);
    virtual void doSomething();
    
    virtual ~RegularSalmonella();
};




//--------AGGRESSIVE SALMONELLA
class AggressiveSalmonella: public Attacker
{
public:
    AggressiveSalmonella(double x, double y, StudentWorld* myWorld);
    virtual void doSomething();
    
    virtual ~AggressiveSalmonella();
};


//--------EColi
class EColi: public Attacker
{
public:
    EColi(double x, double y, StudentWorld* myWorld);
    virtual void hurtSound();
    virtual void deathActions();
    virtual void doSomething();
    
    virtual ~EColi();
};



//-------GOODIE
class Goodie: public Actor
{
public:
    Goodie(int imageID, double x, double y, int life, StudentWorld* myWorld);
    
    void goodieActorsDoSomething(int score, int lifetime);
    int returnLifeTime() const;
    void decreaseLife();
    virtual bool isDamageable();
    virtual void uniqueGoodieAction() = 0;
    virtual void doSomething() = 0;
    
    virtual ~Goodie();
private:
    int lifetime;
};


//---------------RESTORE HEALTH GOODIE
class RestoreHealthGoodie: public Goodie
{
public:
    RestoreHealthGoodie(double x, double y, int life, StudentWorld* myWorld);
    virtual void doSomething();
    virtual void uniqueGoodieAction();
    
    virtual ~RestoreHealthGoodie();
};


//------------FLAME THROWER GOODIE
class FlameThrowerGoodie: public Goodie
{
public:
    FlameThrowerGoodie(double x, double y, int life, StudentWorld* myWorld);
    virtual void doSomething();
    virtual void uniqueGoodieAction();
    
    virtual ~FlameThrowerGoodie();
};


//------------EXTRA LIFE GOODIE
class ExtraLifeGoodie: public Goodie
{
public:
    ExtraLifeGoodie(double x, double y, int life, StudentWorld* myWorld);
    virtual void doSomething();
    virtual void uniqueGoodieAction();
    
    virtual ~ExtraLifeGoodie();
};


//------------FUNGUS
class Fungus: public Goodie
{
public:
    Fungus(double x, double y, int life, StudentWorld* myWorld);
    virtual void doSomething();
    virtual void uniqueGoodieAction();
    
    virtual ~Fungus();
};


//------------SOCRATES
class Socrates: public Actor
{
public:
    Socrates(StudentWorld* myWorld);
    void addFlames(int amount);
    virtual void doSomething();
    
    int getSprays() const;
    int getFlames() const;
    
    virtual ~Socrates();
private:
    double moveXCircle() const;
    double moveYCircle() const;
    
    int flameStock;
    int disinfectantStock;
};


//------------DIRT
class Dirt: public Actor
{
public:
    Dirt(double x, double y, StudentWorld* myWorld);
    virtual bool isDamageable();
    virtual bool canBlock();

    virtual ~Dirt();
};


//------------PIT
class Pit: public Actor
{
public:
    Pit(double x, double y, StudentWorld* myWorld);
    virtual void doSomething();
    virtual bool canOverlap();
    
    virtual ~Pit();
private:
    int regularSal;
    int aggressiveSal;
    int eColi;
    int totalInventory;
};


//------------FOOD
class Food: public Actor
{
public:
    Food(double x, double y, StudentWorld* myWorld);
    virtual bool isEdible();
    virtual bool canOverlap();
    virtual ~Food();
};


//------------DAMAGE
class Damage: public Actor //create virtual destructors
{
public:
    Damage(int imageID, double x, double y, double direction, StudentWorld* myWorld);
    void damageActorsDoSomething(double degrees, double& travel, int hurt);
    void setDegreeDirection(double degree);
    void setTravelDistance(double travel);
    double returnDegree() const;
    double& returnTravel();
    
    virtual bool canDamage();
    virtual void doSomething() = 0;
    
    virtual ~Damage();
private:
    double degreeDirection;
    double travelDistance;
};


//------------FLAME
class Flame: public Damage
{ //figure out how to implement no hit point counter
public:
    Flame(double x, double y, double direction, StudentWorld* myWorld);
    virtual void doSomething();
    
    virtual ~Flame();
};


//------------SPRAY
class Spray: public Damage
{ //figure out how to implement no hit point counter
public:
    Spray(double x, double y, double direction, StudentWorld* myWorld);
    virtual void doSomething();
    
    virtual ~Spray();
};
#endif // ACTOR_H_
