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
	return this->joueur.pseudo + " a annoncé " + (this->dodo ? "un Dodo! " : (this->kalzone ? "une Kalzone! " : ( "le " + to_string(this->dice) + " au moins " + to_string(this->nbDice) + " fois!"))) + " \r\n";
}