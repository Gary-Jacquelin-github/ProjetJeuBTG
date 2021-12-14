#include "Joueur.h"

/// <summary>
/// constructeur par defaut
/// </summary>
Joueur::Joueur() {
	Joueur::score = 5;
}

/// <summary>
/// On sait jamais pour de futurs implementation
/// </summary>
/// <param name="socket"></param>
Joueur::Joueur(SOCKET socket) {
	Joueur::socket = socket;
	Joueur::score = 5;
}

/// <summary>
/// Constructeur principal
/// </summary>
/// <param name="socket"></param>
/// <param name="pseudo"></param>
Joueur::Joueur(SOCKET socket, std::string pseudo) {
	Joueur::socket = socket;
	Joueur::pseudo = pseudo;
	Joueur::score = 5;
}



int Joueur::envoyerMessage(Joueur Destinataire, std::string message) {
	return 0;
}

int Joueur::recevoirMessage(Joueur Destinataire, std::string message) {
	return 0;
}

void Joueur::rollDices() {
	dices.clear();
	for (short i = 0; i < score; i++)
		dices.push_back((rand() % (6) + 1));
}