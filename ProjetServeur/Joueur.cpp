#include "Joueur.h"
#include "Time.h"


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


/// <summary>
/// C'est pas du code mmort, c'est une evolution possible
/// </summary>
/// <param name="Destinataire"></param>
/// <param name="message"></param>
/// <returns></returns>
int Joueur::envoyerMessage(Joueur Destinataire, std::string message) {
	return 0;
}

/// <summary>
/// C'est pas du code mmort, c'est une evolution possible
/// </summary>
/// <param name="Destinataire"></param>
/// <param name="message"></param>
/// <returns></returns>
int Joueur::recevoirMessage(Joueur Destinataire, std::string message) {
	return 0;
}

/// <summary>
/// lance les dés du joueur
/// </summary>
void Joueur::rollDices() {
	//vide les dés
	dices.clear();
	//nouvelle seed pour avoir du "vrai" aleatoire
	srand(time(NULL) + socket);
	for (short i = 0; i < score; i++)
		dices.push_back((rand() % (6) + 1));
}

/// <summary>
/// Cherche le nombre de dés demandés
/// </summary>
/// <param name="dice"></param>
/// <returns></returns>
int Joueur::countDiceOccurences(short dice) {
	int res = 0;
	//
	for (short x : this->dices)
		if (x == 1 || x == dice)
			res++;
	
	return res;
}
