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
	//Pues limpio el vector 
	for (EntidadIG* a : appListeners)
		delete a;
	appListeners.clear();
}


void EntidadIG::addListener(EntidadIG* entidad)
{
	appListeners.push_back(entidad);
}
