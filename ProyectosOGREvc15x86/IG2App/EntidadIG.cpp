#include "EntidadIG.h"

// Inicializacion del vector
std::vector<EntidadIG*> EntidadIG::appListeners = std::vector<EntidadIG*>(0, nullptr);

EntidadIG::EntidadIG(SceneNode* node)
{
	mNode = node;
	mSM = mNode->getCreator();
}

//Destructora 
EntidadIG::~EntidadIG()
{
	
}


void EntidadIG::addListener(EntidadIG* entidad)
{
	appListeners.push_back(entidad);
}

void EntidadIG::sendEvent(EntidadIG* entidad)
{
	for (EntidadIG* e : appListeners)
		e->receiveEvent(this);
}

Aspa::Aspa(SceneNode* node, bool adorno) : EntidadIG(node) {
	// Tablero
	Ogre::Entity* ent = mSM->createEntity("cube.mesh");
	tableroNode = mNode->createChildSceneNode();
	tableroNode->attachObject(ent);
	tableroNode->scale(5, 1, 0.1);
	tableroNode->translate(225, 0, 0);

	// Adorno
	if (adorno) {
		ent = mSM->createEntity("Barrel.mesh");
		adornoNode = mNode->createChildSceneNode();
		adornoNode->attachObject(ent);
		adornoNode->scale(5, 12, 5);
		adornoNode->setPosition(450, 0, 20);
		adornoNode->setInheritOrientation(false);
	}
}

AspasMolino::AspasMolino(SceneNode* node, int n, bool a) : EntidadIG(node)
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
		if (id % 2 == 0)
			aspasNode->roll(Ogre::Degree(-1));
		else
			aspasNode->roll(Ogre::Degree(1));
	}
	else if (evt.keysym.sym == SDLK_c) {
		cilindroCentralNode->translate(0, 0, -1);
	}
	return true;
}

Molino::Molino(SceneNode* node, int n) : EntidadIG(node)
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

RotorDron::RotorDron(SceneNode* node, int n) : EntidadIG(node)
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

BrazoDron::BrazoDron(SceneNode* node, int n, int i) : EntidadIG(node)
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

Dron::Dron(SceneNode* node, int nBrazos, int nAspas) : EntidadIG(node)
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

	// Luz
	foco = mSM->createLight();
	foco->setType(Light::LT_SPOTLIGHT);
	foco->setDiffuseColour(ColourValue(1.0f, 1.0f, 1.0f));
	foco->setSpotlightOuterAngle(Degree(90.0f));
	focoNode = mNode->createChildSceneNode();
	focoNode->attachObject(foco);
	focoNode->setDirection(Vector3(0, -1, 0));
}

bool Dron::keyPressed(const OgreBites::KeyboardEvent& evt)
{
	for (int i = 0; i < numBrazos; ++i)
		brazos[i]->keyPressed(evt);

	return true;
}


//AVION
Avion::Avion(SceneNode* node) : EntidadIG(node)
{

	//CUERPO
	Ogre::Entity* ent = mSM->createEntity("sphere.mesh");
	cuerpoNode = mNode->createChildSceneNode();
	cuerpoNode->setScale(0.5, 0.5, 0.5);
	cuerpoNode->attachObject(ent);

	//ALAS
	ent = mSM->createEntity("cube.mesh");
	alaINode = mNode->createChildSceneNode();
	alaINode->attachObject(ent);
	alaINode->setScale(1.2, 0.05, 0.5);
	alaINode->translate(-80, 0, 0);
	ent = mSM->createEntity("cube.mesh");
	alaDNode = mNode->createChildSceneNode();
	alaDNode->attachObject(ent);
	alaDNode->setScale(1.2, 0.05, 0.5);
	alaDNode->translate(80, 0, 0);


	//FRENTE CABINA
	ent = mSM->createEntity("Barrel.mesh");
	frenteNode = mNode->createChildSceneNode();
	frenteNode->attachObject(ent);
	frenteNode->setScale(1, 1, 1);
	frenteNode->pitch(Ogre::Degree(90));
	frenteNode->setScale(5, 5, 5);
	frenteNode->translate(0, 0, 50);

	//SANDOKAN
	ent = mSM->createEntity("ninja.mesh");
	pilotoNode = mNode->createChildSceneNode();
	pilotoNode->attachObject(ent);
	pilotoNode->translate(0, 10, 0);
	pilotoNode->yaw(Ogre::Degree(180));
	pilotoNode->setScale(0.3, 0.3, 0.3);

	//HELICES
	heliceINode = mNode->createChildSceneNode();
	heliceIObj = new AspasMolino(heliceINode, 5, true);
	heliceINode->translate(-90, 0, 25);
	heliceINode->setScale(0.06, 0.06, 0.06);
	heliceDNode = mNode->createChildSceneNode();
	heliceDObj = new AspasMolino(heliceDNode, 5, true);
	heliceDNode->translate(90, 0, 25);
	heliceDNode->setScale(0.06, 0.06, 0.06);

	// LUZ
	foco = mSM->createLight();
	foco->setType(Light::LT_SPOTLIGHT);
	foco->setDiffuseColour(ColourValue(1.0f, 1.0f, 1.0f));
	foco->setSpotlightOuterAngle(Degree(90.0f));
	focoNode = mNode->createChildSceneNode();
	focoNode->attachObject(foco);
	focoNode->setDirection(Vector3(0, -1, 0));
}

bool Avion::keyPressed(const OgreBites::KeyboardEvent& evt)
{
	heliceDObj->keyPressed(evt, 0);
	heliceIObj->keyPressed(evt, 0);

	return true;
}

Plano::Plano(SceneNode* node, Real w, Real h, int xSeg, int ySeg) : EntidadIG(node) {
	//PLANO
  //Crear la malla del plano
	MeshManager::getSingleton().createPlane("mPlane1080x800", //nombre
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,    //grupo de recursos utilizados
		Plane(Vector3::UNIT_Y, 0),							//orientacion del plano
		w, h,											//dimensiones 1080 x 800
		xSeg, ySeg,												//divisiones de la malla 100 x 80
		true,													//creacion de normales 
		1, 1.0, 1.0,											//coordenadas de texturas con repeticion
		Vector3::UNIT_Z);										//orientacion de la textura
	//Cargamos la malla y inicializamos el nodo
	Ogre::Entity* plane = mSM->createEntity("mPlane1080x800");
	planoNode = mNode->createChildSceneNode();
	planoNode->attachObject(plane);
	planoNode->translate(0, -100, 0);
}