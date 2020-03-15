#include "provided.h"
#include "ExpandableHashMap.h"
#include <list>
#include <queue>
using namespace std;

class PointToPointRouterImpl
{
public:
    PointToPointRouterImpl(const StreetMap* sm);
    ~PointToPointRouterImpl();
    DeliveryResult generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const;
private:
    struct DistanceNode
    {
        DistanceNode(const GeoCoord& end, double dist, string str) //Have a node that stores distance and the street
        {
            endCoord = end;
            distanceToEnd = dist;
            street = str;
        }
        GeoCoord endCoord;
        double distanceToEnd;
        string street;
    };
    const StreetMap* navMap;
};

PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm)
{
    navMap = sm;
}

PointToPointRouterImpl::~PointToPointRouterImpl()
{
}

DeliveryResult PointToPointRouterImpl::generatePointToPointRoute(const GeoCoord& start, const GeoCoord& end, list<StreetSegment>& route, double& totalDistanceTravelled) const
{
    if (start == end)
    {
        return DELIVERY_SUCCESS;
    }
    route.clear(); //Remove everything in the route
    totalDistanceTravelled = 0;
    queue<GeoCoord> testCoords; //Use a queue to store tested GeoCoords
    ExpandableHashMap<GeoCoord, StreetSegment> locationOfPreviousWayPoint; //Use a hashmap to track where the "traveller" has been

    vector<StreetSegment> startCheck;
    vector<StreetSegment> endCheck;
    if (!(navMap -> getSegmentsThatStartWith(start, startCheck)) || !(navMap -> getSegmentsThatStartWith(end, endCheck)))
    {
        return BAD_COORD; //If no segments start with these coords, signal bad_coord
    }

    bool arrived = false;
    GeoCoord curr = start;
    while (!arrived)
    {
        if (curr == end) //check whether or not you have arrived at the end
        {
            GeoCoord curr1 = end;
            bool done = false;
            while (!done)
            {
                StreetSegment* tempSeg = locationOfPreviousWayPoint.find(curr1);
                if ((*tempSeg).start == start) //If the end has actually been reached
                {
                    done = true;
                }
                route.push_front(*tempSeg); //Find the full route
                totalDistanceTravelled = totalDistanceTravelled + distanceEarthMiles((*tempSeg).start, (*tempSeg).end);
                curr1 = tempSeg -> start; //Add to the distance and then set it to the current GeoCoord
            }
            return DELIVERY_SUCCESS; //Notify a success
        }
        vector<StreetSegment> currSegs;
        vector<double> distances;
        navMap -> getSegmentsThatStartWith(curr, currSegs);
        for (int i = 0; i < currSegs.size(); i++) //Otherwise find the distance of each street segment
        {
            distances.push_back(distanceEarthKM(curr, currSegs[i].end));
        }

        vector<DistanceNode> tempDistances;
        for (int i = 0; i < currSegs.size(); i++)
        {
            tempDistances.push_back(DistanceNode(currSegs[i].end, distances[i], currSegs[i].name)); //Add a new distance node
        }

        while (true)
        {
            if (tempDistances.size() <= 1)
            {
                break;
            }
            bool swapped = false;
            for (int i = 0; i < tempDistances.size() - 1; i++) //Using bubble sort, sort the elements by distance
            {
                if (tempDistances[i].distanceToEnd > tempDistances[i+1].distanceToEnd)
                {
                    DistanceNode temp = tempDistances[i];
                    tempDistances[i] = tempDistances[i+1];
                    tempDistances[i+1] = temp;
                    swapped = true;
                }
            }
            if (swapped == false)
            {
                break;
            }
        }

        for (int i = 0; i < tempDistances.size(); i++)
        {
            if (locationOfPreviousWayPoint.find(tempDistances[i].endCoord) != nullptr)
            {
                continue;
            }
            testCoords.push(tempDistances[i].endCoord); //If any segment is found, record it in the hash map
            locationOfPreviousWayPoint.associate(tempDistances[i].endCoord, StreetSegment(curr, tempDistances[i].endCoord, tempDistances[i].street));
        }

        if (testCoords.empty()) //End if there are no coordinates left to try
        {
            return NO_ROUTE;
        }
        curr = testCoords.front();
        testCoords.pop();
    }
    return NO_ROUTE;

}

//******************** PointToPointRouter functions ***************************

// These functions simply delegate to PointToPointRouterImpl's functions.
// You probably don't want to change any of this code.

PointToPointRouter::PointToPointRouter(const StreetMap* sm)
{
    m_impl = new PointToPointRouterImpl(sm);
}

PointToPointRouter::~PointToPointRouter()
{
    delete m_impl;
}

DeliveryResult PointToPointRouter::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    return m_impl->generatePointToPointRoute(start, end, route, totalDistanceTravelled);
}


