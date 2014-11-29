#ifndef EndState_H
#define EndState_H

#include <Ogre.h>
#include <OIS/OIS.h>

#include "GameState.h"

class EndState : public Ogre::Singleton<EndState>, public GameState
{
 public:
  EndState() {}

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

  void setMessage(std::string message);

  // Heredados de Ogre::Singleton.
  static EndState& getSingleton ();
  static EndState* getSingletonPtr ();

 protected:
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneManager;
  Ogre::SceneNode* _sceneNode;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;

  bool _exitGame;
  std::string _message;
};

#endif
