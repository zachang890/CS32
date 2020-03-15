#include "provided.h"
#include "ExpandableHashMap.h" //added?
#include <vector>
using namespace std;

class DeliveryPlannerImpl
{
public:
    DeliveryPlannerImpl(const StreetMap* sm);
    ~DeliveryPlannerImpl();
    DeliveryResult generateDeliveryPlan(
        const GeoCoord& depot,
        const vector<DeliveryRequest>& deliveries,
        vector<DeliveryCommand>& commands,
        double& totalDistanceTravelled) const;
private:
    const StreetMap* navigator;
    PointToPointRouter router;
};

DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm)
:navigator(sm), router(navigator)
{
}

DeliveryPlannerImpl::~DeliveryPlannerImpl()
{ //Don't have to delete navigator since the memory was not allocated as a result of the constructor
}

DeliveryResult DeliveryPlannerImpl::generateDeliveryPlan(const GeoCoord& depot, const vector<DeliveryRequest>& deliveries, vector<DeliveryCommand>& commands, double& totalDistanceTravelled) const
{
    commands.clear(); //Clear the commands vector
    DeliveryOptimizer p(navigator);
    double firstDist = 0;
    double newDist = 0;
    vector<DeliveryRequest> tempDeliver;
    for (int i = 0; i < deliveries.size(); i++)
    {
        tempDeliver.push_back(deliveries[i]); //Add each of the deliveries into a temporary vector
    }
    p.optimizeDeliveryOrder(depot, tempDeliver, firstDist, newDist); //Optimize the order of deliveries
    vector<list<StreetSegment>> totalRoutes;

    //Using the delivery optimizer class, reorder the deliveries to optimize delivery time
    //Does the commands vector have to be cleared before adding to it
    list<StreetSegment> currRoute;
    GeoCoord curr = depot;
    double segDist = 0;
    for (int i = 0; i < tempDeliver.size(); i++)
    {
        router.generatePointToPointRoute(curr, tempDeliver[i].location, currRoute, segDist); //for covering each delivery
        totalDistanceTravelled = totalDistanceTravelled + segDist;
        totalRoutes.push_back(currRoute); //Add the route to the totalRoutes
        currRoute.clear();
        curr = tempDeliver[i].location; //Set the new starting location to the most recent delivery
    }

    router.generatePointToPointRoute(tempDeliver[tempDeliver.size() - 1].location, depot, currRoute, segDist);
    totalDistanceTravelled = totalDistanceTravelled + segDist;
    totalRoutes.push_back(currRoute); //Do the same one last time for the way back to the depot


    for (int i = 0; i < totalRoutes.size(); i++)
    {
        list<StreetSegment> thisRoute = totalRoutes[i];
        list<StreetSegment>::iterator it = thisRoute.begin();
        bool arrived = false;
        string currName = it -> name;
        GeoCoord currPoint;
        while (!arrived)
        {
            double proceedDistance = 0;
            double angleOfProceed = angleOfLine(*it); //Find the direction that the StreetSegment points in
            string direction = "";
            while (it -> name == currName && it != thisRoute.end())
            {
                proceedDistance = proceedDistance + distanceEarthMiles(it -> start, it -> end);
                it++; //Find how long it takes to reach the end of a given road
            }

            if (angleOfProceed >= 0 && angleOfProceed < 22.5) //Use degrees to determine which direction the road points in
                {direction = "east";}
            else if (angleOfProceed >= 22.5 && angleOfProceed < 67.5)
                {direction = "northeast";}
            else if (angleOfProceed >= 67.5 && angleOfProceed < 112.5)
                {direction = "north";}
            else if (angleOfProceed >= 112.5 && angleOfProceed < 157.5)
                {direction = "northwest";}
            else if (angleOfProceed >= 157.5 && angleOfProceed < 202.5)
                {direction = "west";}
            else if (angleOfProceed >= 202.5 && angleOfProceed < 247.5)
                {direction = "southwest";}
            else if (angleOfProceed >= 247.5 && angleOfProceed < 292.5)
                {direction = "south";}
            else if (angleOfProceed >= 292.5 && angleOfProceed < 337.5)
                {direction = "southeast";}
            else if (angleOfProceed >= 337.5)
                {direction = "east";}

            DeliveryCommand straight;
            straight.initAsProceedCommand(direction, currName, proceedDistance); //Cause a proceed command

            commands.push_back(straight); //Push this in the commands
            if (it == thisRoute.end()) //If no more roads follow
            {
                it--;
                currPoint = it -> end;
            }
            else //If more roads have yet to be followed
            {
                currPoint = it -> start;
            }

            if (currPoint == tempDeliver[i].location || currPoint == depot) //Check if the deliverer has arrived somewhere
            {
                if (it -> end == tempDeliver[i].location) //If a delivery point has been reached
                {
                    DeliveryCommand deliver;
                    deliver.initAsDeliverCommand(tempDeliver[i].item); //Execute a delivery command
                    commands.push_back(deliver);
                }
                break;
            }

            it--;
            double turnAngle; //Get the previous street segment and current street Segments
            StreetSegment temp1 = *it;
            it++;
            StreetSegment temp2 = *it;
            turnAngle = angleBetween2Lines(temp1, temp2); //Calculate the angle between them

            currName = it -> name;
            if (turnAngle >= 1 && turnAngle < 180) //Check whether it is a left turn, right turn or still considered straight
                {direction = "left";}
            else if (turnAngle >= 180 && turnAngle <= 359)
                {direction = "right";}
            else if (turnAngle < 1 || turnAngle > 359)
                {continue;} //essentially a straight line

            DeliveryCommand turn; //Potentially execute a turn command
            turn.initAsTurnCommand(direction, it -> name);
            commands.push_back(turn);
        }
    }
    //DONE WITH DELIVERY ROUTES
    return DELIVERY_SUCCESS;
}

//******************** DeliveryPlanner functions ******************************

// These functions simply delegate to DeliveryPlannerImpl's functions.
// You probably don't want to change any of this code.

DeliveryPlanner::DeliveryPlanner(const StreetMap* sm)
{
    m_impl = new DeliveryPlannerImpl(sm);
}

DeliveryPlanner::~DeliveryPlanner()
{
    delete m_impl;
}

DeliveryResult DeliveryPlanner::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    return m_impl->generateDeliveryPlan(depot, deliveries, commands, totalDistanceTravelled);
}
