////
////  testCarMap.cpp
////  CS32 Homework 1 Zachary Chang
////
////  Created by Zachary Chang on 1/16/20.
////  Copyright © 2020 Zachary Chang. All rights reserved.
////
//
//#include <stdio.h>
//#include "CarMap.h"
//#include <iostream>
//#include <cassert>
//using namespace std;
//
//int main()
//{
//    CarMap cars;
//    assert(cars.addCar("JACK")); //ENSURE THAT CARS ARE ADDED PROPERLY
//    assert(cars.addCar("JILL"));
//    assert(cars.addCar("MARK"));
//    assert(!cars.addCar("JACK"));
//    
//    assert(cars.miles("JILL") == 0 && cars.miles("WILL") == -1);
//    
//    assert(cars.drive("MARK", 40));
//    assert(!cars.drive("MIKE", 30));
//    assert(cars.drive("MARK", 20.3) && cars.miles("MARK") == 60.3);
//    assert(!cars.drive("JILL", -3));
//    assert(!cars.drive("MICHAEL", -20));
//    
//    assert(cars.fleetSize() == 3);
//    cout << "ALL TESTS PASSED" << endl;
//    cars.print();
//}
//
//
