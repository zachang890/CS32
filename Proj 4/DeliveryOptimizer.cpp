#include "provided.h"
#include <vector>
#include <random>
#include <algorithm>
using namespace std;

class DeliveryOptimizerImpl
{
public:
    DeliveryOptimizerImpl(const StreetMap* sm);
    ~DeliveryOptimizerImpl();
    void optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const;
private:
    double calculateTotalDistance (const GeoCoord& depot, vector<DeliveryRequest>& deliveries) const
    {
        double totalDistance = 0;
        vector<GeoCoord> actualItinerary;
        for (int i = 0; i < deliveries.size(); i++)
        {
            actualItinerary.push_back(deliveries[i].location);
        }
        actualItinerary.push_back(depot); //Add all the deliveries, treating the depot also like a delivery location
        
        GeoCoord curr = depot;
        for (int i = 0; i < actualItinerary.size(); i++) //guard against not having deliveries at all
        {
            totalDistance = totalDistance + distanceEarthMiles(curr, actualItinerary[i]); //update the total distance travelled
            curr = actualItinerary[i];
        }
        return totalDistance;
    }
};

DeliveryOptimizerImpl::DeliveryOptimizerImpl(const StreetMap* sm)
{
}

DeliveryOptimizerImpl::~DeliveryOptimizerImpl()
{
}

void DeliveryOptimizerImpl::optimizeDeliveryOrder(const GeoCoord& depot, vector<DeliveryRequest>& deliveries, double& oldCrowDistance, double& newCrowDistance) const
{
    GeoCoord curr;
    if (deliveries.size() > 0) //Only if there is more than one delivery will the deliverer move
    {
        curr = deliveries[0].location;
        oldCrowDistance = oldCrowDistance + distanceEarthMiles(depot, curr);
    }
    for (int i = 1; i < deliveries.size(); i++) //Calculate the current crow distance of the delivery order
    {
        oldCrowDistance = oldCrowDistance + distanceEarthMiles(curr, deliveries[i].location);
        curr = deliveries[i].location;
    }
    
    oldCrowDistance = oldCrowDistance + distanceEarthMiles(curr, depot); //distance from last delivery spot back to the depot
    
    //SIMULATED ANNEALING ALGORITHM
    double temp = 100000;
    double coolingR = 0.003;
    
    vector<DeliveryRequest> currentSol; //Keep track of the current solution
    for (int i = 0; i < deliveries.size(); i++)
    {
        currentSol.push_back(DeliveryRequest(deliveries[i].item, deliveries[i].location)); //add each delivery request to the current solution
    }
    
    int size = currentSol.size(); //Shuffle the current solution's size
    for (int i = 0; i < size - 1; i++) {
       int j = i + rand() % (size - i);
       swap(currentSol[i], currentSol[j]);
    }
    
    vector<DeliveryRequest> bestSolution; //Keep track of the best solution

    while (temp > 1)
    {
        vector<DeliveryRequest> newSolution; //Have a temporary vector for the new better solutions
        for (int i = 0; i < currentSol.size(); i++)
        {
            newSolution.push_back(DeliveryRequest(currentSol[i].item, currentSol[i].location)); //The current Solution is the new solution
        }
        
        int delPos1 = rand()%(newSolution.size()); //Get random positions in the vector
        int delPos2 = rand()%(newSolution.size());
        
        while (delPos1 == delPos2)
        {
            delPos2 = rand()%(newSolution.size()); //Make sure the two values are different
        }

        DeliveryRequest first(newSolution[delPos1].item, newSolution[delPos1].location); //Swap the delivery request positions
        DeliveryRequest second(newSolution[delPos2].item, newSolution[delPos2].location);

        newSolution[delPos2] = first;
        newSolution[delPos1] = second;

        int currDist = calculateTotalDistance(depot, currentSol);
        int nextDist = calculateTotalDistance(depot, newSolution);

        double random = (double)rand() / (double)RAND_MAX;

        double returnThis = 0;
        if (nextDist < currDist) //Check which is the better solution
        {
            returnThis = 1.0;
        }

        else
        {
            returnThis = exp((currDist - nextDist)/temp);
        }

        if (returnThis > random)
        {
            currentSol.clear();
            for (int i = 0; i < newSolution.size(); i++)
            {
                currentSol.push_back(DeliveryRequest(newSolution[i].item, newSolution[i].location)); //Add to the solution
            }
        }

        if (bestSolution.size() == 0)
        {
            for (int i = 0; i < currentSol.size(); i++) //Add to the best solution if a there is no solution yet
            {
                bestSolution.push_back(DeliveryRequest(currentSol[i].item, currentSol[i].location));
            }
        }
        else if (calculateTotalDistance(depot, currentSol) < calculateTotalDistance(depot, bestSolution))
        {
            bestSolution.clear();
            for (int i = 0; i < currentSol.size(); i++) //Replace the old best solution if a better solution is found
            {
                bestSolution.push_back(DeliveryRequest(currentSol[i].item, currentSol[i].location));
            }
        }
        temp *= 1 - coolingR;
    }
    for (int i = 0; i < bestSolution.size(); i++)
    {
        DeliveryRequest temp (bestSolution[i].item, bestSolution[i].location);
        deliveries[i] = temp; //Replace deliveries with best solution
    }

    GeoCoord current;
    if (deliveries.size() > 0)
    {
        current = deliveries[0].location; //Calculate the new crow distance
        newCrowDistance = newCrowDistance + distanceEarthMiles(depot, current);
    }
    for (int i = 1; i < deliveries.size(); i++)
    {
        newCrowDistance = newCrowDistance + distanceEarthMiles(current, deliveries[i].location);
        current = deliveries[i].location;
    }
    newCrowDistance = newCrowDistance + distanceEarthMiles(curr, depot);
}

//******************** DeliveryOptimizer functions ****************************

// These functions simply delegate to DeliveryOptimizerImpl's functions.
// You probably don't want to change any of this code.

DeliveryOptimizer::DeliveryOptimizer(const StreetMap* sm)
{
    m_impl = new DeliveryOptimizerImpl(sm);
}

DeliveryOptimizer::~DeliveryOptimizer()
{
    delete m_impl;
}

void DeliveryOptimizer::optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const
{
    return m_impl->optimizeDeliveryOrder(depot, deliveries, oldCrowDistance, newCrowDistance);
}
