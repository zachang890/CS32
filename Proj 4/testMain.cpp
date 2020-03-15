//
//  testMain.cpp
//  CS 32 Project 4 - Zachary Chang
//
//  Created by Zachary Chang on 3/6/20.
//  Copyright © 2020 Zachary Chang. All rights reserved.
//

#include <iostream>

#include "provided.h"
#include <functional>
using namespace std;

int main()
{
    StreetMap sm; //figure out why you cant just create a new dynamically allocated one
    if (sm.load("mapdata.txt"))
    {

    }

    DeliveryPlanner plan(&sm);
//    GeoCoord begin("34.0645901", "-118.4598992");
//    GeoCoord deliver("34.0614882", "-118.4590867");
//    GeoCoord deliver2("34.0626582", "-118.4600964");
//    DeliveryRequest first("food", deliver);
//    DeliveryRequest second("morefood", deliver2);
//    vector<DeliveryRequest> deliverThese;
//    deliverThese.push_back(first);
//    deliverThese.push_back(second);
    GeoCoord begin("34.0625329", "-118.4470263");
    GeoCoord deliver("34.0712323", "-118.4505969");
    GeoCoord deliver1("34.0687443", "-118.4449195");
    GeoCoord deliver2("34.0685657", "-118.4489289");
    DeliveryRequest first("Chickies", deliver);
    DeliveryRequest second("Tomato", deliver1);
    DeliveryRequest third("Potato", deliver2);
    cout << distanceEarthMiles(begin, deliver) + distanceEarthMiles(deliver, deliver1) + distanceEarthMiles(deliver1, deliver2) + distanceEarthMiles(deliver2, begin) << endl;
    cout << distanceEarthMiles(begin, deliver1) + distanceEarthMiles(deliver1, deliver) + distanceEarthMiles(deliver, deliver2) + distanceEarthMiles(deliver2, begin);
    
//    GeoCoord begin("34.0385500", "-118.4515601");
//    GeoCoord deliver("34.0703060", "-118.4341183");
//    GeoCoord deliver1("34.0447242", "-118.4338545");
//    GeoCoord deliver2("34.0440914", "-118.4674934");
//    GeoCoord deliver3("34.0652517", "-118.4404283");
//    DeliveryRequest first ("Chickies", deliver);
//    DeliveryRequest second ("Salmon", deliver1);
//    DeliveryRequest third ("Beer", deliver2);
//    DeliveryRequest fourth ("Sandwich", deliver3);
    vector<DeliveryRequest> deliverThese;
    deliverThese.push_back(first);
    deliverThese.push_back(second);
    deliverThese.push_back(third);
//    deliverThese.push_back(fourth);

    vector<DeliveryCommand> comm;
    double totalDistance = 0;
    plan.generateDeliveryPlan(begin, deliverThese, comm, totalDistance);

    for (int i = 0; i < comm.size(); i++)
    {
        cout << comm[i].description() << endl;
    }
    //Last step figure out why the StreetMap is not destructed properly
}



