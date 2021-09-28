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
	tableroNode->setScale(10, 1, 0.1);

	ent = mSM->createEntity("Barrel.mesh");
	adornoNode = mNode->createChildSceneNode();
	adornoNode->attachObject(ent);
	adornoNode->setScale(5, 15, 5);
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
	cilindroCentralNode->setScale(20, 10, 20);
	cilindroCentralNode->pitch(Degree(90));

	SceneNode* aux;
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
		aspasNode->roll(Ogre::Degree(1));
	}
	return true;
}
