//
//  Game.h
//  CS32 - Programming Assignment 1
//
//  Created by Zachary Chang on 1/7/20.
//  Copyright © 2020 Zachary Chang. All rights reserved.
//

#ifndef Game_h
#define Game_h
#include <string>
class Arena;

class Game
{
  public:
      // Constructor/destructor
    Game(int rows, int cols, int nVampires);
    ~Game();

      // Mutators
    void play();

  private:
    Arena* m_arena;
    bool historyDisplay;
      // Helper functions
    std::string takePlayerTurn();
};

#endif /* Game_h */
