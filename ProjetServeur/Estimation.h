#pragma once
#include <string>

#include "Joueur.h"

using namespace std;

class Estimation
{
	public:
		Estimation();
		Estimation(string commande, Joueur joueur);
		bool dodo = false;
		bool kalzone = false;
		short dice = 0;
		int nbDice = 0;
		Joueur joueur;
		string getMessage();
		bool isCorrect(Estimation oldEstimation);
};

