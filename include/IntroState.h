#ifndef IntroState_H
#define IntroState_H

#include <Ogre.h>
#include <OIS/OIS.h>

#include "GameState.h"

class IntroState : public Ogre::Singleton<IntroState>, public GameState
{
 public:
  IntroState() {}

  void enter ();
  void exit ();
  void pause ();
  void resume ();

  void keyPressed (const OIS::KeyEvent &e);
  void keyReleased (const OIS::KeyEvent &e);

  void mouseMoved (const OIS::MouseEvent &e);
  void mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id);
  void mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id);

  bool frameStarted (const Ogre::FrameEvent& evt);
  bool frameEnded (const Ogre::FrameEvent& evt);

  void createMenu();

  // Heredados de Ogre::Singleton.
  static IntroState& getSingleton ();
  static IntroState* getSingletonPtr ();

 protected:
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneManager;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;
  Ogre::SceneNode* _menuSceneNode;
  bool _exitGame;

  Ogre::RenderWindow* _win;
  Ogre::Ray setRayQuery(int posx, int posy);
  Ogre::RaySceneQuery *_raySceneQuery;
  
  Ogre::Entity* _playEnt;
  Ogre::Entity* _creditsEnt;
  Ogre::Entity* _quitEnt;
};

#endif
