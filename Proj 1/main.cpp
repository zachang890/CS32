// vampires.cpp
#include "Game.h"
#include "Arena.h"
#include "Vampire.h"
#include "Player.h"
#include "History.h" //ADDED
#include "globals.h" //ADDED

int main()
{
      // Create a game
      // Use this instead to create a mini-game:   Game g(3, 5, 2);
    Game g(10, 12, 40);
      // Play the game
    g.play();
}


