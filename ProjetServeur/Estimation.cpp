#include "Estimation.h"

Estimation::Estimation() {
}

Estimation::Estimation(string commande, Joueur joueur) {
	this->joueur = joueur;
	int find = commande.find("Dodo"); // c++ est trop nul pour tout faire sans utiliser de variable
	this->dodo = find >= 0;
	find = commande.find("Kalzone");
	this->kalzone = find >= 0;
	if (!dodo && !kalzone) {
		// on prend les estimations
		//obligé de les mettre dans ses variable pcq c++ c'est nul
		int separation = commande.find(" ");
		int tailleTotal = commande.size();
		this->dice = stoi(commande.substr(0, (separation) ));
		this->nbDice = stoi(commande.substr(separation, (tailleTotal-1) ));
		
	}
}

string Estimation::getMessage() {
	return  "GiveEstimate " + this->joueur.pseudo +  (this->dodo ? " Dodo" : (this->kalzone ? " Kalzone" : (" " + to_string(this->dice) + " " + to_string(this->nbDice)))) + " \r\n";
}

bool Estimation::isCorrect(Estimation oldEstimation) {
	// on ne peux pas faire de dodo si c'est la premiere estimation du tour
	if ( ((oldEstimation.dodo || oldEstimation.kalzone) && oldEstimation.nbDice != 0))
		return true;

	//on check le cas des 1
	if (oldEstimation.dice == 1 && this->dice != 1)
		return this->nbDice > (oldEstimation.nbDice * 2);

	if (oldEstimation.dice != 1 && this->dice == 1)
		return (this->nbDice * 2) > oldEstimation.nbDice;

	// le cas general
	return this->nbDice > oldEstimation.nbDice;
}