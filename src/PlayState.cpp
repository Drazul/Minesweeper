#include "EndState.h"
#include "PlayState.h"

template<> PlayState* Ogre::Singleton<PlayState>::msSingleton = 0;

void
PlayState::enter ()
{
  _minesweeper.set_difficulty(_level);
  _minesweeper.initialize();  

  _root = Ogre::Root::getSingletonPtr();
  
  _sceneManager = _root->createSceneManager(Ogre::ST_GENERIC, "PlaySceneManager");

  initializeCamera();
  
  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);

  createScene();

  _win = _root->getAutoCreatedWindow();
  _raySceneQuery = _sceneManager->createRayQuery(Ogre::Ray());

  _root->startRendering();

  _exitGame = false;
}

void 
PlayState::setLevel(Difficulty level) {
  _level = level;
}

void
PlayState::initializeCamera() {
  _camera = _sceneManager->createCamera("MainCamera");

  if(_level == Difficulty::Easy){
    _camera->setPosition(Ogre::Vector3(8.1985, 15.7293, -9.34037));
    _camera->setDirection(Ogre::Vector3(-0.000912916, -0.728545, 0.684998));
  } 
  else if (_level == Difficulty::VeryEasy) {
    _camera->setPosition(Ogre::Vector3(2.19394, 12.0866, -5.92338));
    _camera->setDirection(Ogre::Vector3(-0.000912916, -0.728545, 0.684998));
  }
  else if (_level == Difficulty::Medium) {
    _camera->setPosition(Ogre::Vector3(17.2131, 27.386, -20.2883));
    _camera->setDirection(Ogre::Vector3(-0.000912916, -0.728545, 0.684998));
  }

  _camera->setNearClipDistance(0.1);
  _camera->setFarClipDistance(100);
}

Ogre::Ray PlayState::setRayQuery(int posx, int posy) {
  Ogre::Ray rayMouse = _camera->getCameraToViewportRay
    (posx/float(_win->getWidth()), posy/float(_win->getHeight()));
  _raySceneQuery->setRay(rayMouse);
  _raySceneQuery->setSortByDistance(true);
  return (rayMouse);
}

void PlayState::createScene() {

  createGroundScene();
  createBoardScene();
}

void PlayState::createBoardScene() {
  Ogre::Entity * entity;
  Ogre::Entity * flagEnt;

  Ogre::SceneNode * sceneNode = _sceneManager->createSceneNode("BoardSceneNode");

  Ogre::SceneNode * sceneNodeFlags;
  Ogre::SceneNode * sceneNodeCells;
  std::stringstream sceneNodeName;

  std::vector<char> visibleBoard = _minesweeper.get_visible_board();

  for(int i = 0; i < _level; i++) {
    for(int j = 0; j < _level; j++) {
      sceneNodeName.str(""); sceneNodeName.str(""); 
      sceneNodeName << "Cell" << i/100 << i/10 << i % 10 << j/100 << j/10 << j % 10 << "";
      sceneNodeCells = _sceneManager->createSceneNode(sceneNodeName.str());

      sceneNodeName.str(""); sceneNodeName.str(""); 
      sceneNodeName << "Flag" << i/100 << i/10 << i % 10 << j/100 << j/10 << j % 10 << "";
      sceneNodeFlags = _sceneManager->createSceneNode(sceneNodeName.str());

      entity = _sceneManager->createEntity("Cell.mesh");
      entity->setMaterialName("hierba");
      sceneNodeCells->attachObject(entity);
      sceneNodeCells->setPosition(2.3 * i, 0, 2.3 * j);
      sceneNodeCells->setScale(1, 10, 1);

      flagEnt = _sceneManager->createEntity("Flagpole_Flag_Flag1.mesh");
      sceneNodeFlags->attachObject(flagEnt);
      sceneNodeFlags->setPosition(2.3 * i, 0.5, 2.3 * j);
      sceneNodeFlags->pitch(Ogre::Degree(90), Ogre::Node::TS_LOCAL);
      sceneNodeFlags->roll(Ogre::Degree(180), Ogre::Node::TS_LOCAL);
      flagEnt->setVisible(false);

      sceneNode->addChild(sceneNodeCells);
      sceneNode->addChild(sceneNodeFlags);

      _entityNodes.push_back(std::make_pair(entity, flagEnt));
    }
  }
  _sceneManager->getRootSceneNode()->addChild(sceneNode);
}

void PlayState::createGroundScene() {
  //Creo el plano del suelo
  Ogre::Plane planeGround(Ogre::Vector3::UNIT_Y, 0);
  Ogre::MeshManager::getSingleton().createPlane("planeGround",
  Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, planeGround,
  200,200,1,1,true,1,20,20,Ogre::Vector3::UNIT_Z);

  Ogre::SceneNode* GroundNode = _sceneManager->createSceneNode("Ground");
  Ogre::Entity* groundEnt = _sceneManager->createEntity("Ground", "planeGround");
  groundEnt->setMaterialName("Ground");
  GroundNode->attachObject(groundEnt);

  _sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE); 
  Ogre::Light* light = _sceneManager->createLight("Light");
  light->setType(Ogre::Light::LT_DIRECTIONAL);
  light->setDirection(Ogre::Vector3(1,-1,0));
  GroundNode->attachObject(light);

  _sceneManager->getRootSceneNode()->addChild(GroundNode);
}

void
PlayState::actualizeBoard() {
  int size = _entityNodes.size();

  std::vector<char> visibleBoard = _minesweeper.get_visible_board();
  std::stringstream materialName;

  for(int i = size-1; i >= 0; i--) {
    _entityNodes[i].first->setVisible(true);
    _entityNodes[i].second->setVisible(false);

    switch(visibleBoard[i]){
      case 'B':
        _entityNodes[i].first->setMaterialName("bomba");
        break;
      case '*':
        _entityNodes[i].first->setMaterialName("hierba");
        break;
      case ' ':
        _entityNodes[i].first->setVisible(false);
        break;
      case 'F':
        _entityNodes[i].second->setVisible(true);
        break;
      default:
        materialName.str(""); materialName.str(""); // Limpiamos el stream
        materialName << "numero" << visibleBoard[i];
        _entityNodes[i].first->setMaterialName(materialName.str());
        break;
    }
  }
  BoardState state = _minesweeper.get_board_state();

  if(state != BoardState::Continue) {
    EndState* endState = EndState::getSingletonPtr();
    if(state == BoardState::GameOver)
      endState->setMessage("GameOver", _level);

    if(state == BoardState::Winner)
      endState->setMessage("Winner", _level);

    pushState(endState);
  }
}

void
PlayState::exit()
{
  _sceneManager->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
}

void
PlayState::pause ()
{
}

void
PlayState::resume ()
{
  _minesweeper.initialize();
  actualizeBoard();
}

bool
PlayState::frameStarted
(const Ogre::FrameEvent& evt) 
{
  return true;
}

bool
PlayState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    return false;
  
  return true;
}

void
PlayState::keyPressed
(const OIS::KeyEvent &e)
{
  if (e.key == OIS::KC_ESCAPE) 
    _exitGame = true;
  
  if (e.key == OIS::KC_P)
    pushState(EndState::getSingletonPtr());
}

void
PlayState::keyReleased
(const OIS::KeyEvent &e )
{
  if (e.key == OIS::KC_ESCAPE) {
    _exitGame = true;
  }
}

void
PlayState::mouseMoved
(const OIS::MouseEvent &e)
{
}

void
PlayState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  float posx = e.state.X.abs;
  float posy = e.state.Y.abs;

  bool mbleft, mbright;

  mbleft = e.state.buttonDown(OIS::MB_Left);
  mbright = e.state.buttonDown(OIS::MB_Right);

  setRayQuery(posx, posy);
  Ogre::RaySceneQueryResult &result = _raySceneQuery->execute();
  Ogre::RaySceneQueryResult::iterator it;
  it = result.begin();

  if (it != result.end()) {
    std::string name = it->movable->getParentSceneNode()->getName();
    while (name.find("Flag") == 0) {
      it++;
      name = it->movable->getParentSceneNode()->getName();
    }

    if(name.find("Cell") == 0) {
      std::string number = name.substr (4);
      int index = std::stoi(number);
      int x = index / 1000;
      int y = index % 1000;
      if (mbleft) {
        _minesweeper.execute(x, y);
      }
      else if (mbright) {
        _minesweeper.put_flag(x, y);
      }
      actualizeBoard();
    }
  }
}

void
PlayState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

PlayState*
PlayState::getSingletonPtr ()
{
return msSingleton;
}

PlayState&
PlayState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}