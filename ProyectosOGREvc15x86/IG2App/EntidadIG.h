#include <OgreSceneNode.h>
#include <OgreTrays.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreMeshManager.h>

#include "IG2ApplicationContext.h"

#include "vector"

using namespace Ogre;

#pragma once
class EntidadIG:public OgreBites::InputListener
{
public:
	//Constructora y destructora
	EntidadIG(SceneNode* node);
	~EntidadIG();

	//Vector estatico de listeners
	static std::vector<EntidadIG*> appListeners;

	//Añade entidad al vector
	static void addListener(EntidadIG* entidad);

	virtual void frameRendered(const Ogre::FrameEvent& evt) {}
	void sendEvent(EntidadIG* entidad);
	virtual void receiveEvent(EntidadIG* entidad) {};

protected:
	Ogre::SceneNode* mNode;
	Ogre::SceneManager* mSM;

	//virtual bool keyPressed(const OgreBites::KeyboardEvent& evt) { return false; };
};

// Aspa de molino / helice de rotor
class Aspa : public EntidadIG {
public:
	Aspa(SceneNode* node, bool adorno); // el booleano indica si debe llevar adorno o no
	~Aspa() {};
protected:
	SceneNode* tableroNode = nullptr;
	SceneNode* adornoNode = nullptr;
};

// Conjunto de aspas del molino
class AspasMolino : public EntidadIG {
public:
	AspasMolino(SceneNode* node, int n, bool a); // n  = numero de aspas, a = para los adornos
	~AspasMolino() {};

protected:
	SceneNode* cilindroCentralNode = nullptr;
	int numAspas;
	SceneNode* aspasNode = nullptr;
	std::vector<SceneNode*> arrayAspas;

public:
	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt, int id);
};

class Molino : public EntidadIG {
public:
	Molino(SceneNode* node, int n);
	~Molino() {};
protected:
	SceneNode* techoNode = nullptr;
	SceneNode* cuerpoNode = nullptr;
	SceneNode* aspasNode = nullptr;
	SceneNode* nodoRotarAspas = nullptr; // nodo ficticio para rotar las aspas alrededor del molino
	AspasMolino* aspas = nullptr;

	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);
};

class RotorDron : public EntidadIG {
public:
	RotorDron(SceneNode* node, int n);
	~RotorDron() {};
protected:
	int numAspas;
	SceneNode* esferaNode = nullptr;
	SceneNode* helicesNode = nullptr;
	AspasMolino* helices = nullptr;

public:
	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt, int id);
};

class BrazoDron : public EntidadIG {
public:
	BrazoDron(SceneNode* node, int n, int i);
	~BrazoDron() {};
protected:
	int numAspas;
	int id;
	SceneNode* cilindroNode = nullptr;
	SceneNode* rotorNode = nullptr;
	RotorDron* rotor = nullptr;

public:
	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);
};

class Dron : public EntidadIG {
public:
	Dron(SceneNode* node, int brazos, int aspas);
	~Dron() {};
protected:
	int numBrazos;
	int numAspas;
	SceneNode* esferaNode = nullptr;
	SceneNode* rotorNode = nullptr;
	std::vector<SceneNode*> brazoNodes;
	std::vector<BrazoDron*> brazos;
	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);
};

class Avion : public EntidadIG {
public:
	Avion(SceneNode* node);
	~Avion() {};
protected:
	int numBrazos;
	int numAspas;
	SceneNode* cuerpoNode = nullptr;
	SceneNode* alaINode = nullptr;
	SceneNode* alaDNode = nullptr;
	SceneNode* frenteNode = nullptr;
	SceneNode* pilotoNode = nullptr;
	SceneNode* heliceINode = nullptr;
	AspasMolino* heliceIObj;
	AspasMolino* heliceDObj;
	SceneNode* heliceDNode = nullptr;

	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);
};

class Plano : public EntidadIG {
public:
	Plano(SceneNode* node, Real w, Real h, int xSeg, int ySeg);
	~Plano() {};
protected:
	SceneNode* planoNode = nullptr;
};
