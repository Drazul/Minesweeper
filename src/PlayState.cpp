#include "IntroState.h"
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

  _camera->setPosition(Ogre::Vector3(7.19941, 13.4578, -10.0267));
  _camera->setDirection(Ogre::Vector3(-0.00103734, -0.62784, 0.778358));

  _camera->setNearClipDistance(0.1);
  _camera->setFarClipDistance(100);

  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);

  createScene();
  createOverlay();


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

void PlayState::createOverlay() {
  _overlayManager = Ogre::OverlayManager::getSingletonPtr();
  Ogre::Overlay *overlay = _overlayManager->getByName("Info");
  overlay->show();
}


void PlayState::createScene() {

  createGroundScene();
  createBoardScene();
}

void PlayState::createBoardScene() {
  //Se debe sustitutir esto por el nivel correspondiente
  Ogre::Entity * entity;
  Ogre::SceneNode * sceneNode = _sceneManager->createSceneNode("BoardSceneNode");
  Ogre::SceneNode * sceneNodeCells;
  std::stringstream sceneNodeName;

  std::vector<char> visibleBoard = _minesweeper.get_visible_board();

  for(int i = 0; i < _level; i++) {
    for(int j = 0; j < _level; j++) {
      sceneNodeName << "Cell" << i << j << "";

      sceneNodeCells = _sceneManager->createSceneNode(sceneNodeName.str());
      entity = _sceneManager->createEntity("Cell.mesh");
      entity->setMaterialName("hierba");

      sceneNodeCells->attachObject(entity);
      //Multiplico por 2 porque ese es el tamaño de cada celda, el 0.1 restante es para separarlas un poquito
      sceneNodeCells->setPosition(2.1 * i, 0, 2.1 * j);
      sceneNode->addChild(sceneNodeCells);

      sceneNodeName.str(""); sceneNodeName.str(""); // Limpiamos el stream

      _entityNodes.push_back(entity);
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
  //Se debe sustitutir esto por el nivel correspondiente
  int size = _entityNodes.size();
    //std::cout << size << std::endl;

  std::vector<char> visibleBoard = _minesweeper.get_visible_board();
  std::stringstream materialName;

  for(int i = size-1; i >= 0; i--) {
    _entityNodes[i]->setVisible(true);
    switch(visibleBoard[i]){
      case 'B':
        _entityNodes[i]->setMaterialName("bomba");
        break;
      case '*':
        _entityNodes[i]->setMaterialName("hierba");
        break;
      case ' ':
        _entityNodes[i]->setVisible(false);
        break;
      case 'F':
        //_entityNodes[i]->getParentSceneNode()->attachObject(_sceneManager->createEntity("Flagpole_Flag_Flag1.mesh"));
        _entityNodes[i]->setMaterialName("flag");
        break;
      default:
        materialName.str(""); materialName.str(""); // Limpiamos el stream
        materialName << "numero" << visibleBoard[i];
        _entityNodes[i]->setMaterialName(materialName.str());
        break;
    }
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
  // Transición al siguiente estado.
  // Espacio --> IntroState
  //Al poner esa transicion ya no coge el escape
  //if (e.key == OIS::KC_SPACE) {
  //  changeState(IntroState::getSingletonPtr());
  //}
  if (e.key == OIS::KC_ESCAPE) {
    _exitGame = true;
  }
  if (e.key == OIS::KC_A) {
    std::cout << "Tecla A" << std::endl;
    _minesweeper.initialize();
    actualizeBoard();
  }
   
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
  float posx = e.state.X.abs;
  float posy = e.state.Y.abs;

  Ogre::OverlayElement *oe;
  oe = _overlayManager->getOverlayElement("cursor");
  oe->setLeft(posx);  oe->setTop(posy);
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
    //std::cout << "nombre de la entidad en la que pincho " << it->movable->getParentSceneNode()->getName() << std::endl;
    std::string name = it->movable->getParentSceneNode()->getName();
    if(name != "Ground") {
      std::string number = name.substr (4);
      //std::cout << number << std::endl;
      int index = std::stoi(number);
      //std::cout << index << std::endl;

      //time_t  time1 = 0, time2 = 0;

      if (mbleft) {
        _minesweeper.execute(index/10, index % 10);
        //std::cout << "se ha ejecutado" << std::endl;
      }
      else if (mbright) {
        //time(&time2);
        //std::cout << "time2 - time1: " << difftime(time1, time2) << std::endl;
        //std::cout << "time1: " << time1 << " time2: " << time2 << std::endl;

        //if (difftime(time2, time1) > 100) {
          _minesweeper.put_flag(index/10, index % 10);
          //time(&time1);
          //std::cout << "se ha puesto bandera" << std::endl;
        //}
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
