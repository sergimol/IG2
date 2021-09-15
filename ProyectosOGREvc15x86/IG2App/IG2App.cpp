#include "IG2App.h"

#include <OgreEntity.h>
#include <OgreInput.h>
#include <SDL_keycode.h>
#include <OgreMeshManager.h>

using namespace Ogre;

bool IG2App::keyPressed(const OgreBites::KeyboardEvent& evt)
{
  if (evt.keysym.sym == SDLK_ESCAPE)
  {
    getRoot()->queueEndRendering();
  }
  //else if (evt.keysym.sym == SDLK_???)
  
  return true;
}

void IG2App::shutdown()
{
  mShaderGenerator->removeSceneManager(mSM);  
  mSM->removeRenderQueueListener(mOverlaySystem);  
					
  mRoot->destroySceneManager(mSM);  

  delete mTrayMgr;  mTrayMgr = nullptr;
  delete mCamMgr; mCamMgr = nullptr;
  
  // do not forget to call the base 
  IG2ApplicationContext::shutdown();
}

void IG2App::setup(void)
{
  // do not forget to call the base first
  IG2ApplicationContext::setup();

  mSM = mRoot->createSceneManager();  

  // register our scene with the RTSS
  mShaderGenerator->addSceneManager(mSM);

  mSM->addRenderQueueListener(mOverlaySystem);

  mTrayMgr = new OgreBites::TrayManager("TrayGUISystem", mWindow.render);  
  mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
  addInputListener(mTrayMgr);

  addInputListener(this);   
  setupScene();
}

void IG2App::setupScene(void)
{
  // create the camera
  Camera* cam = mSM->createCamera("Cam");
  cam->setNearClipDistance(1); 
  cam->setFarClipDistance(10000);
  cam->setAutoAspectRatio(true);
  //cam->setPolygonMode(Ogre::PM_WIREFRAME); 

  mCamNode = mSM->getRootSceneNode()->createChildSceneNode("nCam");
  mCamNode->attachObject(cam);

  mCamNode->setPosition(0, 0, 1000);
  mCamNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);
  mCamNode->setDirection(Ogre::Vector3((0.7, 0.8, 0.9)));  
  
  // and tell it to render into the main window
  Viewport* vp = getRenderWindow()->addViewport(cam);
  vp->setBackgroundColour(Ogre::ColourValue(1, 1, 1));

  //------------------------------------------------------------------------

  // without light we would just get a black screen 

  Light* luz = mSM->createLight("Luz");
  luz->setType(Ogre::Light::LT_DIRECTIONAL);
  luz->setDiffuseColour(0.75, 0.75, 0.75);

  mLightNode = mSM->getRootSceneNode()->createChildSceneNode("nLuz");
  //mLightNode = mCamNode->createChildSceneNode("nLuz");
  mLightNode->attachObject(luz);

  mLightNode->setDirection(Ogre::Vector3(1, 0, 0));  //vec3.normalise();
  //lightNode->setPosition(0, 0, 1000);
 
  //------------------------------------------------------------------------

  // finally something to render

  Ogre::Entity* ent = mSM->createEntity("Sinbad.mesh");

  mSinbadNode = mSM->getRootSceneNode()->createChildSceneNode("nSinbad");
  mSinbadNode->attachObject(ent);

  mSinbadNode->setPosition(0, 20, 0);
  mSinbadNode->setScale(20, 20, 20);
  mSinbadNode->yaw(Ogre::Degree(180));
  //mSinbadNode->showBoundingBox(true);
  //mSinbadNode->setVisible(false);

  
  /*ent = mSM->createEntity("DamagedHelmet.mesh");
  mHelmetNode = mSM->getRootSceneNode()->createChildSceneNode("nHelmet");
  mHelmetNode->attachObject(ent);
  mHelmetNode->setScale(50, 50, 50);*/

  ent = mSM->createEntity("Columns.mesh");
  mColumnsNode = mSM->getRootSceneNode()->createChildSceneNode("nColumns");
  mColumnsNode->attachObject(ent);
  //mColumnsNode->setScale(50, 50, 50);
  
  ent = mSM->createEntity("RomanBathLower.mesh");
  mLowerNode = mSM->getRootSceneNode()->createChildSceneNode("nLower");
  mLowerNode->attachObject(ent);
  //mLowerNode->setScale(50, 50, 50);
  
  ent = mSM->createEntity("RomanBathUpper.mesh");
  mUpperNode = mSM->getRootSceneNode()->createChildSceneNode("nUpper");
  mUpperNode->attachObject(ent);
  //mLowerNode->setScale(50, 50, 50);
  //------------------------------------------------------------------------

  mCamMgr = new OgreBites::CameraMan(mCamNode);
  addInputListener(mCamMgr);
  mCamMgr->setStyle(OgreBites::CS_ORBIT);  
  
  //mCamMgr->setTarget(mSinbadNode);  
  //mCamMgr->setYawPitchDist(Radian(0), Degree(30), 100);

  //------------------------------------------------------------------------

}

