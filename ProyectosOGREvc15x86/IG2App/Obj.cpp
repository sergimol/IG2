#include "Obj.h"

Obj::Obj(SceneNode* node)
{
	mNode = node;
	mSM = mNode->getCreator();
}

Aspa::Aspa(SceneNode* node) : Obj(node) {
	Ogre::Entity* ent = mSM->createEntity("cube.mesh");
	tableroNode = mNode->createChildSceneNode();
	tableroNode->attachObject(ent);
	tableroNode->scale(10, 1, 0.1);

	ent = mSM->createEntity("Barrel.mesh");
	adornoNode = mNode->createChildSceneNode();
	adornoNode->attachObject(ent);
	adornoNode->scale(5, 15, 5);
	adornoNode->setPosition(450, 0, 30);
	adornoNode->setInheritOrientation(false);
}

AspasMolino::AspasMolino(SceneNode* node, int n) : Obj(node)
{
	numAspas = n;
	aspasNode = mNode->createChildSceneNode("Aspas");

	Ogre::Entity* ent = mSM->createEntity("Barrel.mesh");
	cilindroCentralNode = mNode->createChildSceneNode("Centro");
	cilindroCentralNode->attachObject(ent);
	cilindroCentralNode->scale(20, 10, 20);
	cilindroCentralNode->pitch(Degree(90));

	for (int i = 0; i < numAspas; ++i) {
		arrayAspas.push_back(aspasNode->createChildSceneNode("Aspa " + std::to_string(i)));
		Aspa* aux = new Aspa(arrayAspas[i]);

		arrayAspas[i]->roll(Degree(360 / numAspas * i));
	}
}

bool AspasMolino::keyPressed(const OgreBites::KeyboardEvent& evt)
{
	if (evt.keysym.sym == SDLK_g)
	{
		aspasNode->roll(Ogre::Degree(-1));
	}
	else if (evt.keysym.sym == SDLK_c) {
		cilindroCentralNode->translate(0, 0, -1);
	}
	return true;
}

Molino::Molino(SceneNode* node, int n) : Obj(node)
{
	Ogre::Entity* ent = mSM->createEntity("Barrel.mesh");
	cuerpoNode = mNode->createChildSceneNode("Cuerpo");
	cuerpoNode->attachObject(ent);
	cuerpoNode->scale(30, 35, 30);
	cuerpoNode->translate(0, -90, 0);

	ent = mSM->createEntity("sphere.mesh");
	techoNode = mNode->createChildSceneNode("Techo");
	techoNode->attachObject(ent);
	techoNode->scale(0.75, 0.75, 0.75);

	aspasNode = mNode->createChildSceneNode("Aspas Molino");
	aspas = new AspasMolino(aspasNode, n);
	aspasNode->scale(0.3, 0.3, 0.3);
	aspasNode->translate(0, 0, 80);
}

bool Molino::keyPressed(const OgreBites::KeyboardEvent& evt)
{	
	if (evt.keysym.sym == SDLK_h) {
		aspasNode->translate(0, 0, -80, SceneNode::TS_LOCAL);
		aspasNode->yaw(Degree(1));
		aspasNode->translate(0, 0, 80, SceneNode::TS_LOCAL);
	}
	else
		aspas->keyPressed(evt);

	return true;
}
