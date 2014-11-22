#include <Ogre.h>
#include "MyFrameListener.h"

class MyApp {
  
private:
  SceneManager* _sceneManager;
  Root* _root;
  MyFrameListener* _framelistener;
  OverlayManager* _overlayManager;
  std::vector<Ogre::SceneNode> _sceneNodes;
public:
  MyApp();
  ~MyApp();  
  int start();
  void loadResources();
  void createScene();
  void createOverlay();
  void createGroundScene();
  void createBoardScene();
  void createPlane4RayQuery();

};
