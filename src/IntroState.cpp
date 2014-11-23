#include "IntroState.h"
#include "PlayState.h"

template<> IntroState* Ogre::Singleton<IntroState>::msSingleton = 0;

void
IntroState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();

  _sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "SceneManager");
  _camera = _sceneMgr->createCamera("IntroCamera");
  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  //_viewport->setBackgroundColour(Ogre::ColourValue(1.0, 1.0, 1.0));

    Ogre::Entity * entity;
     entity = _sceneMgr->createEntity("Cell.mesh");
     entity->setMaterialName("play_btn");

     Ogre::SceneNode* node = _sceneMgr->createSceneNode("playnode");
     node->attachObject(entity);
     _sceneMgr->getRootSceneNode()->addChild(node);
     node->setScale(2, 1, 1);
     node->setPosition(0, 3, 0);
     node->pitch(Ogre::Degree(90), Ogre::SceneNode::TS_LOCAL);
     node->yaw(Ogre::Degree(180), Ogre::SceneNode::TS_LOCAL);

     Ogre::SceneNode* node2 = _sceneMgr->createSceneNode("quitnode");
     entity = _sceneMgr->createEntity("Cell.mesh");
     entity->setMaterialName("exit_btn");
     node2->attachObject(entity);
     _sceneMgr->getRootSceneNode()->addChild(node2);
     node2->setScale(2, 1, 1);
     node2->setPosition(0, 0, 0);
     node2->pitch(Ogre::Degree(90), Ogre::SceneNode::TS_LOCAL);
     node2->yaw(Ogre::Degree(180), Ogre::SceneNode::TS_LOCAL);

     Ogre::SceneNode* node3 = _sceneMgr->createSceneNode("creditsnode");
     entity = _sceneMgr->createEntity("Cell.mesh");
     entity->setMaterialName("credits_btn");
     node3->attachObject(entity);
     _sceneMgr->getRootSceneNode()->addChild(node3);
     node3->setScale(2, 1, 1);
     node3->setPosition(0, -3, 0);
     node3->pitch(Ogre::Degree(90), Ogre::SceneNode::TS_LOCAL);
     node3->yaw(Ogre::Degree(180), Ogre::SceneNode::TS_LOCAL);

  _root->startRendering();

  _exitGame = false;
}

void
IntroState::exit()
{
  _sceneMgr->clearScene();
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
