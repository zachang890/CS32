//
//  History.h
//  CS32 - Programming Assignment 1
//
//  Created by Zachary Chang on 1/9/20.
//  Copyright © 2020 Zachary Chang. All rights reserved.
//

#ifndef History_h
#define History_h
#include "globals.h"

class History
{
    int m_rows;
    int m_cols;
    int h_grid[MAXROWS][MAXCOLS];
  public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
};

#endif /* History_h */
