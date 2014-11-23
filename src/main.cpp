/*
#include "MyApp.h"

int main() {
  MyApp app;
  app.start();

  return 0;
}
*/

#define UNUSED_VARIABLE(x) (void)x

#include "GameManager.h"
//#include "IntroState.h"
#include "PlayState.h"
#include "PauseState.h"

#include <iostream>

using namespace std;

int main () {

  GameManager* game = new GameManager();
  //IntroState* introState = new IntroState();
  PlayState* playState = new PlayState();
  PauseState* pauseState = new PauseState();

  //UNUSED_VARIABLE(introState);
  UNUSED_VARIABLE(playState);
  UNUSED_VARIABLE(pauseState);
    
  try
    {
      // Inicializa el juego y transición al primer estado.
      game->start(PlayState::getSingletonPtr());
    }
  catch (Ogre::Exception& e)
    {
      std::cerr << "Excepción detectada: " << e.getFullDescription();
    }
  
  delete game;
  
  return 0;
}