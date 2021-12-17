#pragma once
#include <string>

#include "Joueur.h"

using namespace std;

class Estimation
{
	public:
		Estimation();
		Estimation(string commande, Joueur joueur);
		bool dodo;
		bool kalzone;
		short dice;
		int nbDice;
		Joueur joueur;
		string getMessage();
};

