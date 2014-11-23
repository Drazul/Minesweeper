#include "IntroState.h"
#include "PlayState.h"

template<> IntroState* Ogre::Singleton<IntroState>::msSingleton = 0;

void
IntroState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();

  _sceneManager = _root->createSceneManager(Ogre::ST_GENERIC, "SceneManager");

  _camera = _sceneManager->createCamera("MenuCamera");
  //_viewport->setBackgroundColour(Ogre::ColourValue(1.0, 1.0, 1.0));

  _camera->setPosition(Ogre::Vector3(0.5, 4, 12));
  _camera->setDirection(Ogre::Vector3(0, 0, -1));

  _camera->setNearClipDistance(0.1);
  _camera->setFarClipDistance(100);

  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);

  createMenu();

  _root->startRendering();

  _exitGame = false;
}

void IntroState::createMenu() {
  Ogre::SceneNode* menuSceneNode = _sceneManager->createSceneNode("MenuScene");

  Ogre::SceneNode* wallNode = menuSceneNode->createChildSceneNode("wallNode");
  Ogre::SceneNode* playNode = menuSceneNode->createChildSceneNode("playNode");
  Ogre::SceneNode* moreNode = menuSceneNode->createChildSceneNode("moreNode");
  Ogre::SceneNode* quitNode = menuSceneNode->createChildSceneNode("quitNode");


  Ogre::Entity* wallEnt = _sceneManager->createEntity("wall.mesh");
  Ogre::Entity* playEnt = _sceneManager->createEntity("play.mesh");
  Ogre::Entity* moreEnt = _sceneManager->createEntity("more.mesh");
  Ogre::Entity* quitEnt = _sceneManager->createEntity("quit.mesh");
    
  wallNode->attachObject(wallEnt);
  playNode->attachObject(playEnt);
  moreNode->attachObject(moreEnt);
  quitNode->attachObject(quitEnt);

  wallNode->setPosition(0, 0, 0);
  playNode->setPosition(0, 7, 0);
  moreNode->setPosition(0, 4.5, 0);
  quitNode->setPosition(0, 2, 0);

  playNode->pitch(Ogre::Degree(90), Ogre::Node::TS_LOCAL);
  moreNode->pitch(Ogre::Degree(90), Ogre::Node::TS_LOCAL);
  quitNode->pitch(Ogre::Degree(90), Ogre::Node::TS_LOCAL);

  playNode->yaw(Ogre::Degree(-90), Ogre::Node::TS_LOCAL);
  moreNode->yaw(Ogre::Degree(-90), Ogre::Node::TS_LOCAL);
  quitNode->yaw(Ogre::Degree(-90), Ogre::Node::TS_LOCAL);


  _sceneManager->getRootSceneNode()->addChild(menuSceneNode);

//camara position: Vector3(0.5, 4, 12)
//camara direction: Vector3(0, 0, -1)
}

void
IntroState::exit()
{
  _sceneManager->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
}

void
IntroState::pause ()
{
}

void
IntroState::resume ()
{
}

bool
IntroState::frameStarted
(const Ogre::FrameEvent& evt) 
{
  return true;
}

bool
IntroState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    return false;
  
  return true;
}

void
IntroState::keyPressed
(const OIS::KeyEvent &e)
{
  // Transición al siguiente estado.
  // Espacio --> PlayState
  if (e.key == OIS::KC_SPACE) {
    changeState(PlayState::getSingletonPtr());
  }
}

void
IntroState::keyReleased
(const OIS::KeyEvent &e )
{
  if (e.key == OIS::KC_ESCAPE) {
    _exitGame = true;
  }
}

void
IntroState::mouseMoved
(const OIS::MouseEvent &e)
{
}

void
IntroState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

void
IntroState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

IntroState*
IntroState::getSingletonPtr ()
{
return msSingleton;
}

IntroState&
IntroState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}
