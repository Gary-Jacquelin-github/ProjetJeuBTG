#include "Joueur.h"

class Joueur {
	std::string Prenom;
	std::string Nom;
	std::string Pseudo;
	int socket;

	int envoyerMessage(Joueur Destinataire, std::string message) {
		return 0;
	}

	int recevoirMessage(Joueur Destinataire, std::string message) {
		return 0;
	}
};