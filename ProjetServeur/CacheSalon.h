#pragma once
#include <string>
#include <list>

#include "Joueur.h"
#include "Estimation.h"

using namespace std;

class CacheSalon
{
	public:
		bool isEnCours = false;
		int nbJoueurs = 0;
		int tourEnCours = 1;
		Joueur joueurEnCours;
		Estimation lastEstimation;
		list<Joueur> Joueurs;

};

