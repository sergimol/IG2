#include "Obj.h"

Obj::Obj(SceneNode* node)
{
	mNode = node;
	mSM = mNode->getCreator();
}

Aspa::Aspa(SceneNode* node, bool adorno) : Obj(node) {
	// Tablero
	Ogre::Entity* ent = mSM->createEntity("cube.mesh");
	tableroNode = mNode->createChildSceneNode();
	tableroNode->attachObject(ent);
	tableroNode->scale(10, 1, 0.1);

	// Adorno
	if (adorno) {
		ent = mSM->createEntity("Barrel.mesh");
		adornoNode = mNode->createChildSceneNode();
		adornoNode->attachObject(ent);
		adornoNode->scale(5, 15, 5);
		adornoNode->setPosition(450, 0, 30);
		adornoNode->setInheritOrientation(false);
	}
}

AspasMolino::AspasMolino(SceneNode* node, int n, bool a) : Obj(node)
{
	numAspas = n;
	aspasNode = mNode->createChildSceneNode();

	// Cilindro del centro
	Ogre::Entity* ent = mSM->createEntity("Barrel.mesh");
	cilindroCentralNode = mNode->createChildSceneNode();
	cilindroCentralNode->attachObject(ent);
	cilindroCentralNode->scale(20, 10, 20);
	cilindroCentralNode->pitch(Degree(90));

	// Aspas
	for (int i = 0; i < numAspas; ++i) {
		arrayAspas.push_back(aspasNode->createChildSceneNode());
		Aspa* aux = new Aspa(arrayAspas[i], a);

		arrayAspas[i]->roll(Degree(360 / numAspas * i));
	}
}

bool AspasMolino::keyPressed(const OgreBites::KeyboardEvent& evt, int id)
{
	if (evt.keysym.sym == SDLK_g)
	{
		if(id % 2 == 0)
			aspasNode->roll(Ogre::Degree(-1));
		else
			aspasNode->roll(Ogre::Degree(1));
	}
	else if (evt.keysym.sym == SDLK_c) {
		cilindroCentralNode->translate(0, 0, -1);
	}
	return true;
}

Molino::Molino(SceneNode* node, int n) : Obj(node)
{
	// Cuerpo
	Ogre::Entity* ent = mSM->createEntity("Barrel.mesh");
	cuerpoNode = mNode->createChildSceneNode("Cuerpo");
	cuerpoNode->attachObject(ent);
	cuerpoNode->scale(30, 35, 30);
	cuerpoNode->translate(0, -90, 0);

	// Techo
	ent = mSM->createEntity("sphere.mesh");
	techoNode = mNode->createChildSceneNode("Techo");
	techoNode->attachObject(ent);
	techoNode->scale(0.75, 0.75, 0.75);

	// Aspas
	//nodoRotarAspas = mNode->createChildSceneNode("Aspas ficticio"); 
	//aspasNode = nodoRotarAspas->createChildSceneNode("Aspas Molino");
	aspasNode = mNode->createChildSceneNode("Aspas Molino");
	aspas = new AspasMolino(aspasNode, n, true);
	aspasNode->scale(0.3, 0.3, 0.3);
	aspasNode->translate(0, 0, 80);
}

bool Molino::keyPressed(const OgreBites::KeyboardEvent& evt)
{	
	// Rotacion aspas
	if (evt.keysym.sym == SDLK_h) {
		// Con truco
		aspasNode->translate(0, 0, -80, SceneNode::TS_LOCAL);
		aspasNode->yaw(Degree(1));
		aspasNode->translate(0, 0, 80, SceneNode::TS_LOCAL);
		// Con nodo ficticio
		//nodoRotarAspas->yaw(Degree(1));
	}
	
	aspas->keyPressed(evt, 0);

	return true;
}

RotorDron::RotorDron(SceneNode* node, int n) : Obj(node)
{
	// Esfera
	Ogre::Entity* ent = mSM->createEntity("sphere.mesh");
	esferaNode = mNode->createChildSceneNode();
	esferaNode->attachObject(ent);

	// Helices
	numAspas = n;
	helicesNode = mNode->createChildSceneNode();
	helices = new AspasMolino(helicesNode, numAspas, false);
	helicesNode->scale(0.2, 0.2, 0.2);
	helicesNode->pitch(Degree(-90));
	helicesNode->translate(0, 100, 0);
}

bool RotorDron::keyPressed(const OgreBites::KeyboardEvent& evt, int id)
{	
	helices->keyPressed(evt, id);

	return true;
}

BrazoDron::BrazoDron(SceneNode* node, int n, int i) : Obj(node)
{
	// Brazo
	id = i;
	Ogre::Entity* ent = mSM->createEntity("Barrel.mesh");
	cilindroNode = mNode->createChildSceneNode();
	cilindroNode->attachObject(ent);
	cilindroNode->scale(20, 50, 20);
	cilindroNode->pitch(Degree(90));
	cilindroNode->translate(0, 0, 235);

	// Rotor
	numAspas = n;
	rotorNode = mNode->createChildSceneNode();
	rotor = new RotorDron(rotorNode, numAspas);
}

bool BrazoDron::keyPressed(const OgreBites::KeyboardEvent& evt)
{
	rotor->keyPressed(evt, id);

	return true;
}

Dron::Dron(SceneNode* node, int nBrazos, int nAspas) : Obj(node)
{
	numBrazos = nBrazos;
	numAspas = nAspas;

	// Esfera central
	Ogre::Entity* ent = mSM->createEntity("sphere.mesh");
	esferaNode = mNode->createChildSceneNode();
	esferaNode->attachObject(ent);
	esferaNode->scale(2, 2, 2);
	
	// Brazos
	for (int i = 0; i < numBrazos; ++i) {
		brazoNodes.push_back(mNode->createChildSceneNode());
		brazos.push_back(new BrazoDron(brazoNodes[i], numAspas, i));
		brazoNodes[i]->yaw(Degree(360 / numBrazos * i));
		brazoNodes[i]->translate(0, 0, -450, SceneNode::TS_LOCAL);
	}
	brazoNodes[0]->translate(0, 0, -100, SceneNode::TS_LOCAL);
}

bool Dron::keyPressed(const OgreBites::KeyboardEvent& evt)
{
	for (int i = 0; i < numBrazos; ++i)
		brazos[i]->keyPressed(evt);

	return true;
}
