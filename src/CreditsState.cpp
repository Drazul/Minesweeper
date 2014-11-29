#include "CreditsState.h"

template<> CreditsState* Ogre::Singleton<CreditsState>::msSingleton = 0;

void
CreditsState::enter ()
{
  if(_root == nullptr){
    _root = Ogre::Root::getSingletonPtr();

    std::cout << _message << std::endl;
    _sceneManager = _root->getSceneManager("IntroSceneManager");

    _camera = _sceneManager->getCamera("MenuCamera");
    _viewport = _root->getAutoCreatedWindow()->getViewport(0);
    _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 1.0, 0.0));

    _sceneNode = _sceneManager->createSceneNode("CreditsScene");

    _sceneNode->setPosition(0, 2.5, -1.5);
    _sceneNode->setScale(8, 0, 8);
    _sceneNode->pitch(Ogre::Degree(90), Ogre::Node::TS_LOCAL);
    _sceneNode->yaw(Ogre::Degree(180), Ogre::Node::TS_LOCAL);

    _sceneManager->getRootSceneNode()->addChild(_sceneNode);
  }

  Ogre::Entity* entity = _sceneManager->createEntity("cell.mesh");
  entity->setMaterialName("credits");

  _sceneNode->attachObject(entity);

  _exitGame = false;
}

void
CreditsState::exit ()
{
  _sceneNode->detachAllObjects();
}

void
CreditsState::pause ()
{
}

void
CreditsState::resume ()
{
}

bool
CreditsState::frameStarted
(const Ogre::FrameEvent& evt)
{
  return true;
}

bool
CreditsState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    return false;
  
  return true;
}

void
CreditsState::keyPressed
(const OIS::KeyEvent &e) {
    popState();
}

void
CreditsState::keyReleased
(const OIS::KeyEvent &e)
{
}

void
CreditsState::mouseMoved
(const OIS::MouseEvent &e)
{
}

void
CreditsState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  popState();
}

void
CreditsState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

CreditsState*
CreditsState::getSingletonPtr ()
{
return msSingleton;
}

CreditsState&
CreditsState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}
