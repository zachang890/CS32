//
//  maze.cpp
//  CS32 Homework 3 Zachary Chang
//
//  Created by Zachary Chang on 2/6/20.
//  Copyright © 2020 Zachary Chang. All rights reserved.
//

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    if (sr == er && sc == ec)
    {
        return true;
    }

    maze[sr][sc] = '*';

    if (maze[sr][sc + 1] != 'X' && maze[sr][sc + 1] != '*') //EAST
    {
        if(pathExists(maze, nRows, nCols, sr, sc + 1, er, ec))
        {
            return true;
        }
    }

    if (maze[sr][sc - 1] != 'X' && maze[sr][sc - 1] != '*') //WEST
    {
        if(pathExists(maze, nRows, nCols, sr, sc - 1, er, ec))
        {
            return true;
        }
    }

    if (maze[sr - 1][sc] != 'X' && maze[sr - 1][sc] != '*') //NORTH
    {
        if(pathExists(maze, nRows, nCols, sr - 1, sc, er, ec))
        {
            return true;
        }
    }

    if (maze[sr + 1][sc] != 'X' && maze[sr + 1][sc] != '*') //SOUTH
    {
        if(pathExists(maze, nRows, nCols, sr + 1, sc, er, ec))
        {
            return true;
        }
    }
    return false;
}
