#include "provided.h"
#include "ExpandableHashMap.h"
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <fstream>
using namespace std;

unsigned int hasher(const GeoCoord& g)
{
    return std::hash<string>()(g.latitudeText + g.longitudeText);
}

class StreetMapImpl
{
public:
    StreetMapImpl();
    ~StreetMapImpl();
    bool load(string mapFile);
    bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;
private:
    ExpandableHashMap<GeoCoord, std::vector<StreetSegment>>* map; //Implemented using an expandable hash map
};

StreetMapImpl::StreetMapImpl()
{
    map = new ExpandableHashMap<GeoCoord, std::vector<StreetSegment>>; //Allocate this new hash map
}

StreetMapImpl::~StreetMapImpl()
{
    delete map;
}

bool StreetMapImpl::load(string mapFile)
{
    ifstream infile(mapFile);    // infile is a name of our choosing CHECK THAT (FIGURE THIS OUT
    if (!infile)                // Did opening the file fail?
    {
        return false;
    }
    while (infile) //While there is still more to be read
    {
        string street; //Get the street name
        getline(infile, street);
        
        if (street == "") //If a blank line it hit, then just continue
        {
            continue;
        }
        
        int iter; //Get the number of coords associated with this street
        infile >> iter;
        infile.ignore(10000, '\n');
        
        std::vector<string> coords; //Get each start, end pairing into a vector
        for (int i = 0; i < iter; i++)
        {
            string currCoords;
            getline(infile, currCoords);
            
            coords.push_back(currCoords);
        }
        
        std::vector<string> stringCoords;
        for (int i = 0; i < coords.size(); i++) //For every coords string
        {
            string toExtract = coords[i];
            string curr;
            for (int j = 0; j < toExtract.length(); j++)
            {
                if (toExtract[j] != ' ')
                {
                    curr = curr + toExtract[j]; //Extract the coords individual
                }
                
                else
                {
                    stringCoords.push_back(curr); //Push the coords into a vector
                    curr = "";
                }
                
                if (j == toExtract.length() - 1)
                {
                    stringCoords.push_back(curr);
                }
            }
        }
        
        for (int i = 0; i < stringCoords.size(); i+= 4)
        {
            GeoCoord start(stringCoords[i], stringCoords[i + 1]); //Get the starting and end coords for each seg (+ reverse)
            GeoCoord end(stringCoords[i + 2], stringCoords[i + 3]);
            GeoCoord revStart = end;
            GeoCoord revEnd = start;
            StreetSegment forward(start, end, street); //Create the forward and reverse street segments
            StreetSegment backward(revStart, revEnd, street);
            
            std::vector<StreetSegment>* vec = map -> find(start); //Find whether or not the segment already exists in the hashmap
            if (vec == nullptr)
            {
                std::vector<StreetSegment> temp; //if not, associate a new vector
                temp.push_back(forward);
                map -> associate(start, temp);
            }
            
            else
            {
                (*vec).push_back(forward);
            }
            
            std::vector<StreetSegment>* vec1 = map -> find(end);
            if (vec1 == nullptr)
            {
                std::vector<StreetSegment> temp; //Also put in the reverse
                temp.push_back(backward);
                map -> associate(end, temp);
            }
            
            else
            {
                (*vec1).push_back(backward);
            }
        }
    }
    return true;
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
    std::vector<StreetSegment>* list = map -> find(gc); //Find the vector of StreetSegments mapped to gc;
    if (list == nullptr) //Return false if not found
    {
        return false;
    }
    else //Or set segs to the "found" vector
    {
        segs = *list;
    }
    return true;
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
    m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
    delete m_impl;
}

bool StreetMap::load(string mapFile)
{
    return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
   return m_impl->getSegmentsThatStartWith(gc, segs);
}
