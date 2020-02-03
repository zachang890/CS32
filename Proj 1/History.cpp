//
//  History.cpp
//  CS32 - Programming Assignment 1
//
//  Created by Zachary Chang on 1/9/20.
//  Copyright © 2020 Zachary Chang. All rights reserved.
//

#include "History.h"
#include "globals.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols)
{
    m_rows = nRows;
    m_cols = nCols;
    for (int i = 0; i < MAXROWS; i++)
    {
        for (int j = 0; j < MAXCOLS; j++)
        {
            h_grid[i][j] = 0;
        }
    }
    
}

bool History::record(int r, int c)
{
    if (r > m_rows || c > m_cols)
    {
        return false;
    }
    
    h_grid[r-1][c-1] = h_grid[r-1][c-1] + 1;
    return true;
    
}

void History::display() const
{
    clearScreen();
    char alphabet[] = {'.', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    char d_grid[MAXROWS][MAXCOLS];
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_cols; j++)
        {
            int val = h_grid[i][j];
            if (val >= 26)
            {
                d_grid[i][j] = 'Z';
            }
            else
            {
                d_grid[i][j] = alphabet[val];
            }
        }
    }
    
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_cols; j++)
        {
            cout << d_grid[i][j];
        }
        cout << endl;
    }
    cout << endl;
}


