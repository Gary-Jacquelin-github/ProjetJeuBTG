#include "Joueur.h"


Joueur::Joueur(int socket, std::string pseudo) {
	Joueur::socket = socket;
	Joueur::pseudo = pseudo;
}

Joueur::Joueur(int socket) {
	Joueur::socket = socket;
}


int Joueur::envoyerMessage(Joueur Destinataire, std::string message) {
	return 0;
}

int Joueur::recevoirMessage(Joueur Destinataire, std::string message) {
	return 0;
}
