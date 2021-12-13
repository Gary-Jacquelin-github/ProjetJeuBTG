#include "Lobby.h"
#include "Salon.h"

#include <sstream>
#include <iostream>
#include <mutex>

mutex creationMutex;

Lobby::Lobby() {

}

void Lobby::listenPlayer(SOCKET socket) {
	string message = "Ask action Exit/play/create (name)\r\n";

	if (send(socket, message.c_str(), message.length(), 0) < 0) {
		perror("Error send: ");
	}

	char tmp[6000];
	int nbBytes = 0;
	string stringRecu;
	int finDeSession = -1;
	//On attend que le client mette fin à la conversation
	while (finDeSession < 0) {
		stringRecu = "";

		//On prend toute la ligne
		int test = stringRecu.find("\r\n");		
		while (test < 0) {
			if ((nbBytes = recv(socket, tmp, 6000, 0)) == -1) {
				perror("Error recu: ");
			}
			tmp[nbBytes] = '\0';
			stringRecu += tmp;
			test = stringRecu.find("\r\n");
			int finDeSession = stringRecu.find("Exit");
		}

		//On enlève l'entré
		stringRecu = stringRecu.substr(0, stringRecu.length() - 2);

		if (stringRecu == "play") {
			if (send(socket, "yo\r\n", 4, 0) == -1)
				perror("Erreur d'envoi: \r\n");
		}

		int findCreate = stringRecu.find("create ");
		if (findCreate >= 0) {
			string nom = stringRecu.substr(stringRecu.find("create "), stringRecu.length());
			Lobby::creationSalon(nom, 6);
			if (send(socket, "creation\r\n", 10, 0) == -1)
				perror("Erreur d'envoi: \r\n");
		}

		if (stringRecu == "Exit") {
			finDeSession = 1;
			cout << "adios \r\n";
		}

	}
	closesocket(socket);
	return;
}


void Lobby::creationSalon(string nom, int nbJoueur) {
	//On utilise notre mutex pour ne pas creer deux salons ayant le même nom
	creationMutex.lock();
	if (Salon::mapSalonNbJoueurs.count(nom) == 0) {
		Salon::mapSalonNbJoueurs[nom] = 0;
		Salon salon = Salon(nom, nbJoueur);
	}
	creationMutex.unlock();
}

