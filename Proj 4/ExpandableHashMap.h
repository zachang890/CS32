//
//  ExpandableHashMap.h
//  CS 32 Project 4 - Zachary Chang
//
//  Created by Zachary Chang on 3/4/20.
//  Copyright © 2020 Zachary Chang. All rights reserved.
//

#include <vector>
#include <list>
#include <iostream>

// ExpandableHashMap.h

// Skeleton for the ExpandableHashMap class template.  You must implement the first six
// member functions.

template<typename KeyType, typename ValueType>
class ExpandableHashMap
{
public:
    ExpandableHashMap(double maximumLoadFactor = 0.5);
    ~ExpandableHashMap();
    void reset();
    int size() const;
    void associate(const KeyType& key, const ValueType& value);

      // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;

      // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
    }

      // C++11 syntax for preventing copying and assignment
    ExpandableHashMap(const ExpandableHashMap&) = delete;
    ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;
//pointer to array of buckets that have KeyType (geocoord) and ValType (StreetSegment)
private:
    struct Node
    {
        Node(const KeyType& k, const ValueType& v) //Every node can hold a key type and value type
        {
            keyNode = k;
            valNode = v;
        }
        KeyType keyNode;
        ValueType valNode;
    };
    std::vector<std::list<Node>> hashMap; //The hash map is implemented using a vector of lists filled with nodes
    double maxLoadFact;
    int currentFill;
    unsigned int bucketNum(const KeyType& k) const
    {
        unsigned int hasher(const KeyType& key); //Define the hasher function
        unsigned int hashed = hasher(k);
        return hashed % hashMap.size(); //Hash and modulo to assign a spot in the vector
    }
};

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor)
:hashMap(8), currentFill(0)
{
    if (maximumLoadFactor <= 0) //Check if the load factor is zero or negative
    {
        maxLoadFact = 0.5;
    }
    else
    {
        maxLoadFact = maximumLoadFactor; //Or else set the actual load factor
    }
}

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap()
{
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset()
{
    hashMap.clear(); //Reset the entire hash map (empty)
    currentFill = 0; //Reset the number of associations
    hashMap.resize(8); //Set the size back to 8
}

template<typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType, ValueType>::size() const
{
    return currentFill;
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    ValueType* valTemp = find(key);
    if (valTemp != nullptr)
    {
        *valTemp = value;
        return;
    }
    
    unsigned bucketNumber = bucketNum(key); //Get the bucket the key should associate into
    hashMap[bucketNumber].push_back(Node(key, value)); //Add a Node with the key-val pair into the hashmap
    currentFill++;
    
    if ((static_cast<double>(currentFill) / hashMap.size()) > maxLoadFact) //If the fill has gone over the limit
    {
        std::vector<std::list<Node>> tempMap(hashMap.size() * 2); //Make a larger hashmap
        std::swap(tempMap, hashMap); //Swap the two maps
        for (int i = 0; i < tempMap.size(); i++)
        {
            typename std::list<Node>::iterator it = tempMap[i].begin();
            while (it != tempMap[i].end())
            {
                hashMap[bucketNum(it -> keyNode)].push_back(Node(it -> keyNode, it ->valNode)); //Rehash each of the pairings
                it++;
            }
        }
    }
}

template<typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const
{
    unsigned bucketNumber = bucketNum(key); //Find the bucket number the key would be in
    
    typename std::list<Node>::const_iterator it = hashMap[bucketNumber].begin();
    while (it != hashMap[bucketNumber].end())
    {
        if ((*it).keyNode == key) //Check if the key matches up
        {
            return &(*it).valNode; //Return a reference to its value
        }
        it++;
    }
    return nullptr; //Or else just return nullptr
}
