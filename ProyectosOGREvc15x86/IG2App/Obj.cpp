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
	tableroNode->setScale(10, 2, 0.1);

	ent = mSM->createEntity("Barrel.mesh");
	adornoNode = mNode->createChildSceneNode();
	adornoNode->attachObject(ent);
	adornoNode->setScale(10, 20, 10);
	adornoNode->setPosition(425, 0, 30);
	adornoNode->setInheritOrientation(false);
}
