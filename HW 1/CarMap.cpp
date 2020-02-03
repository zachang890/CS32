////
////  CarMap.cpp
////  CS32 Homework 1 Zachary Chang
////
////  Created by Zachary Chang on 1/16/20.
////  Copyright © 2020 Zachary Chang. All rights reserved.
////
//
//#include <stdio.h>
//#include "CarMap.h"
//#include "Map.h"
//
//CarMap::CarMap()
//{
//    
//}
//bool CarMap::addCar(std::string license)
//{
//    if (m_cars.insert(license, 0))
//    {
//        return true;
//    }
//    return false;
//}
//
//double CarMap::miles(std::string license) const
//{
//    double milesDriven = -1;
//    m_cars.get(license, milesDriven);
//    return milesDriven;
//}
//
//bool CarMap::drive(std::string license, double distance)
//{
//    if (m_cars.contains(license) == false || distance < 0)
//    {
//        return false;
//    }
//    double milesDriven = 0;
//    m_cars.get(license, milesDriven);
//    m_cars.update(license, milesDriven + distance);
//    return true;
//}
//
//int CarMap::fleetSize() const
//{
//    return m_cars.size();
//}
//
//void CarMap::print() const
//{
//    KeyType keyTemp;
//    ValueType valTemp;
//    for (int i = 0; i < m_cars.size(); i++)
//    { //DO WE NEED TO AVOID THE EXTRANEOUS LINE AT THE END
//        m_cars.get(i, keyTemp, valTemp);
//        std::cout << keyTemp << " " << valTemp << std::endl;
//    }
//}
//
//
