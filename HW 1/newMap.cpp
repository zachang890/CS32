////
////  newMap.cpp
////  CS32 Homework 1 Zachary Chang
////
////  Created by Zachary Chang on 1/16/20.
////  Copyright © 2020 Zachary Chang. All rights reserved.
////
//
//#include <stdio.h>
//#include "newMap.h"
//using namespace std;
//
//Map::Map()
//{
//    m_map = new KeyValEntry[DEFAULT_MAX_ITEMS];
//    capacity = DEFAULT_MAX_ITEMS;
//    num_Entries = 0;
//}
//
//Map::Map(int entries)
//{
//    if (entries < 0)
//    {
//        exit(1);
//    }
//    m_map = new KeyValEntry[entries];
//    capacity = entries;
//    num_Entries = 0;
//}
//
//Map::Map(const Map &source)
//{
//    capacity = source.capacity;
//    num_Entries = source.num_Entries;
//    m_map = new KeyValEntry[capacity];
//    for (int i = 0; i < capacity; i++)
//    {
//        m_map[i] = source.m_map[i];
//    }//ASK WHETHER THE SWAP FUNCTION CAUSES AN INCONSISTENT NUMBER OF STATEMENT RUNS
//}
//
//Map& Map::operator=(const Map &source)
//{
//    if (&source == this)
//    {
//        return *this;
//    }
//    delete [] m_map;
//    capacity = source.capacity;
//    num_Entries = source.num_Entries;
//    m_map = new KeyValEntry[capacity];
//    for (int i = 0; i < capacity; i++)
//    {
//        m_map[i] = source.m_map[i];
//    }
//    return *this;
//}
//
//Map::~Map()
//{
//    delete [] m_map;
//}
//
//bool Map::empty() const
//{
//    if (num_Entries == 0)
//    {
//        return true;
//    }
//    return false;
//};
//
//int Map::size() const
//{
//    return num_Entries;
//}
//
//bool Map::insert(const KeyType& key, const ValueType& value)
//{
//    if (num_Entries == capacity)
//    {
//        return false;
//    }
//    
//    int indexVal = -1;
//    for (int i = 0; i < num_Entries; i++)
//    {
//        if (m_map[i].m_key == key)
//        {
//            indexVal = i;
//            break;
//        }
//    }
//
//    if (indexVal != -1)
//    {
//        return false;
//    }
//    
//    m_map[num_Entries].m_key = key;
//    m_map[num_Entries].m_value = value;
//    num_Entries++;
//    return true;
//}
//
//bool Map::update(const KeyType& key, const ValueType& value)
//{
//    int indexVal = -1;
//    for (int i = 0; i < num_Entries; i++)
//    {
//        if (m_map[i].m_key == key)
//        {
//            indexVal = i;
//            break;
//        }
//    }
//    if (indexVal != -1)
//    {
//        m_map[indexVal].m_value = value;
//        return true;
//    }
//    return false;
//}
//
//bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
//{
//    if (update(key, value) == true)
//    {
//        return true;
//    }
//    
//    if (insert(key, value) == true)
//    {
//        return true;
//    }
//    
//    return false;
//}
//
//bool Map::erase(const KeyType& key)
//{
//    int indexVal = -1;
//    for (int i = 0; i < num_Entries; i++)
//    {
//        if (m_map[i].m_key == key)
//        {
//            indexVal = i;
//            break;
//        }
//    }
//    
//    if (indexVal != -1)
//    {
//        for (int i = indexVal; i < num_Entries - 1; i++)
//        {
//            m_map[i].m_key = m_map[i + 1].m_key;
//            m_map[i].m_value = m_map[i + 1].m_value;
//        }
//        num_Entries = num_Entries - 1;
//        return true;
//    }
//    return false;
//}
//
//bool Map::contains(const KeyType& key) const
//{
//    int indexVal = -1;
//    for (int i = 0; i < num_Entries; i++)
//    {
//        if (m_map[i].m_key == key)
//        {
//            indexVal = i;
//            break;
//        }
//    }
//    
//    if (indexVal != -1)
//    {
//        return true;
//    }
//    return false;
//}
//
//bool Map::get(const KeyType& key, ValueType& value) const
//{
//    int indexVal = -1;
//    for (int i = 0; i < num_Entries; i++)
//    {
//        if (m_map[i].m_key == key)
//        {
//            indexVal = i;
//            break;
//        }
//    }
//    
//    if (indexVal != -1)
//    {
//        value = m_map[indexVal].m_value;
//        return true;
//    }
//    return false;
//}
//
//bool Map::get(int i, KeyType& key, ValueType& value) const
//{
//    if (i >= 0 && i < size())
//    {
//        key = m_map[i].m_key;
//        value = m_map[i].m_value;
//        return true;
//    }
//    return false;
//}
//
//void Map::swap(Map& other)
//{
//    int tempEntries = num_Entries; //SWAP THE NUMBER OF ENTRIES IN EACH AND THEIR CAPACITIES
//    num_Entries = other.num_Entries;
//    other.num_Entries = tempEntries;
//    
//    int tempCapacity = capacity;
//    capacity = other.capacity;
//    other.capacity = tempCapacity;
//    
//    KeyValEntry* mapPtr = other.m_map; //SWAP THE MAPS THAT EACH OBJECT POINTS TO
//    other.m_map = m_map;
//    m_map = mapPtr;
//}
