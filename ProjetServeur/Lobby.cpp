#include "Lobby.h"

#include <sstream>
#include <iostream>
#include <mutex>

mutex creationMutex;

/// <summary>
/// constructeur par defaut
/// </summary>
Lobby::Lobby() {
}


/// <summary>
/// Fonction principale du lobby qui permet d'ecouter les commandes du joueur et d'agir en consequence
/// </summary>
/// <param name="socket"></param>
void Lobby::listenPlayer(SOCKET socket) {

	string message = "Ask pseudo\r\n";
	if (send(socket, message.c_str(), message.length(), 0) < 0) {
		perror("Error send: ");
	}

	string pseudo = Lobby::listenCommands(socket);

	Joueur monJoueur = Joueur(socket, pseudo);

	message = "Ask action Exit/play/create (name)\r\n";
	if (send(socket, message.c_str(), message.length(), 0) < 0) {
		perror("Error send: ");
	}

	int finDeSession = -1;

	//On attend que le client mette fin à la conversation
	while (finDeSession < 0) {

		string stringRecu = Lobby::listenCommands(socket);

		int findPlay = stringRecu.find("play ");
		if (findPlay >= 0) {
			if (send(socket, "yo\r\n", 4, 0) == -1)
				perror("Erreur d'envoi: \r\n");

			string nom = stringRecu.substr(5, stringRecu.length());
			Lobby::jumpInSalon(monJoueur, nom);
		}

		int findCreate = stringRecu.find("create ");
		if (findCreate >= 0) {
			stringRecu = stringRecu.substr(7, stringRecu.length());
			string nom = stringRecu.substr(0, stringRecu.find(" "));
			int nbJoueur = stoi(stringRecu.substr(nom.length(), stringRecu.length()));
			Lobby::creationSalon(nom, nbJoueur);
			if (send(socket, "creation\r\n", 10, 0) == -1)
				perror("Erreur d'envoi: \r\n");
			Lobby::jumpInSalon(monJoueur, nom);
		}

		if (stringRecu == "Exit") {
			finDeSession = 1;
			cout << "adios \r\n";
		}

	}
	closesocket(socket);
	return;
}

/// <summary>
/// On prend toute la ligne
/// </summary>
/// <param name="socket"></param>
/// <returns></returns>
string Lobby::listenCommands(SOCKET socket) {
	char tmp[6000];
	int nbBytes = 0;
	string stringRecu = "";
	int test = -1;
	while (test < 0) {
		if ((nbBytes = recv(socket, tmp, 6000, 0)) == -1) {
			perror("Error recu: ");
			return "exit";
		}
		tmp[nbBytes] = '\0';
		stringRecu += tmp;
		test = stringRecu.find("\r\n");
	}

	//On enlève l'entrée
	stringRecu = stringRecu.substr(0, stringRecu.length() - 2);
	return stringRecu;
}

/// <summary>
/// On creer notre salon
/// </summary>
/// <param name="nom"></param>
/// <param name="nbJoueur"></param>
void Lobby::creationSalon(string nom, int nbJoueur) {
	//On utilise notre mutex pour ne pas creer deux salons ayant le même nom
	
	creationMutex.lock();
	if (Salon::mapNomSalonSalon.count(nom) == 0) {
		Salon monSalon = Salon(nom, nbJoueur);
		Salon::mapNomSalonSalon[nom] = monSalon;
	}
	creationMutex.unlock();
}

/// <summary>
/// On va dans le salon
/// </summary>
/// <param name="joueur"></param>
/// <param name="nom"></param>
void Lobby::jumpInSalon(Joueur joueur, string nom) {
	//Pas cool pour la mémoire mais bon c++ quoi...
	Salon newSalon = Salon::mapNomSalonSalon[nom].copySalon();
	newSalon.communicationMain(joueur);
}
