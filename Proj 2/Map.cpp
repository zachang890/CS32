#include "Map.h"

Map::Map()
 : m_size(0)
{
    Node* dummy = new Node; //INITIALIZE THE NEW MAP WITH A DUMMY NODE WITH THE HEAD POINTED TO IT
    head = dummy;
    dummy -> next = head;
    dummy -> prev = head;
}

Map::Map(const Map &source) //COPY CONSTRUCTOR
{
    m_size = source.m_size; //GIVE THE MAP THE SIZE OF THE SOURCE
    Node* dummy = new Node; //MAKE A DUMMY NODE FOR THE NEW MAP
    head = dummy;
    dummy -> next = head;
    dummy -> prev = head;
    
    Node* sourcePtr = (source.head) -> next;
    Node* targetPtr = head;
    while (sourcePtr != source.head)
    {
        Node* addition = new Node; //CONTINUE TO ADD NODES TO THE ADDITION WITH PAIRS THE MATCH WITH THE SOURCE
        addition -> m_pair = sourcePtr -> m_pair;
        addition -> next = head;
        head -> prev = addition;
        targetPtr -> next = addition;
        addition -> prev = targetPtr;
        targetPtr = targetPtr -> next; //REMEMBER TO SET THE TARGET POINTERS NEW NEXT
        sourcePtr = sourcePtr -> next;
    }
}

Map::~Map()
{
    Node* p = head -> next;
    while (p != head)
    {
        Node* save = p -> next; //DELETE ALL NODES WHILE SAVING THE NEXT ONE IN LINE
        delete p;
        p = save;
    }
    delete p; //FINALLY, DELETE THE DUMMY NODE
}

Map& Map::operator=(const Map &source) //ASSIGNMENT OPERATOR
{
    if (&source == this) //CHECK WHETHER YOU ARE COPYING A MAP TO ITSELF
    {
        return *this;
    }
    
    Node* p = head -> next; //DELETE EXISTING CONTENT IN THE TARGET MAP
    while (p != head)
    {
        Node* save = p -> next;
        delete p;
        p = save;
    }
    delete p;
    
    m_size = source.m_size; //DO THE SAME ACTIONS AS THE COPY CONSTRUCTOR
    Node* dummy = new Node;
    head = dummy;
    dummy -> next = head;
    dummy -> prev = head;
    
    Node* sourcePtr = (source.head) -> next;
    Node* targetPtr = head;
    while (sourcePtr != source.head)
    {
        Node* addition = new Node;
        addition -> m_pair = sourcePtr -> m_pair;
        addition -> next = head;
        head -> prev = addition;
        targetPtr -> next = addition;
        addition -> prev = targetPtr;
        targetPtr = targetPtr -> next;
        sourcePtr = sourcePtr -> next;
    }
    return *this; //RETURN A POINTER TO THE CURRENT MAP
}

bool Map::erase(const KeyType& key)
{
    Node* destroy = find(key); //LOOK FOR AN EXISTING MATCH
    if (destroy == head)
    {
        return false;
    }
    m_size--; //DECREMENT THE SIZE OF THE MAP
    destroy -> prev -> next = destroy -> next; //RELINK THE LINKED LIST
    destroy -> next -> prev = destroy -> prev;
    delete destroy;
    return true;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
    Node* ptr = find(key);
    if (ptr == head) //CHECK THAT NO MATCHING KEY WAS FOUND
    {
        return false;
    }
    value = (ptr -> m_pair).m_value; //SET THE VALUE'S NEW VALUE
    return true;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
    if (i < 0  ||  i >= m_size) //FIRST CHECK IF THE i IS OUT OF BOUNDS
    {
        return false;
    }
    Node* ptr = head -> next;
    for (int j = 0; j < i; j++) //ITERATE UNTIL THE REQUESTED INDEX
    {
        ptr = ptr -> next;
    }
    key = (ptr -> m_pair).m_key; //SET THE NEW KEY'S VALUE
    value = (ptr -> m_pair).m_value; //SET THE NEW VALUE'S VALUE
    return true;
}

void Map::swap(Map& other)
{
    Node* temp = head; //SWITCH POINTERS TO THE HEADS OF THE TWO MAPS
    head = other.head;
    other.head = temp;
    
    int tempSize = m_size;
    m_size = other.m_size;
    other.m_size = tempSize;
}

bool Map::doInsertOrUpdate(const KeyType& key, const ValueType& value, bool mayInsert, bool mayUpdate)
{
    Node* ptr = find(key);
    if (ptr != head) //FIND THE MATCHING KEY
    {
        if (mayUpdate)
            (ptr -> m_pair).m_value = value;
        return mayUpdate;
    }
    if (!mayInsert)  // not found, and not allowed to insert
        return false;
    
    Node* insertNode = new Node; //CREATE A NEW NODE WITH THE INDICATED KEY AND VALUE
    (insertNode -> m_pair).m_key = key;
    (insertNode -> m_pair).m_value = value;
    
    head -> prev -> next = insertNode; //REORGANIZE POINTERS
    insertNode -> prev = head -> prev;
    head -> prev = insertNode;
    insertNode -> next = head;
    
    m_size++; //INCREMENT THE SIZE OF THE MAP
    return true;
}

bool combine(const Map& m1, const Map& m2, Map& result)
{
    //INITIAL CLEANUP, PRESUMABLY ONLY LEAVES A DUMMY NODE
    Map temp1(m1); //AVOID ALIAS ISSUES
    Map temp2(m2);
    
    Map empty;
    result = empty;
    
    KeyType keyCompare1;
    ValueType valCompare1;
    KeyType keyCompare2;
    ValueType valCompare2;
    bool correctMatches = true;
    bool singleInsert = true;
    
    for (int i = 0; i < temp1.size(); i++)
    {
        temp1.get(i, keyCompare1, valCompare1); //GET THE FIRST FOUR VALUES (KEYS AND VALUES) IN THE MAP
        for (int j = 0; j < temp2.size(); j++)
        {
            temp2.get(j, keyCompare2, valCompare2);
            if (keyCompare1 == keyCompare2) //CHECK IF THE KEYS MATCH
            {
                singleInsert = false; //DONT INSERT THEM AGAIN SINCE THEY MATCH
                if (valCompare1 == valCompare2) //CHECK IF THE VALUES MATCH
                {
                    result.insert(keyCompare1, valCompare1); //INSERT IF PERFECT MATCH
                }
                else
                {
                    correctMatches = false; //MAKE SURE TO RETURN FALSE IF THEY DON'T MATCH
                }
            }
        }
        if (singleInsert == true) //INSERT IF THE KEY DOESN'T FIND A MATCH
        {
            result.insert(keyCompare1, valCompare1);
        }
        singleInsert = true;
    }
    
    for (int i = 0; i < temp2.size(); i++) //ITERATE AND INSERT LEFTOVERS IN THE SECOND MAP
    {
        temp2.get(i, keyCompare2, valCompare2);
        if (!result.contains(keyCompare2) && !temp1.contains(keyCompare2))
        {
            result.insert(keyCompare2, valCompare2);
        }
    }
    return correctMatches;
}

void reassign(const Map& m, Map& result)
{
    Map temp(m); //INITIAL CLEAN UP AND AVOID ISSUES WITH ALIASING
    
    Map empty;
    result = empty;
    
    if (temp.size() == 0) //PREVENT REASSIGNMENT WITH AN EMPTY MAP
    {
        return;
    }
    
    if (temp.size() == 1) //RETURN A MAP EQUAL TO m IF ONLY ONE PAIR
    {
        KeyType key1;
        ValueType val1;
        temp.get(0, key1, val1);
        result.insert(key1, val1);
        return;
    }
    
    ValueType saveLastVal;
    KeyType currentKey;
    ValueType currentVal;
    KeyType currentKey2;
    ValueType currentVal2;
    
    temp.get(0, currentKey, saveLastVal);
    
    for (int i = 0; i < temp.size() - 1; i++) //SHIFT EACH VALUE DOWN THE LINKED LIST
    {
        temp.get(i, currentKey, currentVal);
        temp.get(i+1, currentKey2, currentVal2);
        result.insert(currentKey, currentVal2);
    }
    temp.get(temp.size() - 1, currentKey, currentVal); //INSERT THE LAST VALUE TO THE LAST NODE
    result.insert(currentKey, saveLastVal);
}


