/*********************************************************************
 * Módulo 2. Curso de Experto en Desarrollo de Videojuegos
 * Autor: Carlos González Morcillo     Carlos.Gonzalez@uclm.es
 *
 * You can redistribute and/or modify this file under the terms of the
 * GNU General Public License ad published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * and later version. See <http://www.gnu.org/licenses/>.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.  
 *********************************************************************/

#include "MyApp.h" 

MyApp::MyApp() {
  _sceneManager = NULL;
  _framelistener = NULL;
}

MyApp::~MyApp() {
  delete _root;
  delete _framelistener;
}

int MyApp::start() {
  _root = new Root();
  
  if(!_root->restoreConfig()) {
    _root->showConfigDialog();
    _root->saveConfig();
  }

  RenderWindow* window = _root->initialise(true,"MyApp Example");
  _sceneManager = _root->createSceneManager(ST_INTERIOR);
  
  Camera* cam = _sceneManager->createCamera("MainCamera");
  cam->setPosition(Vector3(5.98, 2.73, 7.09));
  cam->lookAt(Vector3(-3.45,1.45,2.69));
  cam->setNearClipDistance(0.1);
  cam->setFarClipDistance(100);
  
  Viewport* viewport = window->addViewport(cam);
  viewport->setBackgroundColour(ColourValue(0.0,0.0,0.0));
  double width = viewport->getActualWidth();
  double height = viewport->getActualHeight();
  cam->setAspectRatio(width / height);
  
  loadResources();
  createScene();
  createOverlay();

  _framelistener = new MyFrameListener(window, cam, _overlayManager, _sceneManager);
  _root->addFrameListener(_framelistener);
  
  _root->startRendering();
  return 0;
}

void MyApp::loadResources() {
  ConfigFile cf;
  cf.load("resources.cfg");
  
  ConfigFile::SectionIterator sI = cf.getSectionIterator();
  String sectionstr, typestr, datastr;
  while (sI.hasMoreElements()) {
    sectionstr = sI.peekNextKey();
    ConfigFile::SettingsMultiMap *settings = sI.getNext();
    ConfigFile::SettingsMultiMap::iterator i;
    for (i = settings->begin(); i != settings->end(); ++i) {
      typestr = i->first;    datastr = i->second;
      ResourceGroupManager::getSingleton().addResourceLocation
            (datastr, typestr, sectionstr);	
    }
  }
  ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void MyApp::createScene() {

  createGroundScene();
  createPlane4RayQuery();
  createBoardScene();
}

void MyApp::createBoardScene() {
  //Se debe sustitutir esto por el nivel correspondiente
  int level = 8, index =0;
  Ogre::Entity * entity;
  Ogre::SceneNode * sceneNode = _sceneManager->createSceneNode("BoardSceneNode");
  Ogre::SceneNode * sceneNodeCells;
  std::stringstream sceneNodeName;

  for(int i = 0; i < level; i++) {
    for(int j = 0; j < level; j++) {
      sceneNodeName << "Cell" << i << j << "SceneNode";

      sceneNodeCells = _sceneManager->createSceneNode(sceneNodeName.str());
      entity = _sceneManager->createEntity("Cell.mesh");
      sceneNodeCells->attachObject(entity);
      //Multiplico por 2 porque ese es el tamaño de cada celda, el 0.1 restante es para separarlas un poquito
      sceneNodeCells->setPosition(2.1 * i, 0, 2.1 * j);
      sceneNode->addChild(sceneNodeCells);

      sceneNodeName.str(""); sceneNodeName.str(""); // Limpiamos el stream
    }
  }
  _sceneManager->getRootSceneNode()->addChild(sceneNode);
}

void MyApp::createPlane4RayQuery() {
  //Creo el plano del suelo
  Ogre::Plane plane4RayQuery(Ogre::Vector3::UNIT_Y, 0);
  Ogre::MeshManager::getSingleton().createPlane("plane4RayQuery",
  Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane4RayQuery,
  200,200,1,1,true,1,20,20,Ogre::Vector3::UNIT_Z);

  //Creo el nodo de escena y la entidad que van a contener al plano
  Ogre::SceneNode* rayQueryNode = _sceneManager->createSceneNode("RayQueryNode");
  Ogre::Entity* rayQueryEnt = _sceneManager->createEntity("RayQueryNode", "plane4RayQuery");
  rayQueryNode->attachObject(rayQueryEnt);
  rayQueryNode->setVisible(false);

  rayQueryNode->setPosition(0, 0.05, 0); //Posiciono el plano justo encima de las casillas

  //Adjunto al nodo de escena principal el nodo de escena creado
  _sceneManager->getRootSceneNode()->addChild(rayQueryNode);
}

void MyApp::createGroundScene() {
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

void MyApp::createOverlay() {
  _overlayManager = OverlayManager::getSingletonPtr();
  Overlay *overlay = _overlayManager->getByName("Info");
  overlay->show();
}
