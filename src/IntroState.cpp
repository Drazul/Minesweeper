#include "IntroState.h"
#include "PlayState.h"

template<> IntroState* Ogre::Singleton<IntroState>::msSingleton = 0;

void
IntroState::enter ()
{
  if(_root == nullptr)
    _root = Ogre::Root::getSingletonPtr();

  if(_sceneManager == nullptr)
    _sceneManager = _root->createSceneManager(Ogre::ST_GENERIC, "IntroSceneManager");

  if(_camera == nullptr)
    _camera = _sceneManager->createCamera("MenuCamera");
  
  _camera->setPosition(Ogre::Vector3(0.5, 4, 12));
  _camera->setDirection(Ogre::Vector3(0, 0, -1));

  _camera->setNearClipDistance(0.1);
  _camera->setFarClipDistance(100);

  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);

  createMenu();

  _win = _root->getAutoCreatedWindow();
  _raySceneQuery = _sceneManager->createRayQuery(Ogre::Ray());

  _root->startRendering();

  _exitGame = false;
}

void IntroState::createMenu() {
  Ogre::SceneNode* menuSceneNode = _sceneManager->createSceneNode("MenuScene");

  //Ogre::SceneNode* wallNode = menuSceneNode->createChildSceneNode("wallNode");
  Ogre::SceneNode* playNode = menuSceneNode->createChildSceneNode("playNode");
  Ogre::SceneNode* creditsNode = menuSceneNode->createChildSceneNode("creditsNode");
  Ogre::SceneNode* quitNode = menuSceneNode->createChildSceneNode("quitNode");
  Ogre::SceneNode* background = menuSceneNode->createChildSceneNode("background");


  //Ogre::Entity* wallEnt = _sceneManager->createEntity("wall.mesh");
  _playEnt = _sceneManager->createEntity("play.mesh");
  _creditsEnt = _sceneManager->createEntity("credits.mesh");
  _quitEnt = _sceneManager->createEntity("quit.mesh");
  Ogre::Entity* backgroundEnt = _sceneManager->createEntity("cell.mesh");

  _playEnt->setMaterialName("playOFF");
  _creditsEnt->setMaterialName("creditsOFF");
  _quitEnt->setMaterialName("quitOFF");

  //wallNode->attachObject(wallEnt);
  playNode->attachObject(_playEnt);
  creditsNode->attachObject(_creditsEnt);
  quitNode->attachObject(_quitEnt);
  background->attachObject(backgroundEnt);

  //wallNode->setPosition(0, 0, 0);
  playNode->setPosition(0, 7, 0);
  creditsNode->setPosition(0, 4.5, 0);
  quitNode->setPosition(0, 2, 0);
  background->setPosition(0, 0, -2);

  background->setScale(20, 0, 30);

  playNode->pitch(Ogre::Degree(90), Ogre::Node::TS_LOCAL);
  creditsNode->pitch(Ogre::Degree(90), Ogre::Node::TS_LOCAL);
  quitNode->pitch(Ogre::Degree(90), Ogre::Node::TS_LOCAL);
  background->pitch(Ogre::Degree(90), Ogre::Node::TS_LOCAL);

  playNode->yaw(Ogre::Degree(-90), Ogre::Node::TS_LOCAL);
  creditsNode->yaw(Ogre::Degree(-90), Ogre::Node::TS_LOCAL);
  quitNode->yaw(Ogre::Degree(-90), Ogre::Node::TS_LOCAL);
  background->yaw(Ogre::Degree(-90), Ogre::Node::TS_LOCAL);


  _sceneManager->getRootSceneNode()->addChild(menuSceneNode);

//camara position: Vector3(0.5, 4, 12)
//camara direction: Vector3(0, 0, -1)
}

Ogre::Ray IntroState::setRayQuery(int posx, int posy) {
  Ogre::Ray rayMouse = _camera->getCameraToViewportRay
    (posx/float(_win->getWidth()), posy/float(_win->getHeight()));
  _raySceneQuery->setRay(rayMouse);
  _raySceneQuery->setSortByDistance(true);
  //_raySceneQuery->setQueryMask(mask);
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
}

void
IntroState::resume ()
{
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
  // Transición al siguiente estado.
  // Espacio --> PlayState
  if (e.key == OIS::KC_SPACE) {
    changeState(PlayState::getSingletonPtr());
  }
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

  bool mbleft, mbright; // Botones del raton pulsados

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

  bool mbleft, mbright; // Botones del raton pulsados

  mbleft = e.state.buttonDown(OIS::MB_Left);
  mbright = e.state.buttonDown(OIS::MB_Right);

  if(mbleft || mbright) {
    //Con esto le dices desde donde empezar el rayQuery. El compilador dice que nunca se utiliza
    setRayQuery(posx, posy);
    Ogre::RaySceneQueryResult &result = _raySceneQuery->execute();
    Ogre::RaySceneQueryResult::iterator it;
    it = result.begin();

    if (it != result.end()) {
      //Aqui se en la casilla que pincho, puede ejecutarla directamente
      std::string name = it->movable->getParentSceneNode()->getName();
      if (mbleft) {
        if(name.find("play") == 0) {
          changeState(PlayState::getSingletonPtr());
        } else if(name.find("quit") == 0) {
          //_exitGame = true;
          std::exit(0);
        }
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
