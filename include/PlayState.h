#ifndef PlayState_H
#define PlayState_H

#include <Ogre.h>
#include <OIS/OIS.h>

#include "GameState.h"
#include "minesweeper.h"


class PlayState : public Ogre::Singleton<PlayState>, public GameState
{
 public:
  PlayState() {}

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

  void createScene();
  void createBoardScene();
  void createGroundScene();
  void actualizeBoard();
  void setLevel(Difficulty level);

  // Heredados de Ogre::Singleton.
  static PlayState& getSingleton ();
  static PlayState* getSingletonPtr ();

 protected:
  Minesweeper _minesweeper;
  Difficulty _level;
  std::vector<std::pair<Ogre::Entity*, Ogre::Entity*>> _entityNodes;
  Ogre::RenderWindow* _win;

  Ogre::Root* _root;
  Ogre::SceneManager* _sceneManager;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;
  Ogre::SceneNode* _sceneLevel;

  bool _exitGame;

  void initializeCamera();
  Ogre::RaySceneQuery *_raySceneQuery;
  Ogre::Ray setRayQuery(int posx, int posy);
};

#endif
