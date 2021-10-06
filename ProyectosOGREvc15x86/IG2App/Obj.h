#include <OgreSceneNode.h>
#include <OgreTrays.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreMeshManager.h>

#include "IG2ApplicationContext.h"

using namespace Ogre;

#pragma once
// Clase padre para todos los objetos
class Obj : public OgreBites::InputListener
{
public:
	Obj(SceneNode* node);
	~Obj() {};
protected:
	SceneNode* mNode = nullptr; // node principal
	SceneManager* mSM = nullptr; // referencia al manager de la escena

	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt) { return true; };
};

// Aspa de molino / helice de rotor
class Aspa : public Obj {
public:
	Aspa(SceneNode* node, bool adorno); // el booleano indica si debe llevar adorno o no
	~Aspa() {};
protected:
	SceneNode* tableroNode = nullptr; 
	SceneNode* adornoNode = nullptr;
};

// Conjunto de aspas del molino
class AspasMolino : public Obj {
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

class Molino : public Obj {
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

class RotorDron : public Obj {
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

class BrazoDron : public Obj {
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

class Dron : public Obj {
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

class Avion : public Obj {
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