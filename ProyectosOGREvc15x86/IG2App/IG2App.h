#ifndef __IG2App_H__
#define __IG2App_H__

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreTrays.h>
#include <OgreCameraMan.h>

#include "Obj.h"
#include "EntidadIG.h"


class IG2App : public  OgreBites::IG2ApplicationContext, OgreBites::InputListener 
{
public:
  explicit IG2App() : IG2ApplicationContext("IG2App") { };  // new -> setup()  
  virtual ~IG2App() { };   // delete -> shutdown()  
 
protected:
  virtual void setup();
  virtual void shutdown();
  virtual void setupScene();

  void setupHours();

  virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);  // InputListener
      
  Ogre::SceneManager* mSM = nullptr;
  OgreBites::TrayManager* mTrayMgr = nullptr;    
  Ogre::SceneNode* mLightNode = nullptr;
  Ogre::SceneNode* mCamNode = nullptr;
  Ogre::SceneNode* mSinbadNode = nullptr;
  OgreBites::CameraMan* mCamMgr = nullptr;
 /* Ogre::SceneNode* mColumnsNode = nullptr;
  Ogre::SceneNode* mLowerNode = nullptr;
  Ogre::SceneNode* mUpperNode = nullptr;*/
  Ogre::SceneNode* mClockNode = nullptr;
  Ogre::SceneNode* mHoursHandNode = nullptr;
  Ogre::SceneNode* mMinutesHandNode = nullptr;
  Ogre::SceneNode* mSecondsHandNode = nullptr;
  Ogre::SceneNode* mSpheresNode = nullptr;
  Ogre::SceneNode* mHourNode[12];
  Ogre::SceneNode* nodoDron = nullptr;
  Dron* dron = nullptr;
  SceneNode* planetaNode = nullptr;
  SceneNode* ficticioDronNode = nullptr;
  SceneNode* planoNode = nullptr;
  Plano* plano = nullptr;

  //AVION
  SceneNode* avionNode = nullptr;
  Avion* avionObj = nullptr;
};

#endif
