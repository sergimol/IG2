#include <OgreSceneNode.h>
#include <OgreTrays.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreMeshManager.h>

#include "IG2ApplicationContext.h"

using namespace Ogre;

#pragma once
class Obj : public OgreBites::InputListener
{
public:
	Obj(SceneNode* node);
	~Obj() {};
protected:
	SceneNode* mNode = nullptr;
	SceneManager* mSM = nullptr;
};

class Aspa : public Obj {
public:
	Aspa(SceneNode* node);
	~Aspa() {};
protected:
	SceneNode* tableroNode = nullptr;
	SceneNode* adornoNode = nullptr;
};

