#define UNUSED_VARIABLE(x) (void)x

#include "GameManager.h"
#include "IntroState.h"
#include "PlayState.h"
#include "EndState.h"

#include <iostream>

using namespace std;

int main () {

  GameManager* game = new GameManager();
  IntroState* introState = new IntroState();
  PlayState* playState = new PlayState();
  EndState* endState = new EndState();

  UNUSED_VARIABLE(introState);
  UNUSED_VARIABLE(playState);
  UNUSED_VARIABLE(endState);
    
  try
    {
      // Inicializa el juego y transición al primer estado.
      game->start(IntroState::getSingletonPtr());
    }
  catch (Ogre::Exception& e)
    {
      std::cerr << "Excepción detectada: " << e.getFullDescription();
    }
  
  delete game;
  
  return 0;
}