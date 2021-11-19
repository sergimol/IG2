#include "IG2App.h"

#include <OgreEntity.h>
#include <OgreInput.h>
#include <SDL_keycode.h>
#include <OgreMeshManager.h>

using namespace Ogre;

bool IG2App::keyPressed(const OgreBites::KeyboardEvent& evt)
{
	if (evt.keysym.sym == SDLK_ESCAPE)
	{
		getRoot()->queueEndRendering();
	}
	else if (evt.keysym.sym == SDLK_g && mClockNode != nullptr)
		mClockNode->roll(Ogre::Degree(-1));
	else if (evt.keysym.sym == SDLK_h) {
		if (mSpheresNode != nullptr)
			mSpheresNode->roll(Ogre::Degree(-1));
		//else if (ficticioDronNode != nullptr)
		//	// mover el dron
		//	ficticioDronNode->pitch(Degree(-1));
		else if (avionFicticio != nullptr) {
			AxisAlignedBox aabAv = avionNode->_getWorldAABB();
			AxisAlignedBox aabDr;
			for (int i = 0; i < microDrones.size(); ++i) {
				aabDr = microDrones[i].first->_getWorldAABB();
				if (aabAv.intersection(aabDr) != AxisAlignedBox()) {
					delete microDrones[i].first;
					delete microDrones[i].second;
					microDrones.erase(microDrones.begin() + i);
					if (microDrones.empty())
						dron->receiveEvent(GAME_FINISH, nullptr);
				}
			}
		}
	}
	else if (evt.keysym.sym == SDLK_b)
	{
		if (mSpheresNode != nullptr)
		{
			nodoDron->translate(0, -550, 0, SceneNode::TS_LOCAL);
			nodoDron->pitch(Ogre::Degree(-1));
			nodoDron->translate(0, 550, 0, SceneNode::TS_LOCAL);
		}

	}
	else if (evt.keysym.sym == SDLK_j && ficticioDronNode != nullptr)
		// girar el dron
		ficticioDronNode->yaw(Degree(-1));
	//else if (evt.keysym.sym == SDLK_???)

	for (EntidadIG* a : EntidadIG::appListeners)
		a->keyPressed(evt);

	return true;
}

void IG2App::frameRendered(const Ogre::FrameEvent& evt)
{
	for (EntidadIG* a : EntidadIG::appListeners)
		a->frameRendered(evt);
}

void IG2App::shutdown()
{
	mShaderGenerator->removeSceneManager(mSM);
	mSM->removeRenderQueueListener(mOverlaySystem);

	mRoot->destroySceneManager(mSM);

	delete mTrayMgr;  mTrayMgr = nullptr;
	delete mCamMgr; mCamMgr = nullptr;
	delete dron; dron = nullptr;

	//Pues limpio el vector 
	/*for (EntidadIG* a : EntidadIG::appListeners)
		delete a;
	EntidadIG::appListeners.clear();*/
	// do not forget to call the base 
	IG2ApplicationContext::shutdown();
}

void IG2App::setup(void)
{
	srand(time(NULL));

	// do not forget to call the base first
	IG2ApplicationContext::setup();

	mSM = mRoot->createSceneManager();

	// register our scene with the RTSS
	mShaderGenerator->addSceneManager(mSM);

	mSM->addRenderQueueListener(mOverlaySystem);

	mTrayMgr = new OgreBites::TrayManager("TrayGUISystem", mWindow.render);
	mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
	addInputListener(mTrayMgr);

	addInputListener(this);
	setupScene();
}

void IG2App::setupScene(void)
{
	// create the camera
	Camera* cam = mSM->createCamera("Cam");
	cam->setNearClipDistance(1);
	cam->setFarClipDistance(10000);
	cam->setAutoAspectRatio(true);
	//cam->setPolygonMode(Ogre::PM_WIREFRAME); 

	mCamNode = mSM->getRootSceneNode()->createChildSceneNode("nCam");
	mCamNode->attachObject(cam);

	mCamNode->setPosition(0, 0, 1000);
	mCamNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);
	mCamNode->setDirection(Ogre::Vector3((0, 0, 0.0)));

	// and tell it to render into the main window
	Viewport* vp = getRenderWindow()->addViewport(cam);
	vp->setBackgroundColour(Ogre::ColourValue(0.6, 0.7, 0.8));

	//------------------------------------------------------------------------

	// without light we would just get a black screen 

	Light* luz = mSM->createLight("Luz");
	luz->setType(Ogre::Light::LT_DIRECTIONAL);
	luz->setDiffuseColour(1, 1, 1);

	//mLightNode = mSM->getRootSceneNode()->createChildSceneNode("nLuz");
	mLightNode = mCamNode->createChildSceneNode("nLuz");
	mLightNode->attachObject(luz);

	mLightNode->setDirection(Ogre::Vector3(-1, -1, -1));  //vec3.normalise();
	//mLightNode->setPosition(0, 0, 1000);

	//------------------------------------------------------------------------

	// finally something to render
	 /* mClockNode = mSM->getRootSceneNode()->createChildSceneNode("Clock");
	 setupHours();*/
	Ogre::Entity* ent = mSM->createEntity("sphere.mesh");
	planetaNode = mSM->getRootSceneNode()->createChildSceneNode("Planeta");
	planetaNode->attachObject(ent);
	planetaNode->scale(0.25, 0.25, 0.25);
	planetaNode->translate(360, 0, -800 / 3);
	ent->setMaterialName("Cursed");

	/*ficticioDronNode = mSM->getRootSceneNode()->createChildSceneNode("Dron ficticio");
	nodoDron = ficticioDronNode->createChildSceneNode("Dron");
	dron = new Dron(nodoDron, 3, 5, true);
	nodoDron->scale(0.1, 0.1, 0.1);
	nodoDron->translate(0, 550, 0);
	EntidadIG::addListener(dron);*/

	/*for (int i = 0; i < 50; ++i) {
		SceneNode* ficticio = mSM->getRootSceneNode()->createChildSceneNode();
		SceneNode* nodo = ficticio->createChildSceneNode();
		Dron* d = new Dron(nodo, 3, 5, false);
		nodo->scale(0.05, 0.05, 0.05);
		nodo->translate(0, 550, 0);
		int r = rand() % 360;
		ficticio->yaw(Degree(r));
		r = rand() % 360;
		ficticio->pitch(Degree(r));
		EntidadIG::addListener(d);
		microDrones.push_back(std::make_pair(nodo, d));
	}*/

	//AVION
	avionFicticio = mSM->getRootSceneNode()->createChildSceneNode();
	avionNode = avionFicticio->createChildSceneNode();
	avionObj = new Avion(avionNode);
	EntidadIG::addListener(avionObj);
	avionFicticio->translate(0, 400, 0);
	avionNode->translate(300, 0, 0);

	rioNode = mSM->getRootSceneNode()->createChildSceneNode();
	rio = new Plano(rioNode, 1080, 800, 100, 80, "Water", Vector3(0, 0, 0));
	EntidadIG::addListener(rio);

	plataformaRNode = mSM->getRootSceneNode()->createChildSceneNode();
	plataformaR = new Plano(rioNode, 360, 800 / 3, 100, 80, "Rojo", Vector3(360, 1, -800 / 3));

	plataformaANode = mSM->getRootSceneNode()->createChildSceneNode();
	plataformaA = new Plano(rioNode, 360, 800 / 3, 100, 80, "Amarillo", Vector3(-360, 1, 800 / 3));

	sinbadFicticio = mSM->getRootSceneNode()->createChildSceneNode();
	sinbadNode = sinbadFicticio->createChildSceneNode();
	sinbad = new Sinbad(sinbadNode);
	sinbad->Arma();
	sinbadNode->translate(-360, 100, 800 / 3);
	EntidadIG::addListener(sinbad);

	// Niebla
	nieblaNode = mSM->getRootSceneNode()->createChildSceneNode();
	niebla = mSM->createBillboardSet("niebla", 6);
	niebla->setDefaultDimensions(400, 400);
	niebla->setMaterialName("Smoke");
	nieblaNode->attachObject(niebla);
	nieblaNode->translate(300, 200, 0);
	Billboard* bb = niebla->createBillboard(Vector3(-100, 0, 50));
	bb = niebla->createBillboard(Vector3(100, 0, 50));
	bb = niebla->createBillboard(Vector3(-100, 0, -50));
	bb = niebla->createBillboard(Vector3(100, 0, -50));
	bb = niebla->createBillboard(Vector3(0, 0, 0));

	bombaNode = mSM->getRootSceneNode()->createChildSceneNode();
	bomba = new Bomba(bombaNode);
	EntidadIG::addListener(bomba);
	//------------------------------------------------------------------------

	mCamMgr = new OgreBites::CameraMan(mCamNode);
	addInputListener(mCamMgr);
	mCamMgr->setStyle(OgreBites::CS_ORBIT);
	//mCamMgr->setTarget(mSinbadNode);  
	//mCamMgr->setYawPitchDist(Radian(0), Degree(30), 100);

	//------------------------------------------------------------------------

}

void IG2App::setupHours() {
	Ogre::Entity* ent;
	mSpheresNode = mClockNode->createChildSceneNode("Esferas");
	for (int i = 0; i < 12; ++i) {
		ent = mSM->createEntity("sphere.mesh");
		mHourNode[i] = mSpheresNode->createChildSceneNode("Hora " + std::to_string(i));
		mHourNode[i]->attachObject(ent);

		mHourNode[i]->setPosition(Ogre::Math::Sin(Ogre::Math::DegreesToRadians(30 * i)) * 750, Ogre::Math::Cos(Ogre::Math::DegreesToRadians(30 * i)) * 750, 0);

		if (i % 2 == 0)
			mSM->getSceneNode("Hora " + std::to_string(i))->setScale(0.5, 0.5, 0.5);
	}

	ent = mSM->createEntity("cube.mesh");
	mHoursHandNode = mClockNode->createChildSceneNode("Aguja horas");
	mHoursHandNode->attachObject(ent);
	mSM->getSceneNode("Aguja horas")->setScale(0.35, 4, 0.1);
	mSM->getSceneNode("Aguja horas")->setPosition(0, 200, 0);

	ent = mSM->createEntity("cube.mesh");
	mHoursHandNode = mClockNode->createChildSceneNode("Aguja minutos");
	mHoursHandNode->attachObject(ent);
	mSM->getSceneNode("Aguja minutos")->setScale(0.25, 6, 0.1);
	mSM->getSceneNode("Aguja minutos")->roll(Ogre::Degree(-90));
	mSM->getSceneNode("Aguja minutos")->setPosition(250, 0, 0);

	ent = mSM->createEntity("cube.mesh");
	mHoursHandNode = mClockNode->createChildSceneNode("Aguja segundos");
	mHoursHandNode->attachObject(ent);
	mSM->getSceneNode("Aguja segundos")->setScale(0.1, 6.5, 0.1);
	mSM->getSceneNode("Aguja segundos")->roll(Ogre::Degree(-240));
	mSM->getSceneNode("Aguja segundos")->setPosition(-250, -150, 0);
}
