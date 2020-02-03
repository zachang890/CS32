//
//  Vampire.cpp
//  CS32 - Programming Assignment 1
//
//  Created by Zachary Chang on 1/7/20.
//  Copyright © 2020 Zachary Chang. All rights reserved.
//

#include "Vampire.h"
#include "Arena.h"
#include "globals.h" //globals
#include <iostream>
using namespace std;

Vampire::Vampire(Arena* ap, int r, int c)
:recordedOnce(false)
{
    if (ap == nullptr)
    {
        cout << "***** A vampire must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "***** Vampire created with invalid coordinates (" << r << ","
             << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_health = INITIAL_VAMPIRE_HEALTH;
    m_idleTurnsRemaining = 0;
}

int Vampire::row() const
{
    return m_row;
}

int Vampire::col() const
{
    return m_col;
}

bool Vampire::isDead() const
{
    return m_health == 0;
}

void Vampire::move()
{
    if (m_idleTurnsRemaining > 0)
    {
        m_arena -> history().record(this -> row(), this -> col());
        m_idleTurnsRemaining--;
        recordedOnce = true;
        return;
    }
    
    if (recordedOnce == true)
    {
        m_arena -> history().record(this -> row(), this -> col());
    }
    recordedOnce = false;
      // Attempt to move in a random direction; if we can't move, don't move
    if (attemptMove(*m_arena, randInt(0, NUMDIRS-1), m_row, m_col))
    {
        if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
        {
            m_arena->setCellStatus(m_row, m_col, EMPTY);
            m_health--;
        }
    }

    if (m_health < INITIAL_VAMPIRE_HEALTH)
        m_idleTurnsRemaining = POISONED_IDLE_TIME;
}
