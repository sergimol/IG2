#include "EntidadIG.h"

EntidadIG::EntidadIG(SceneNode* node)
{
	//Esto del vector igual esta mal
	appListeners = std::vector<EntidadIG*>(0, nullptr);
	//O igual no

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
