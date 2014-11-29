#ifndef LevelState_H
#define LevelState_H

#include <Ogre.h>
#include <OIS/OIS.h>

#include "GameState.h"

class LevelState : public Ogre::Singleton<LevelState>, public GameState
{
 public:
  LevelState() {}

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

  // Heredados de Ogre::Singleton.
  static LevelState& getSingleton ();
  static LevelState* getSingletonPtr ();

 protected:
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneManager;
  Ogre::SceneNode* _sceneNode;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;
  Ogre::RenderWindow* _win;

  Ogre::Entity *_veryEasyEnt, *_easyEnt, *_mediumEnt;

  void chooseLevel();
  bool _exitGame;

  Ogre::RaySceneQuery *_raySceneQuery;
  Ogre::Ray setRayQuery(int posx, int posy);
};

#endif
