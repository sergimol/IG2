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
	Aspa(SceneNode* node, bool adorno);
	~Aspa() {};
protected:
	SceneNode* tableroNode = nullptr;
	SceneNode* adornoNode = nullptr;
};

class AspasMolino : public Obj {
public:
	AspasMolino(SceneNode* node, int n, bool a);
	~AspasMolino() {};

	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);
protected:
	SceneNode* cilindroCentralNode = nullptr;
	int numAspas;
	SceneNode* aspasNode = nullptr;
	std::vector<SceneNode*> arrayAspas;
};

class Molino : public Obj {
public:
	Molino(SceneNode* node, int n);
	~Molino() {};
protected:
	SceneNode* techoNode = nullptr;
	SceneNode* cuerpoNode = nullptr;
	SceneNode* aspasNode = nullptr;
	SceneNode* nodoRotarAspas = nullptr;
	AspasMolino* aspas = nullptr;

	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);
};

class RotorDron : public Obj {
public:
	RotorDron(SceneNode* node, int n);
	~RotorDron() {};
protected:
	int numHelices;
	SceneNode* esferaNode = nullptr;
	SceneNode* helicesNode = nullptr;
	AspasMolino* helices = nullptr;

	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);
};