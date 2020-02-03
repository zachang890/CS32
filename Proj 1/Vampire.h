//
//  Vampire.h
//  CS32 - Programming Assignment 1
//
//  Created by Zachary Chang on 1/7/20.
//  Copyright © 2020 Zachary Chang. All rights reserved.
//

#ifndef Vampire_h
#define Vampire_h

class Arena;

class Vampire
{
  public:
      // Constructor
    Vampire(Arena* ap, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    void move();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_health;
    int    m_idleTurnsRemaining;
    bool recordedOnce;
};

#endif /* Vampire_h */
