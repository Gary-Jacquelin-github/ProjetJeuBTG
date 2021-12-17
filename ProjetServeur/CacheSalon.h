#pragma once
#include <string>
#include <list>

#include "Joueur.h"
#include "Estimation.h"

using namespace std;

class CacheSalon
{
	public:
		bool isEnCours;
		int nbJoueurs;
		int tourEnCours;
		Joueur joueurEnCours;
		Estimation lastEstimation;
		list<Joueur> Joueurs;

};

