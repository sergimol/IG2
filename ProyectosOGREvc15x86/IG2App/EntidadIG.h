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

protected:
	Ogre::SceneNode* mNode;
	Ogre::SceneManager* mSM;

	//virtual bool keyPressed(const OgreBites::KeyboardEvent& evt) { return false; };
};

