#include "PauseState.h"

template<> PauseState* Ogre::Singleton<PauseState>::msSingleton = 0;

void
PauseState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();
  std::cout << _message << std::endl;
  // Se recupera el gestor de escena y la cámara.
  _sceneManager = _root->getSceneManager("PlaySceneManager");
  _camera = _sceneManager->getCamera("MainCamera");
  _viewport = _root->getAutoCreatedWindow()->getViewport(0);
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 1.0, 0.0));

  Ogre::SceneNode* pauseSceneNode = _sceneManager->createSceneNode("PauseScene");

  Ogre::SceneNode* textNode = pauseSceneNode->createChildSceneNode("textNode");
  Ogre::Entity* entity = _sceneManager->createEntity("Mesh.mesh");
  entity->setMaterialName("red");

  textNode->attachObject(entity);

  textNode->setScale(2, 1, 2);
  textNode->setPosition(8, 7, 1);
  textNode->yaw(Ogre::Degree(180));

  _sceneManager->getRootSceneNode()->addChild(pauseSceneNode);

  _exitGame = false;
}

void
PauseState::setMessage(std::string message){
  _message = message;
}
void
PauseState::exit ()
{
}

void
PauseState::pause ()
{
}

void
PauseState::resume ()
{
}

bool
PauseState::frameStarted
(const Ogre::FrameEvent& evt)
{
  return true;
}

bool
PauseState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    return false;
  
  return true;
}

void
PauseState::keyPressed
(const OIS::KeyEvent &e) {
  // Tecla p --> Estado anterior.
  if (e.key == OIS::KC_P) {
    popState();
  }
  if (e.key == OIS::KC_ESCAPE)
    _exitGame = true;
}

void
PauseState::keyReleased
(const OIS::KeyEvent &e)
{
}

void
PauseState::mouseMoved
(const OIS::MouseEvent &e)
{
}

void
PauseState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

void
PauseState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

PauseState*
PauseState::getSingletonPtr ()
{
return msSingleton;
}

PauseState&
PauseState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}
