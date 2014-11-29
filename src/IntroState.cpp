#include "IntroState.h"
#include "CreditsState.h"
#include "LevelState.h"

template<> IntroState* Ogre::Singleton<IntroState>::msSingleton = 0;

void
IntroState::enter ()
{
  if(_root == nullptr) {
    _root = Ogre::Root::getSingletonPtr();
    _sceneManager = _root->createSceneManager(Ogre::ST_GENERIC, "IntroSceneManager");
    _camera = _sceneManager->createCamera("MenuCamera");

    _camera->setPosition(Ogre::Vector3(0.5, 4, 12));
    _camera->setDirection(Ogre::Vector3(0, 0, -1));

    _camera->setNearClipDistance(0.1);
    _camera->setFarClipDistance(100);
  }

  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);

  createMenu();

  _win = _root->getAutoCreatedWindow();
  _raySceneQuery = _sceneManager->createRayQuery(Ogre::Ray());

  _root->startRendering();

  _exitGame = false;
}

void IntroState::createMenu() {
  _menuSceneNode = _sceneManager->createSceneNode("MenuScene");

  Ogre::SceneNode* playNode = _menuSceneNode->createChildSceneNode("playNode");
  Ogre::SceneNode* creditsNode = _menuSceneNode->createChildSceneNode("creditsNode");
  Ogre::SceneNode* quitNode = _menuSceneNode->createChildSceneNode("quitNode");
  Ogre::SceneNode* background = _menuSceneNode->createChildSceneNode("background");


  _playEnt = _sceneManager->createEntity("play.mesh");
  _creditsEnt = _sceneManager->createEntity("credits.mesh");
  _quitEnt = _sceneManager->createEntity("quit.mesh");
  Ogre::Entity* backgroundEnt = _sceneManager->createEntity("cell.mesh");

  _playEnt->setMaterialName("playOFF");
  _creditsEnt->setMaterialName("creditsOFF");
  _quitEnt->setMaterialName("quitOFF");
  backgroundEnt->setMaterialName("background");

  playNode->attachObject(_playEnt);
  creditsNode->attachObject(_creditsEnt);
  quitNode->attachObject(_quitEnt);
  background->attachObject(backgroundEnt);

  playNode->setPosition(-0.75, 5, 0);
  creditsNode->setPosition(-0.75, 2.75, 0);
  quitNode->setPosition(-0.75, 0.5, 0);
  background->setPosition(0, 2.5, -1.5);

  background->setScale(8, 0, 8);

  playNode->pitch(Ogre::Degree(90), Ogre::Node::TS_LOCAL);
  creditsNode->pitch(Ogre::Degree(90), Ogre::Node::TS_LOCAL);
  quitNode->pitch(Ogre::Degree(90), Ogre::Node::TS_LOCAL);
  background->pitch(Ogre::Degree(90), Ogre::Node::TS_LOCAL);

  playNode->yaw(Ogre::Degree(-90), Ogre::Node::TS_LOCAL);
  creditsNode->yaw(Ogre::Degree(-90), Ogre::Node::TS_LOCAL);
  quitNode->yaw(Ogre::Degree(-90), Ogre::Node::TS_LOCAL);
  background->yaw(Ogre::Degree(180), Ogre::Node::TS_LOCAL);

  _sceneManager->getRootSceneNode()->addChild(_menuSceneNode);
}

Ogre::Ray IntroState::setRayQuery(int posx, int posy) {
  Ogre::Ray rayMouse = _camera->getCameraToViewportRay
    (posx/float(_win->getWidth()), posy/float(_win->getHeight()));
  _raySceneQuery->setRay(rayMouse);
  _raySceneQuery->setSortByDistance(true);
  return (rayMouse);
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
  _menuSceneNode->setVisible(false);
}

void
IntroState::resume ()
{
  _menuSceneNode->setVisible(true);
}

bool
IntroState::frameStarted
(const Ogre::FrameEvent& evt) 
{
  if (_exitGame)
    return false;

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
  if (e.key == OIS::KC_ESCAPE) {
    _exitGame = true;
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
  float posx = e.state.X.abs;
  float posy = e.state.Y.abs;

  bool mbleft, mbright; 

  mbleft = e.state.buttonDown(OIS::MB_Left);
  mbright = e.state.buttonDown(OIS::MB_Right);

  if(!mbleft && !mbright) {
    setRayQuery(posx, posy);
    Ogre::RaySceneQueryResult &result = _raySceneQuery->execute();
    Ogre::RaySceneQueryResult::iterator it;
    it = result.begin();
    std::string name;
    if (it != result.end()) {
      name = it->movable->getParentSceneNode()->getName();

      if(name.find("play") == 0)
        _playEnt->setMaterialName("playON");
      else
        _playEnt->setMaterialName("playOFF");
 
      if(name.find("credits") == 0)
        _creditsEnt->setMaterialName("creditsON");
      else
        _creditsEnt->setMaterialName("creditsOFF");

      if(name.find("quit") == 0)
        _quitEnt->setMaterialName("quitON");
      else
        _quitEnt->setMaterialName("quitOFF");
    }
  }
}

void
IntroState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  float posx = e.state.X.abs;
  float posy = e.state.Y.abs;

  bool mbleft, mbright; 

  mbleft = e.state.buttonDown(OIS::MB_Left);
  mbright = e.state.buttonDown(OIS::MB_Right);

  if(mbleft || mbright) {
    setRayQuery(posx, posy);
    Ogre::RaySceneQueryResult &result = _raySceneQuery->execute();
    Ogre::RaySceneQueryResult::iterator it;
    it = result.begin();

    if (it != result.end()) {
      std::string name = it->movable->getParentSceneNode()->getName();
      if (mbleft) {
        if(name.find("play") == 0) 
          pushState(LevelState::getSingletonPtr());
        else if(name.find("quit") == 0) 
          std::exit(0);
        else if(name.find("credits") == 0)
          pushState(CreditsState::getSingletonPtr());
      }
    }
  }
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
