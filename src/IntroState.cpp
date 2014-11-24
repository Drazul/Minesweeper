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
  
  _viewport->setBackgroundColour(Ogre::ColourValue(0, 1.0, 1.0));

  _camera->setPosition(Ogre::Vector3(0.5, 4, 12));
  _camera->setDirection(Ogre::Vector3(0, 0, -1));

  _camera->setNearClipDistance(0.1);
  _camera->setFarClipDistance(100);

  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);

  createOverlay();
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
  Ogre::SceneNode* moreNode = menuSceneNode->createChildSceneNode("moreNode");
  Ogre::SceneNode* quitNode = menuSceneNode->createChildSceneNode("quitNode");
  Ogre::SceneNode* background = menuSceneNode->createChildSceneNode("background");


  //Ogre::Entity* wallEnt = _sceneManager->createEntity("wall.mesh");
  Ogre::Entity* playEnt = _sceneManager->createEntity("play.mesh");
  Ogre::Entity* moreEnt = _sceneManager->createEntity("more.mesh");
  Ogre::Entity* quitEnt = _sceneManager->createEntity("quit.mesh");
  Ogre::Entity* backgroundEnt = _sceneManager->createEntity("cell.mesh");

  playEnt->setMaterialName("playOFF");
  moreEnt->setMaterialName("moreOFF");
  quitEnt->setMaterialName("quitOFF");

  //wallNode->attachObject(wallEnt);
  playNode->attachObject(playEnt);
  moreNode->attachObject(moreEnt);
  quitNode->attachObject(quitEnt);
  background->attachObject(backgroundEnt);

  //wallNode->setPosition(0, 0, 0);
  playNode->setPosition(0, 7, 0);
  moreNode->setPosition(0, 4.5, 0);
  quitNode->setPosition(0, 2, 0);
  background->setPosition(0, 0, -2);

  background->setScale(20, 0, 30);

  playNode->pitch(Ogre::Degree(90), Ogre::Node::TS_LOCAL);
  moreNode->pitch(Ogre::Degree(90), Ogre::Node::TS_LOCAL);
  quitNode->pitch(Ogre::Degree(90), Ogre::Node::TS_LOCAL);
  background->pitch(Ogre::Degree(90), Ogre::Node::TS_LOCAL);

  playNode->yaw(Ogre::Degree(-90), Ogre::Node::TS_LOCAL);
  moreNode->yaw(Ogre::Degree(-90), Ogre::Node::TS_LOCAL);
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

void IntroState::createOverlay() {
  _overlayManager = Ogre::OverlayManager::getSingletonPtr();
  Ogre::Overlay *overlay = _overlayManager->getByName("Info");
  overlay->show();
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

  Ogre::OverlayElement *oe;
  oe = _overlayManager->getOverlayElement("cursor");
  oe->setLeft(posx);  oe->setTop(posy);
}

void
IntroState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  //MoouseButtonID MB_Left = 0, MB_Right,
  float posx = e.state.X.abs;
  float posy = e.state.Y.abs;

  bool mbleft, mbright; // Botones del raton pulsados

  mbleft = e.state.buttonDown(OIS::MB_Left);
  mbright = e.state.buttonDown(OIS::MB_Right);

  //Con esto le dices desde donde empezar el rayQuery. El compilador dice que nunca se utiliza
  setRayQuery(posx, posy);
  Ogre::RaySceneQueryResult &result = _raySceneQuery->execute();
  Ogre::RaySceneQueryResult::iterator it;
  it = result.begin();

  if (it != result.end()) {
    //Aqui se en la casilla que pincho, puede ejecutarla directamente
    std::string name = it->movable->getParentSceneNode()->getName();
    std::cout << name << std::endl;
    if(name.find("play") == 0) {
      if(mbleft) 
        changeState(PlayState::getSingletonPtr());
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
