//
//  Map.cpp
//  CS32 Homework 1 Zachary Chang
//
//  Created by Zachary Chang on 1/15/20.
//  Copyright © 2020 Zachary Chang. All rights reserved.
//

#include <stdio.h>
#include "Map.h"
using namespace std;

Map::Map()
{
    num_Entries = 0;
}

bool Map::empty() const
{
    if (num_Entries == 0)
    {
        return true;
    }
    return false;
};

int Map::size() const
{
    return num_Entries;
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
    if (num_Entries == DEFAULT_MAX_ITEMS)
    {
        return false;
    }

    if (findKey(key) != -1)
    {
        return false;
    }

    m_map[num_Entries].m_key = key;
    m_map[num_Entries].m_value = value;
    num_Entries++;
    return true;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
    int indexVal = findKey(key);
    if (indexVal != -1)
    {
        m_map[indexVal].m_value = value;
        return true;
    }
    return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
    if (update(key, value) == true)
    {
        return true;
    }

    if (insert(key, value) == true)
    {
        return true;
    }

    return false;
}

bool Map::erase(const KeyType& key)
{
    int indexVal = findKey(key);
    if (indexVal != -1)
    {
        for (int i = indexVal; i < num_Entries - 1; i++)
        {
            m_map[i].m_key = m_map[i + 1].m_key;
            m_map[i].m_value = m_map[i + 1].m_value;
        }
        num_Entries = num_Entries - 1;
        return true;
    }
    return false;
}

bool Map::contains(const KeyType& key) const
{
    if (findKey(key) != -1)
    {
        return true;
    }
    return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
    int indexVal = findKey(key);
    if (indexVal != -1)
    {
        value = m_map[indexVal].m_value;
        return true;
    }
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
    if (i >= 0 && i < size())
    {
        key = m_map[i].m_key;
        value = m_map[i].m_value;
        return true;
    }
    return false;
}

void Map::swap(Map& other)
{
    KeyType temp1;
    ValueType temp2;
    int temp3;

    for (int i = 0; i < DEFAULT_MAX_ITEMS; i++)
    {
        temp1 = m_map[i].m_key;
        m_map[i].m_key = other.m_map[i].m_key;
        other.m_map[i].m_key = temp1;

        temp2 = m_map[i].m_value;
        m_map[i].m_value = other.m_map[i].m_value;
        other.m_map[i].m_value = temp2;
    }
    temp3 = num_Entries;
    num_Entries = other.num_Entries;
    other.num_Entries = temp3;
}

