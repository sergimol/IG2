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

	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt) { return true; };
};

class Aspa : public Obj {
public:
	Aspa(SceneNode* node);
	~Aspa() {};
protected:
	SceneNode* tableroNode = nullptr;
	SceneNode* adornoNode = nullptr;
};

class AspasMolino : public Obj {
public:
	AspasMolino(SceneNode* node, int n);
	~AspasMolino() {};
protected:
	SceneNode* cilindroCentralNode = nullptr;
	int numAspas;
	SceneNode* aspasNode = nullptr;
	std::vector<SceneNode*> arrayAspas;

	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);
};

