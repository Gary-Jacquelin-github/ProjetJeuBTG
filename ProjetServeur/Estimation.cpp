#include "Estimation.h"

Estimation::Estimation() {
	this->dice = 1;
	this->nbDice = 1;
	this->dodo = false;
	this->kalzone = false;
}

Estimation::Estimation(string commande, Joueur joueur) {
	this->joueur = joueur;
	size_t find = commande.find("Dodo"); // c++ est trop nul pour tout faire sans utiliser de variable
	this->dodo = find >= 0;
	find = commande.find("Kalzone");
	this->kalzone = find >= 0;
	if (!dodo && !kalzone) {
		// on prend les estimations
		//obligé de les mettre dans ses variable pcq c++ c'est nul
		size_t separation = commande.find(" ");
		size_t tailleTotal = commande.size();
		this->dice = stoi(commande.substr(0, (separation) ));
		this->nbDice = stoi(commande.substr(separation, (tailleTotal-1) ));
		
	}
}

string Estimation::getMessage() {
	return  "GiveEstimate " + this->joueur.pseudo +  (this->dodo ? " Dodo" : (this->kalzone ? " Kalzone" : (" " + to_string(this->dice) + " " + to_string(this->nbDice)))) + " \r\n";
}

bool Estimation::isCorrect(Estimation oldEstimation) {
	//si c'est la premiere estimation du tour elle est forcement bonne
	if (oldEstimation.dodo || oldEstimation.kalzone || nbDice == 0)
		return true;

	//on check le cas des 1
	if (oldEstimation.dice == 1 && this->dice != 1)
		return this->nbDice > (oldEstimation.nbDice * 2);

	if (oldEstimation.dice != 1 && this->dice == 1)
		return (this->nbDice * 2) > oldEstimation.nbDice;

	return this->nbDice > oldEstimation.nbDice;
}