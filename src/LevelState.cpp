#include "LevelState.h"
#include "PlayState.h"

template<> LevelState* Ogre::Singleton<LevelState>::msSingleton = 0;

void
LevelState::enter ()
{
  if(_root == nullptr){
    _root = Ogre::Root::getSingletonPtr();

    _sceneManager = _root->getSceneManager("IntroSceneManager");

    _camera = _sceneManager->getCamera("MenuCamera");

    _win = _root->getAutoCreatedWindow();

    _viewport = _win->getViewport(0);

    _sceneNode = _sceneManager->createSceneNode("CreditsScene");

    _sceneNode->setPosition(0, 2.5, -1.5);
    _sceneNode->setScale(8, 0, 8);
    _sceneNode->pitch(Ogre::Degree(90), Ogre::Node::TS_LOCAL);
    _sceneNode->yaw(Ogre::Degree(180), Ogre::Node::TS_LOCAL);

    _sceneManager->getRootSceneNode()->addChild(_sceneNode);
   _raySceneQuery = _sceneManager->createRayQuery(Ogre::Ray());
  }

  chooseLevel();

  _exitGame = false;
}

void 
LevelState::chooseLevel() {
  Ogre::SceneNode* _sceneLevel = _sceneManager->createSceneNode("SceneLevel");

  Ogre::SceneNode* veryEasyNode = _sceneLevel->createChildSceneNode("veryEasyNode");
  Ogre::SceneNode* EasyNode = _sceneLevel->createChildSceneNode("EasyNode");
  Ogre::SceneNode* MediumNode = _sceneLevel->createChildSceneNode("MediumNode");
  Ogre::SceneNode* background = _sceneLevel->createChildSceneNode("backgroundLevel");


  Ogre::Entity* veryEasyEnt = _sceneManager->createEntity("play.mesh");
  Ogre::Entity* easyEnt = _sceneManager->createEntity("credits.mesh");
  Ogre::Entity* mediumEnt = _sceneManager->createEntity("quit.mesh");
  Ogre::Entity* backgroundEnt = _sceneManager->createEntity("cell.mesh");

  veryEasyEnt->setMaterialName("veryEasy");
  easyEnt->setMaterialName("easy");
  mediumEnt->setMaterialName("medium");
  backgroundEnt->setMaterialName("chooseLevel");

  veryEasyNode->attachObject(veryEasyEnt);
  EasyNode->attachObject(easyEnt);
  MediumNode->attachObject(mediumEnt);
  background->attachObject(backgroundEnt);

  veryEasyNode->setPosition(-0.75, 5, 0);
  EasyNode->setPosition(-0.75, 2.75, 0);
  MediumNode->setPosition(-0.75, 0.5, 0);
  background->setPosition(0, 2.5, -1.5);

  background->setScale(8, 0, 8);

  veryEasyNode->pitch(Ogre::Degree(90), Ogre::Node::TS_LOCAL);
  EasyNode->pitch(Ogre::Degree(90), Ogre::Node::TS_LOCAL);
  MediumNode->pitch(Ogre::Degree(90), Ogre::Node::TS_LOCAL);
  background->pitch(Ogre::Degree(90), Ogre::Node::TS_LOCAL);

  veryEasyNode->yaw(Ogre::Degree(-90), Ogre::Node::TS_LOCAL);
  EasyNode->yaw(Ogre::Degree(-90), Ogre::Node::TS_LOCAL);
  MediumNode->yaw(Ogre::Degree(-90), Ogre::Node::TS_LOCAL);
  background->yaw(Ogre::Degree(180), Ogre::Node::TS_LOCAL);

  _sceneManager->getRootSceneNode()->addChild(_sceneLevel);
}

Ogre::Ray LevelState::setRayQuery(int posx, int posy) {
  Ogre::Ray rayMouse = _camera->getCameraToViewportRay
    (posx/float(_win->getWidth()), posy/float(_win->getHeight()));
  _raySceneQuery->setRay(rayMouse);
  _raySceneQuery->setSortByDistance(true);
  return (rayMouse);
}

void
LevelState::exit ()
{
}

void
LevelState::pause ()
{
}

void
LevelState::resume ()
{
}

bool
LevelState::frameStarted
(const Ogre::FrameEvent& evt)
{
  return true;
}

bool
LevelState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    return false;
  
  return true;
}

void
LevelState::keyPressed
(const OIS::KeyEvent &e) {  
  if (e.key == OIS::KC_ESCAPE)
    _exitGame = true;
}

void
LevelState::keyReleased
(const OIS::KeyEvent &e)
{
}

void
LevelState::mouseMoved
(const OIS::MouseEvent &e)
{
  /* Los nombres que consulta son play, credits y quit *
   * porque se reaprovechan dichos componentes         */
  /*if(_level == Difficulty::Empty) { 
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
          _playEnt->setMaterialName("veryEasy");
   
        if(name.find("credits") == 0)
          _creditsEnt->setMaterialName("creditsON");
        else
          _creditsEnt->setMaterialName("easy");

        if(name.find("quit") == 0)
          _quitEnt->setMaterialName("quitON");
        else
          _quitEnt->setMaterialName("medium");
      }
    }
  }*/
}

void
LevelState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  /* En este método busco las cadenas play, credits, y quit *
   * porque se reaprovechan las entidades .mesh del menú    */
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
        PlayState* playState = PlayState::getSingletonPtr();
        if(name.find("play") == 0) 
          playState->setLevel(Difficulty::VeryEasy);
        else if(name.find("credits") == 0)
          playState->setLevel(Difficulty::Easy);
        else if(name.find("quit") == 0) 
          playState->setLevel(Difficulty::Medium);

        changeState(playState);
      }
    }
  }
}

void
LevelState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

LevelState*
LevelState::getSingletonPtr ()
{
return msSingleton;
}

LevelState&
LevelState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}
