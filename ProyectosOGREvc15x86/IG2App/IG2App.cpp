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
	else if (evt.keysym.sym == SDLK_g && mClockNode != nullptr)
		mClockNode->roll(Ogre::Degree(-1));
	else if (evt.keysym.sym == SDLK_h && mSpheresNode != nullptr) 
		mSpheresNode->roll(Ogre::Degree(-1));		
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
  delete rotor; rotor = nullptr;
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
  mCamNode->setDirection(Ogre::Vector3((0, 0, 0.0)));  
  
  // and tell it to render into the main window
  Viewport* vp = getRenderWindow()->addViewport(cam);
  vp->setBackgroundColour(Ogre::ColourValue(0.7, 0.8, 0.9));

  //------------------------------------------------------------------------

  // without light we would just get a black screen 

  Light* luz = mSM->createLight("Luz");
  luz->setType(Ogre::Light::LT_DIRECTIONAL);
  luz->setDiffuseColour(0.75, 0.75, 0.75);

  mLightNode = mSM->getRootSceneNode()->createChildSceneNode("nLuz");
  //mLightNode = mCamNode->createChildSceneNode("nLuz");
  mLightNode->attachObject(luz);

  mLightNode->setDirection(Ogre::Vector3(0, -1, -1));  //vec3.normalise();
  //mLightNode->setPosition(0, 0, 1000);
 
  //------------------------------------------------------------------------

  // finally something to render
 /* mClockNode = mSM->getRootSceneNode()->createChildSceneNode("Clock");
  setupHours();*/
  //Ogre::Entity* ent = mSM->createEntity("Sinbad.mesh");

  nodoRotor = mSM->getRootSceneNode()->createChildSceneNode("Rotor");
  rotor = new RotorDron(nodoRotor, 6);
  addInputListener(rotor);
  //------------------------------------------------------------------------

  mCamMgr = new OgreBites::CameraMan(mCamNode);
  addInputListener(mCamMgr);
  mCamMgr->setStyle(OgreBites::CS_ORBIT);  
  //mCamMgr->setTarget(mSinbadNode);  
  //mCamMgr->setYawPitchDist(Radian(0), Degree(30), 100);

  //------------------------------------------------------------------------

}

void IG2App::setupHours() {
	Ogre::Entity* ent;
	mSpheresNode = mClockNode->createChildSceneNode("Esferas");
	for (int i = 0; i < 12; ++i) {
		ent = mSM->createEntity("sphere.mesh");
		mHourNode[i] = mSpheresNode->createChildSceneNode("Hora " + std::to_string(i));
		mHourNode[i]->attachObject(ent);

		mHourNode[i]->setPosition(Ogre::Math::Sin(Ogre::Math::DegreesToRadians(30 * i)) * 750, Ogre::Math::Cos(Ogre::Math::DegreesToRadians(30 * i)) * 750, 0);

		if (i % 2 == 0) 
			mSM->getSceneNode("Hora " + std::to_string(i))->setScale(0.5, 0.5, 0.5);
	}

	ent = mSM->createEntity("cube.mesh");
	mHoursHandNode = mClockNode->createChildSceneNode("Aguja horas");
	mHoursHandNode->attachObject(ent);
	mSM->getSceneNode("Aguja horas")->setScale(0.35, 4, 0.1);
	mSM->getSceneNode("Aguja horas")->setPosition(0, 200, 0);

	ent = mSM->createEntity("cube.mesh");
	mHoursHandNode = mClockNode->createChildSceneNode("Aguja minutos");
	mHoursHandNode->attachObject(ent);
	mSM->getSceneNode("Aguja minutos")->setScale(0.25, 6, 0.1);
	mSM->getSceneNode("Aguja minutos")->roll(Ogre::Degree(-90));
	mSM->getSceneNode("Aguja minutos")->setPosition(250, 0, 0);

	ent = mSM->createEntity("cube.mesh");
	mHoursHandNode = mClockNode->createChildSceneNode("Aguja segundos");
	mHoursHandNode->attachObject(ent);
	mSM->getSceneNode("Aguja segundos")->setScale(0.1, 6.5, 0.1);
	mSM->getSceneNode("Aguja segundos")->roll(Ogre::Degree(-240));
	mSM->getSceneNode("Aguja segundos")->setPosition(-250, -150, 0);
}
