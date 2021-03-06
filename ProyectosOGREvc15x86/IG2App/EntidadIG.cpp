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

void EntidadIG::sendEvent(MessageType msg, EntidadIG* entidad)
{
	for (EntidadIG* e : appListeners)
		e->receiveEvent(msg, this);
}

Aspa::Aspa(SceneNode* node, bool adorno) : EntidadIG(node) {
	// Tablero
	Ogre::Entity* ent = mSM->createEntity("cube.mesh");
	tableroNode = mNode->createChildSceneNode();
	tableroNode->attachObject(ent);
	tableroNode->scale(5, 1, 0.1);
	tableroNode->translate(225, 0, 0);
	ent->setMaterialName("Marron");

	// Adorno
	if (adorno) {
		ent = mSM->createEntity("Barrel.mesh");
		adornoNode = mNode->createChildSceneNode();
		adornoNode->attachObject(ent);
		adornoNode->scale(5, 12, 5);
		adornoNode->setPosition(450, 0, 20);
		adornoNode->setInheritOrientation(false);
		ent->setMaterialName("Rojo");
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
	ent->setMaterialName("Dr");

	// Aspas
	for (int i = 0; i < numAspas; ++i) {
		arrayAspas.push_back(aspasNode->createChildSceneNode());
		Aspa* aux = new Aspa(arrayAspas[i], a);

		arrayAspas[i]->roll(Degree(360 / numAspas * i));
	}
}

void AspasMolino::frameRendered(const Ogre::FrameEvent& evt, int id)
{
	if (id % 2 == 0)
		aspasNode->roll(Ogre::Degree(-150 * evt.timeSinceLastFrame));
	else
		aspasNode->roll(Ogre::Degree(150 * evt.timeSinceLastFrame));
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
	ent->setMaterialName("Naranja");

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

void RotorDron::frameRendered(const Ogre::FrameEvent& evt, int id) {
	helices->frameRendered(evt, id);
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
	ent->setMaterialName("Tusk");

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

void BrazoDron::frameRendered(const Ogre::FrameEvent& evt, int id) {
	rotor->frameRendered(evt, id);
}

Dron::Dron(SceneNode* node, int nBrazos, int nAspas, bool control) : EntidadIG(node)
{
	numBrazos = nBrazos;
	numAspas = nAspas;

	// Esfera central
	Ogre::Entity* ent = mSM->createEntity("sphere.mesh");
	esferaNode = mNode->createChildSceneNode();
	esferaNode->attachObject(ent);
	esferaNode->scale(2, 2, 2);
	if (control) {
		ent->setMaterialName("Rojo");

		// Luz
		foco = mSM->createLight();
		foco->setType(Light::LT_SPOTLIGHT);
		foco->setDiffuseColour(ColourValue(1.0f, 1.0f, 1.0f));
		foco->setSpotlightOuterAngle(Degree(90.0f));
		focoNode = mNode->createChildSceneNode();
		focoNode->attachObject(foco);
		focoNode->setDirection(Vector3(0, -1, 0));
	}
	else
		ent->setMaterialName("Cursed");

	// Brazos
	for (int i = 0; i < numBrazos; ++i) {
		brazoNodes.push_back(mNode->createChildSceneNode());
		brazos.push_back(new BrazoDron(brazoNodes[i], numAspas, i));
		brazoNodes[i]->yaw(Degree(360 / numBrazos * i));
		brazoNodes[i]->translate(0, 0, -450, SceneNode::TS_LOCAL);
	}
	brazoNodes[0]->translate(0, 0, -100, SceneNode::TS_LOCAL);

	// Timer
	myTimer = new Timer();
}

void Dron::frameRendered(const Ogre::FrameEvent& evt)
{
	Ogre::Real time = evt.timeSinceLastFrame;
	if (moving) {
		if (!rotating) {
			unsigned long a = myTimer->getMilliseconds();
			if (a >= 2000) {
				rotationDir = rand() % 2;
				if (rotationDir == 0)
					rotationDir = -1;
				rotating = true;
				myTimer->reset();
			}
			else {
				mNode->getParent()->pitch(Degree(-10 * time));
			}
		}
		else {
			if (myTimer->getMilliseconds() >= 1000) {
				rotating = false;
				myTimer->reset();
			}
			else {
				mNode->getParent()->yaw(Degree(rotationDir * 10 * time));
			}
		}
	}
	for (int i = 0; i < numBrazos; ++i)
		brazos[i]->frameRendered(evt, i);
}

bool Dron::keyPressed(const OgreBites::KeyboardEvent& evt)
{
	for (int i = 0; i < numBrazos; ++i)
		brazos[i]->keyPressed(evt);

	return true;
}

void Dron::receiveEvent(MessageType msg, EntidadIG* e) {
	Entity* ent = static_cast<Ogre::Entity*>(esferaNode->getAttachedObject(0));
	switch (msg)
	{
	case KEY_R:
		moving = !moving;
		ent->setMaterialName("Rojo");
		break;
	case GAME_FINISH:
		ent->setMaterialName("Amarillo");
		break;
	default:
		break;
	}
}

//AVION
Avion::Avion(SceneNode* node) : EntidadIG(node)
{

	//CUERPO
	Ogre::Entity* ent = mSM->createEntity("sphere.mesh");
	cuerpoNode = mNode->createChildSceneNode();
	cuerpoNode->setScale(0.5, 0.5, 0.5);
	cuerpoNode->attachObject(ent);
	ent->setMaterialName("Rojo");

	//ALAS
	ent = mSM->createEntity("cube.mesh");
	alaINode = mNode->createChildSceneNode();
	alaINode->attachObject(ent);
	alaINode->setScale(1.2, 0.05, 0.5);
	alaINode->translate(-80, 0, 0);
	ent->setMaterialName("Checker");

	ent = mSM->createEntity("cube.mesh");
	alaDNode = mNode->createChildSceneNode();
	alaDNode->attachObject(ent);
	alaDNode->setScale(1.2, 0.05, 0.5);
	alaDNode->translate(80, 0, 0);
	ent->setMaterialName("Checker");


	//FRENTE CABINA
	ent = mSM->createEntity("Barrel.mesh");
	frenteNode = mNode->createChildSceneNode();
	frenteNode->attachObject(ent);
	frenteNode->setScale(1, 1, 1);
	frenteNode->pitch(Ogre::Degree(90));
	frenteNode->setScale(5, 5, 5);
	frenteNode->translate(0, 0, 50);
	ent->setMaterialName("Naranja");

	//SANDOKAN
	ent = mSM->createEntity("ninja.mesh");
	pilotoNode = mNode->createChildSceneNode();
	pilotoNode->attachObject(ent);
	pilotoNode->translate(0, 10, 0);
	pilotoNode->yaw(Ogre::Degree(180));
	pilotoNode->setScale(0.3, 0.3, 0.3);
	ent->setMaterialName("Amarillo");

	//HELICES
	heliceINode = mNode->createChildSceneNode();
	heliceIObj = new AspasMolino(heliceINode, 5, true);
	heliceINode->translate(-90, 0, 25);
	heliceINode->setScale(0.06, 0.06, 0.06);
	heliceDNode = mNode->createChildSceneNode();
	heliceDObj = new AspasMolino(heliceDNode, 5, true);
	heliceDNode->translate(90, 0, 25);
	heliceDNode->setScale(0.06, 0.06, 0.06);

	pointsNode = mNode->createChildSceneNode();
	pointsNode->translate(0, 0, -75);
	bbSet = mSM->createBillboardSet("10points", 1);
	bbSet->setDefaultDimensions(50, 50);
	bbSet->setMaterialName("10points");
	pointsNode->attachObject(bbSet);
	Billboard* bb = bbSet->createBillboard(Vector3(0, 0, 0));

	mPSNode = mNode->createChildSceneNode();
	mPSNode->translate(0, 0, -100);
	pSystem = mSM->createParticleSystem("psSmoke", "Estela");
	pSystem->setEmitting(true);
	mPSNode->attachObject(pSystem);

	explosionSystem = mSM->createParticleSystem("explosion", "Explosion");
	explosionSystem->setEmitting(false);
	mPSNode->attachObject(explosionSystem);

	myTimer = new Timer();
}

bool Avion::keyPressed(const OgreBites::KeyboardEvent& evt)
{
	if (evt.keysym.sym == SDLK_g) {
		heliceDObj->keyPressed(evt, 0);
		heliceIObj->keyPressed(evt, 0);
	}
	else if (evt.keysym.sym == SDLK_r) {
		/*moving = !moving;
		Entity* ent = static_cast<Ogre::Entity*>(alaDNode->getAttachedObject(0));
		ent->setMaterialName("Rojo");
		ent = static_cast<Ogre::Entity*>(alaINode->getAttachedObject(0));
		ent->setMaterialName("Rojo");
		sendEvent(KEY_R, this);*/

		pSystem->setEmitting(false);
		explosionSystem->setEmitting(true);
		moving = false;
		myTimer->reset();

		sendEvent(EXPLOSION, nullptr);

		Ogre::Node::ChildNodeMap m = mNode->getChildren();
		for (int i = 0; i < m.size() - 1; ++i)
			dynamic_cast<SceneNode*>(m[i])->setVisible(false);

	}
	else if (evt.keysym.sym == SDLK_h)
		mNode->getParent()->pitch(Degree(1));
	else if (evt.keysym.sym == SDLK_a)
		mNode->getParent()->yaw(Degree(1));
	else if (evt.keysym.sym == SDLK_d)
		mNode->getParent()->yaw(Degree(-1));

	return true;
}

void Avion::frameRendered(const Ogre::FrameEvent& evt)
{
	Ogre::Real time = evt.timeSinceLastFrame;
	if (moving) {
		/*if (!rotating) {
			unsigned long a = myTimer->getMilliseconds();
			if (a >= 2000) {
				rotationDir = rand() % 2;
				if (rotationDir == 0)
					rotationDir = -1;
				rotating = true;
				myTimer->reset();
			}
			else {
				mNode->getParent()->pitch(Degree(25 * time));
			}
		}
		else {
			if (myTimer->getMilliseconds() >= 250) {
				rotating = false;
				myTimer->reset();
			}
			else {
				mNode->getParent()->yaw(Degree(rotationDir * 50 * time));
			}
		}*/
		mNode->getParent()->yaw(Degree(-50 * time));
	}
	else if (myTimer->getMilliseconds() >= 500)
		explosionSystem->setEmitting(false);

	heliceDObj->frameRendered(evt, 0);
	heliceIObj->frameRendered(evt, 0);
}

Plano::Plano(SceneNode* node, Real w, Real h, int x, int y, string mat, Vector3 trans, Vector3 n) : EntidadIG(node), width(w), height(h),
xSeg(x), ySeg(y), material(mat), translation(trans), normal(n) {
	//PLANO
  //Crear la malla del plano
	MeshManager::getSingleton().createPlane("mPlane1080x800" + material, //nombre
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,    //grupo de recursos utilizados
		Plane(normal, 0),							//orientacion del plano
		w, h,											//dimensiones 1080 x 800
		xSeg, ySeg,												//divisiones de la malla 100 x 80
		true,													//creacion de normales 
		1, 1.0, 1.0,											//coordenadas de texturas con repeticion
		Vector3::UNIT_Z);										//orientacion de la textura
	//Cargamos la malla y inicializamos el nodo
	Ogre::Entity* plane = mSM->createEntity("mPlane1080x800" + material);
	planoNode = mNode->createChildSceneNode();
	planoNode->attachObject(plane);
	plane->setMaterialName(material);
	planoNode->translate(translation);
}

void Plano::receiveEvent(MessageType msg, EntidadIG* entidad)
{
	if (msg == KEY_T && myTimer == nullptr) {
		myTimer = new Timer();
	}
}

void Plano::frameRendered(const Ogre::FrameEvent& evt)
{
	if (myTimer != nullptr) {
		if (myTimer->getMilliseconds() >= 5000) {
			Entity* ent = static_cast<Ogre::Entity*>(planoNode->getAttachedObject(0));
			ent->setMaterialName("Piedras");
		}
	}
}

void Plano::setReflejo(Camera* camRef)
{
	//Crear la malla del plano
	MeshManager::getSingleton().createPlane("reflejo" + material, //nombre
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,    //grupo de recursos utilizados
		Plane(normal, 0),							//orientacion del plano
		width, height,											//dimensiones 1080 x 800
		xSeg, ySeg,												//divisiones de la malla 100 x 80
		true,													//creacion de normales 
		1, 1.0, 1.0,											//coordenadas de texturas con repeticion
		Vector3::UNIT_Z);										//orientacion de la textura

	//Cargamos la malla y inicializamos el nodo
	Ogre::Entity* mirror = mSM->createEntity("reflejo" + material);
	espejoNode = mNode->createChildSceneNode();
	espejoNode->attachObject(mirror);
	mirror->setMaterialName(material);
	espejoNode->translate(translation);

	MovablePlane* mpRef = new MovablePlane(normal, 0);
	espejoNode->attachObject(mpRef);

	camRef->enableReflection(mpRef);
	camRef->enableCustomNearClipPlane(mpRef);

	TexturePtr rttRef = TextureManager::getSingleton().createManual(
		"rttReflejo", // ojo, nombre ?nico -> (*)
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		TEX_TYPE_2D,
		width, // ejemplo
		height, // ejemplo
		0, PF_R8G8B8, TU_RENDERTARGET);

	RenderTexture* renderTexture = rttRef->getBuffer()->getRenderTarget();
	Viewport* vpt = renderTexture->addViewport(camRef);
	vpt->setClearEveryFrame(true);
	vpt->setBackgroundColour(ColourValue::Black);

	TextureUnitState* tu = mirror->getSubEntity(0)->getMaterial()->
		getTechnique(0)->getPass(0)->createTextureUnitState("rttReflejo"); // <- (*)		
	tu->setColourOperation(LBO_ADD); 
	// LBO_ADD / LBO_ALPHA_BLEND / LBO_REPLACE
	tu->setTextureAddressingMode(TextureUnitState::TAM_CLAMP);
	tu->setProjectiveTexturing(true, camRef);

}

void Plano::setEspejo(Camera* camRef)
{
	//Crear la malla del plano
	MeshManager::getSingleton().createPlane("espejo" + material, //nombre
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,    //grupo de recursos utilizados
		Plane(Vector3::NEGATIVE_UNIT_X, -540),							//orientacion del plano
		width, height,											//dimensiones 1080 x 800
		xSeg, ySeg,												//divisiones de la malla 100 x 80
		true,													//creacion de normales 
		1, 1.0, 1.0,											//coordenadas de texturas con repeticion
		Vector3::UNIT_Z);										//orientacion de la textura

	//Cargamos la malla y inicializamos el nodo
	Ogre::Entity* mirror = mSM->createEntity("espejo" + material);
	espejoNode = mNode->createChildSceneNode();
	espejoNode->attachObject(mirror);
	mirror->setMaterialName("nada");
	espejoNode->translate(translation);

	MovablePlane* mpRef = new MovablePlane(Vector3::NEGATIVE_UNIT_X, -540);
	espejoNode->attachObject(mpRef);

	camRef->enableReflection(mpRef);
	camRef->enableCustomNearClipPlane(mpRef);

	TexturePtr rttRef = TextureManager::getSingleton().createManual(
		"rttEspejo", // ojo, nombre ?nico -> (*)
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		TEX_TYPE_2D,
		width, // ejemplo
		height, // ejemplo
		0, PF_R8G8B8, TU_RENDERTARGET);

	RenderTexture* renderTexture = rttRef->getBuffer()->getRenderTarget();
	Viewport* vpt = renderTexture->addViewport(camRef);
	vpt->setClearEveryFrame(true);
	vpt->setBackgroundColour(ColourValue::Black);

	TextureUnitState* tu = mirror->getSubEntity(0)->getMaterial()->
		getTechnique(0)->getPass(0)->createTextureUnitState("rttEspejo"); // <- (*)		
	tu->setColourOperation(LBO_ADD);
	// LBO_ADD / LBO_ALPHA_BLEND / LBO_REPLACE
	tu->setTextureAddressingMode(TextureUnitState::TAM_CLAMP);
	tu->setProjectiveTexturing(true, camRef);
}

Sinbad::Sinbad(SceneNode* node) : EntidadIG(node) {
	//CUERPO
	Ogre::Entity* ent = mSM->createEntity("Sinbad.mesh");
	sinbadNode = mNode->createChildSceneNode();
	sinbadNode->setScale(20, 20, 20);
	sinbadNode->attachObject(ent);

	baseState = ent->getAnimationState("RunBase");

	baseState->setEnabled(true);
	baseState->setLoop(true);

	topState = ent->getAnimationState("RunTop");

	topState->setEnabled(true);
	topState->setLoop(true);

	danceState = ent->getAnimationState("Dance");

	danceState->setEnabled(false);
	danceState->setLoop(true);

	Real duration = 12;
	Real desplazamiento = 50;
	Animation* anim = mSM->createAnimation("animSimbad", duration);
	NodeAnimationTrack* track = anim->createNodeTrack(0);
	track->setAssociatedNode(mNode);
	// Keyframe 0 mirando z positiva
	Vector3 keyFramePos(-360, 100, 800 / 3);
	Vector3 src(0, 0, 1);
	Real durPaso = duration / 6;
	TransformKeyFrame* kf = track->createNodeKeyFrame(0);
	kf->setTranslate(keyFramePos);
	kf->setRotation(src.getRotationTo(Vector3(0, 0, 1)));

	// Keyframe 1 mirando plataforma roja
	kf = track->createNodeKeyFrame(durPaso);
	kf->setTranslate(keyFramePos);
	kf->setRotation(src.getRotationTo(Vector3(1, 0, -1)));

	// Keyframe 2 en plataforma roja
	keyFramePos = Vector3(360, 100, -800 / 3);
	kf = track->createNodeKeyFrame(durPaso * 2);
	kf->setTranslate(keyFramePos);
	kf->setRotation(src.getRotationTo(Vector3(1, 0, -1)));

	// Keyframe 3 mirando plataforma amrilla
	keyFramePos = Vector3(360, 100, -800 / 3);
	kf = track->createNodeKeyFrame(durPaso * 3);
	kf->setTranslate(keyFramePos);
	kf->setRotation(src.getRotationTo(Vector3(-1, 0, 1)));

	// Keyframe 4 en plataforma amarilla
	keyFramePos = Vector3(-360, 100, 800 / 3);
	kf = track->createNodeKeyFrame(durPaso * 4);
	kf->setTranslate(keyFramePos);
	kf->setRotation(src.getRotationTo(Vector3(-1, 0, 1)));

	// Keyframe 5 mirando z positiva
	keyFramePos = Vector3(-360, 100, 800 / 3);
	kf = track->createNodeKeyFrame(durPaso * 5);
	kf->setTranslate(keyFramePos);
	kf->setRotation(src.getRotationTo(Vector3(0, 0, 1)));

	animState = mSM->createAnimationState("animSimbad");
	animState->setLoop(true);
	animState->setEnabled(true);

	myTimer = new Timer();
	AnimationStateSet* aux = ent->getAllAnimationStates();
	auto it = aux->getAnimationStateIterator().begin();
	while (it != aux->getAnimationStateIterator().end())
	{
		auto s = it->first; ++it;
	}
}

void Sinbad::Arma(bool derecha)
{
	if (!dosArmas) {
		Entity* ent = static_cast<Ogre::Entity*>(sinbadNode->getAttachedObject(0));
		Entity* sword = mSM->createEntity("sword", "Sword.mesh");
		if (derecha)
			ent->attachObjectToBone("Handle.R", sword);
		else
			ent->attachObjectToBone("Handle.L", sword);

		derechaEspada = derecha;
	}
}

void Sinbad::Arma() {
	Entity* ent = static_cast<Ogre::Entity*>(sinbadNode->getAttachedObject(0));
	Entity* sword = mSM->createEntity("sword", "Sword.mesh");
	Entity* sword2 = mSM->createEntity("sword2", "Sword.mesh");
	ent->attachObjectToBone("Handle.R", sword);
	ent->attachObjectToBone("Handle.L", sword2);
	dosArmas = true;
}

void Sinbad::cambiaEspada() {
	if (!dosArmas) {
		if (derechaEspada) {
			Entity* ent = static_cast<Ogre::Entity*>(sinbadNode->getAttachedObject(0));
			Entity* sword = static_cast<Ogre::Entity*>(ent->detachObjectFromBone("sword"));
			ent->attachObjectToBone("Handle.L", sword);
		}
		else {
			Entity* ent = static_cast<Ogre::Entity*>(sinbadNode->getAttachedObject(0));
			Entity* sword = static_cast<Ogre::Entity*>(ent->detachObjectFromBone("sword"));
			ent->attachObjectToBone("Handle.R", sword);
		}
		derechaEspada = !derechaEspada;
	}
}

void Sinbad::frameRendered(const Ogre::FrameEvent& evt) {
	if (!dancing) {
		baseState->addTime(evt.timeSinceLastFrame);
		topState->addTime(evt.timeSinceLastFrame);
		animState->addTime(evt.timeSinceLastFrame);
		/*Ogre::Real time = evt.timeSinceLastFrame;
		if (!rotating) {
			unsigned long a = myTimer->getMilliseconds();
			if (a >= 2000) {
				rotationDir = rand() % 2;
				if (rotationDir == 0)
					rotationDir = -1;
				rotating = true;
				myTimer->reset();
			}
		}
		else {
			if (myTimer->getMilliseconds() >= 250) {
				rotating = false;
				myTimer->reset();
			}
			else {
				mNode->getParent()->yaw(Degree(rotationDir * 50 * time));
			}
		}
		mNode->getParent()->pitch(Degree(25 * time));*/
	}
	else {
		danceState->addTime(evt.timeSinceLastFrame);
	}


	if (!alive && myTimer->getMilliseconds() >= 5000)
		sendEvent(DEAD, nullptr);
}

bool Sinbad::keyPressed(const OgreBites::KeyboardEvent& evt)
{
	if (evt.keysym.sym == SDLK_c) {
		dancing = !dancing;
		danceState->setEnabled(dancing);
		baseState->setEnabled(!dancing);
		topState->setEnabled(!dancing);
	}
	else if (evt.keysym.sym == SDLK_t) {
		for (int i = 0; i < appListeners.size(); ++i) {
			appListeners[i]->receiveEvent(KEY_T, nullptr);
		}
	}
	return true;
}

void Sinbad::receiveEvent(MessageType msg, EntidadIG* entidad)
{
	if (msg == EXPLOSION) {
		mNode->pitch(Degree(-90));
		mNode->translate(0, -90, 0);
		alive = false;
		Entity* ent = static_cast<Ogre::Entity*>(sinbadNode->getAttachedObject(0));

		animState->setEnabled(false);

		baseState = ent->getAnimationState("IdleBase");
		baseState->setEnabled(true);
		baseState->setLoop(true);

		topState = ent->getAnimationState("IdleTop");
		topState->setEnabled(true);
		topState->setLoop(true);

		myTimer->reset();
	}
}

Bomba::Bomba(SceneNode* node) : EntidadIG(node)
{
	Ogre::Entity* ent = mSM->createEntity("uv_sphere.mesh");
	barrelNode = mNode->createChildSceneNode();
	barrelNode->attachObject(ent);
	barrelNode->setScale(0.5, 0.5, 0.5);
	ent->setMaterialName("HolesAndLighting");

	Real duration = 3;
	Real desplazamiento = 35;
	Animation* anim = mSM->createAnimation("animVV", duration);
	NodeAnimationTrack* track = anim->createNodeTrack(0);
	track->setAssociatedNode(mNode);
	// Keyframe 0 origen
	Vector3 keyFramePos(0, 0, 0);
	Vector3 src(0, 0, 1);
	Real durPaso = duration / 4;
	TransformKeyFrame* kf = track->createNodeKeyFrame(0);
	kf->setTranslate(keyFramePos);

	// Keyframe 1 arriba 
	keyFramePos = Vector3(0, desplazamiento, 0);
	kf = track->createNodeKeyFrame(durPaso);
	kf->setTranslate(keyFramePos);
	kf->setRotation(src.getRotationTo(Vector3(1, 0, 1)));

	// Keyframe 2 origen
	keyFramePos = Vector3(0, 0, 0);
	kf = track->createNodeKeyFrame(durPaso * 2);
	kf->setTranslate(keyFramePos);

	// Keyframe 3 abajo
	keyFramePos = Vector3(0, -desplazamiento, 0);
	kf = track->createNodeKeyFrame(durPaso * 3);
	kf->setTranslate(keyFramePos);
	kf->setRotation(src.getRotationTo(Vector3(-1, 0, 1)));

	// Keyframe 4 origen
	keyFramePos = Vector3(0, 0, 0);
	kf = track->createNodeKeyFrame(durPaso * 4);
	kf->setTranslate(keyFramePos);

	animState = mSM->createAnimationState("animVV");
	animState->setLoop(true);
	animState->setEnabled(true);

	explosionNode = mNode->createChildSceneNode();
	explosionSystem = mSM->createParticleSystem("bombaExplosion", "ExplosionBomba");
	explosionSystem->setEmitting(false);
	explosionNode->attachObject(explosionSystem);
}

void Bomba::frameRendered(const Ogre::FrameEvent& evt)
{
	if (moving)
		animState->addTime(evt.timeSinceLastFrame);
}

void Bomba::receiveEvent(MessageType msg, EntidadIG* entidad)
{
	if (msg == KEY_T)
		moving = false;
	else if (msg == DEAD) {
		explosionSystem->setEmitting(true);
		moving = false;
		animState->setTimePosition(0);
		barrelNode->setVisible(false);
	}
}
