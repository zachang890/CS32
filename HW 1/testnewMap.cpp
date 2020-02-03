////
////  testnewMap.cpp
////  CS32 Homework 1 Zachary Chang
////
////  Created by Zachary Chang on 1/16/20.
////  Copyright © 2020 Zachary Chang. All rights reserved.
////
//
//#include "newMap.h"
//#include <iostream>
//#include <cassert>
//using namespace std; //MAKE SURE TO UNCOMMENT THESE BRO
//
//int main()
//{
//    Map m;
//    assert(m.empty()); //TEST EMPTY FUNCTION
//    m.insert("", 30); //ENSURE INSERTION OF EMPTY STRING IS FINE
//    assert(!m.empty());
//
//    m.insert("Carl", 50.2); //ENSURE NO DUPLICATE ENTRIES
//    assert(m.size() == 2);
//    assert(!m.insert("", 23.2));
//    assert(!m.insert("Carl", 29.3));
//
//    m.insert("Joe", 24.3); //MAKE SURE TO NOT DOUBLE COUNT
//    assert(m.size() == 3);
//
//    assert(m.update("", 3.2)); //ENSURE THAT VALUES ARE UPDATED APPROPRIATELY
//    double val = 0;
//    assert(m.get("", val) && val == 3.2);
//    assert((!m.update("Bob", val)) && val == 3.2);
//
//    assert(m.insertOrUpdate("Marge", 1.89) && m.size() == 4); //ENSURE THAT IT INSERTS
//    assert(m.insertOrUpdate("Joe", 33.1) && m.size() == 4 && m.get("Joe", val) && val == 33.1); //ENSURE THAT IT UPDATES
//
//    m.insert("Francis", 88.2);
//    m.insert("Will", 57.1);
//
//    assert(m.erase("Carl") && m.size() == 5); //MAKE SURE THAT A KEY/VALUE PAIR IS ERASED PROPERLY
//    assert((!m.erase("Bill")) && m.size() == 5);
//
//    assert(m.contains(""));
//    assert(m.contains("Marge"));
//    assert(!m.contains("Lucky"));
//
//    assert(m.get("Joe", val) && val == 33.1);
//    assert(!m.get("Bill", val));
//
//    string all;
//    double total = 0;
//
//    for (int n = 0; n < m.size(); n++)
//    {
//        string k;
//        double v;
//        m.get(n, k, v);
//        all += k;
//        total += v;
//    }
//    assert(all == "JoeMargeFrancisWill" && total == 183.49);
//
//    Map m1;
//    string arr[] = {"Jake", "Will", "Mike", "Jill", "Alvin", "Quinn"};
//    double arr1[] = {32, 41.1, 66.1, 89.7, 100.2, 33.3};
//
//    for (int i = 0; i < 6; i++)
//    {
//        m1.insert(arr[i], arr1[i]);
//    }
//
//    m.swap(m1);
//    assert(m.size() == 6 && m1.size() == 5);
//
//    cout << "YOU PASSED ALL NEW MAP TESTS!" << endl;
//}
//
