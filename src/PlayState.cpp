#include "PauseState.h"
#include "PlayState.h"

template<> PlayState* Ogre::Singleton<PlayState>::msSingleton = 0;

void
PlayState::enter ()
{
  _level = Difficulty::Easy;

  _minesweeper.set_difficulty(_level);
  _minesweeper.initialize();  

  if(_root == nullptr)
    _root = Ogre::Root::getSingletonPtr();
  
  if(_sceneManager == nullptr)
    _sceneManager = _root->createSceneManager(Ogre::ST_GENERIC, "PlaySceneManager");

  if(_camera == nullptr)
    _camera = _sceneManager->createCamera("MainCamera");

  //_camera->setPosition(Ogre::Vector3(7.19941, 13.4578, -10.0267));
  //_camera->setDirection(Ogre::Vector3(-0.00103734, -0.62784, 0.778358));
  _camera->setPosition(Ogre::Vector3(7.19941, 16.4578, -10.0267));
  _camera->setDirection(Ogre::Vector3(-0.00103734, -0.82784, 0.778358));
  _camera->setNearClipDistance(0.1);
  _camera->setFarClipDistance(100);

  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);

  createScene();

  _win = _root->getAutoCreatedWindow();
  _raySceneQuery = _sceneManager->createRayQuery(Ogre::Ray());

  _root->startRendering();

  _exitGame = false;
}

Ogre::Ray PlayState::setRayQuery(int posx, int posy) {
  Ogre::Ray rayMouse = _camera->getCameraToViewportRay
    (posx/float(_win->getWidth()), posy/float(_win->getHeight()));
  _raySceneQuery->setRay(rayMouse);
  _raySceneQuery->setSortByDistance(true);
  //_raySceneQuery->setQueryMask(mask);
  return (rayMouse);
}

void PlayState::createScene() {

  createGroundScene();
  createBoardScene();
}

void PlayState::createBoardScene() {
  //Se debe sustitutir esto por el nivel correspondiente
  Ogre::Entity * entity;
  Ogre::Entity * flagEnt;

  Ogre::SceneNode * sceneNode = _sceneManager->createSceneNode("BoardSceneNode");

  Ogre::SceneNode * sceneNodeFlags;
  Ogre::SceneNode * sceneNodeCells;
  std::stringstream sceneNodeName;

  std::vector<char> visibleBoard = _minesweeper.get_visible_board();

  for(int i = 0; i < _level; i++) {
    for(int j = 0; j < _level; j++) {
      sceneNodeName.str(""); sceneNodeName.str(""); // Limpiamos el stream
      sceneNodeName << "Cell" << i/100 << i/10 << i % 10 << j/100 << j/10 << j % 10 << "";
      sceneNodeCells = _sceneManager->createSceneNode(sceneNodeName.str());

      sceneNodeName.str(""); sceneNodeName.str(""); // Limpiamos el stream
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

  //Creo el nodo de escena y la entidad que van a contener al plano
  Ogre::SceneNode* GroundNode = _sceneManager->createSceneNode("Ground");
  Ogre::Entity* groundEnt = _sceneManager->createEntity("Ground", "planeGround");
  groundEnt->setMaterialName("Ground");
  GroundNode->attachObject(groundEnt);

  //Añado una luz a la escena, le doy una direccion y la adjunto al nodo de escena
  _sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE); 
  Ogre::Light* light = _sceneManager->createLight("Light");
  light->setType(Ogre::Light::LT_DIRECTIONAL);
  light->setDirection(Ogre::Vector3(1,-1,0));
  GroundNode->attachObject(light);

  //Adjunto al nodo de escena principal el nodo de escena creado
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
    PauseState* pause = PauseState::getSingletonPtr();
    if(state == BoardState::GameOver)
      pause->setMessage("GameOver");

    if(state == BoardState::Winner)
      pause->setMessage("Winner");

    pushState(pause);
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
  
  if (e.key == OIS::KC_A) {
    _minesweeper.initialize();
    actualizeBoard();
  }

  if (e.key == OIS::KC_P)
    pushState(PauseState::getSingletonPtr());
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
    // Esto es para saltar las bandera en el rayquery
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
