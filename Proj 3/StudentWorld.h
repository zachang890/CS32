#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h" //added
#include <list> //added
#include <cmath>
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
//make sure that public functions are used outside StudentWorld, or else move to private
class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init(); //Private
    virtual int move();
    virtual void cleanUp();
    
    bool generalOverlap(Actor& source, double x, double y, int hurt = 0);
    double distance(bool soc, double x1, double y1, double x2 = 0, double y2 = 0);
    bool overlap(bool soc, double x1, double y1, double x2 = 0, double y2 = 0);
    bool movementOverlap(double x, double y);
    bool petriOverlap(double x, double y);
    void findClosestFood(double currX, double currY, double& x, double& y);
    
    void createFlame(double xLoc, double yLoc, double direction);
    void createSpray(double xLoc, double yLoc, double direction);
    void createHealthGoodie(double xLoc, double yLoc);
    void createFlameGoodie(double xLoc, double yLoc);
    void createExtraLifeGoodie(double xLoc, double yLoc);
    void createFungus(double xLoc, double yLoc);
    void createRegular(double xLoc, double yLoc);
    void createAggressive(double xLoc, double yLoc);
    void createEColi(double xLoc, double yLoc);
    void createFood(double xLoc, double yLoc);
    
    void damageSocrates(int amount);
    void activateFlames();
    void activateSpray();
    double angleToSoc(double x, double y);
    void restoreHealth();
    void addFlames();
    void decreasePitNum();
    
    virtual ~StudentWorld();
    
private:
    void initHelper(int& x, int& y);
    bool checkInitOverlap(double x, double y);
    bool initDistOverlap(double x1, double y1, double x2, double y2);
    void genRandCircularCoord(double& randX, double& randY);
    
    std::string gameText();
    
    bool flamesActivate;
    bool sprayActivate;
    int pitNum;
    std::list<Actor*> members;
    Actor* player;
};

#endif // STUDENTWORLD_H_
