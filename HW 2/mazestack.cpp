//
//  main.cpp
//  CS32 Homework 2 Zachary Chang
//
//  Created by Zachary Chang on 1/30/20.
//  Copyright © 2020 Zachary Chang. All rights reserved.
//

#include <string>
#include <stack>
#include <iostream>
using namespace std;

class Coord
{
  public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
  private:
    int m_r;
    int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    stack<Coord> coordStack;
    Coord start(sr, sc);
    coordStack.push(start);
    maze[sr][sc] = '*';
    while (!coordStack.empty())
    {
        Coord explore = coordStack.top();
        //cout << "{" << explore.r() << ", " << explore.c() << "}" << endl; Make sure this line is commented
        coordStack.pop();
        if (explore.r() == er && explore.c() == ec)
        {
            return true;
        }
        
        if (maze[explore.r()][explore.c()+1] != 'X' && maze[explore.r()][explore.c()+1] != '*') //EAST
        {
            Coord pushCord(explore.r(), explore.c()+1);
            coordStack.push(pushCord);
            maze[explore.r()][explore.c()+1] = '*';
        }
        
        if (maze[explore.r()+1][explore.c()] != 'X' && maze[explore.r()+1][explore.c()] != '*') //SOUTH
        {
            Coord pushCord(explore.r()+1, explore.c());
            coordStack.push(pushCord);
            maze[explore.r()+1][explore.c()] = '*';
        }
        
        if (maze[explore.r()][explore.c()-1] != 'X' && maze[explore.r()][explore.c()-1] != '*') //WEST
        {
            Coord pushCord(explore.r(), explore.c()-1);
            coordStack.push(pushCord);
            maze[explore.r()][explore.c()-1] = '*';
        }
        
        if (maze[explore.r()-1][explore.c()] != 'X' && maze[explore.r()-1][explore.c()] != '*') //NORTH
        {
            Coord pushCord(explore.r()-1, explore.c());
            coordStack.push(pushCord);
            maze[explore.r()-1][explore.c()] = '*';
        }
    }
    return false;
}
