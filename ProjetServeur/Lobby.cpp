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

	message = "Ask action Exit/AskAvailableSalon/Play/Create (name)\r\n";
	Lobby::sendMessage(monJoueur, message);

	int finDeSession = -1;

	//On attend que le client mette fin � la conversation
	while (finDeSession < 0) {
		
		string stringRecu = Lobby::listenCommands(socket);

		int findAsk = stringRecu.find("AskAvailableSalon");
		if (findAsk >= 0) {
			message = Salon::getSalonsInfos();
			Lobby::sendMessage(monJoueur, message);
		}

		int findPlay = stringRecu.find("Play ");
		if (findPlay >= 0) {
			string nom = stringRecu.substr(5, stringRecu.length());
			Lobby::jumpInSalon(monJoueur, nom);
		}

		int findCreate = stringRecu.find("Create ");
		if (findCreate >= 0) {
			stringRecu = stringRecu.substr(7, stringRecu.length());
			string nom = stringRecu.substr(0, stringRecu.find(" "));
			int nbJoueur = stoi(stringRecu.substr(nom.length(), stringRecu.length()));
			Lobby::creationSalon(nom, nbJoueur);
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

	//On enl�ve l'entr�e
	stringRecu = stringRecu.substr(0, stringRecu.length() - 2);
	return stringRecu;
}

/// <summary>
/// envoie un message au joueur
/// </summary>
/// <param name="joueur"></param>
/// <param name="message"></param>
void Lobby::sendMessage(Joueur joueur, string message) {
	if (send(joueur.socket, message.c_str(), message.length(), 0) < 0) {
		perror("Error send: ");
	}
}

/// <summary>
/// On creer notre salon
/// </summary>
/// <param name="nom"></param>
/// <param name="nbJoueur"></param>
void Lobby::creationSalon(string nom, int nbJoueur) {
	//On utilise notre mutex pour ne pas creer deux salons ayant le m�me nom
	
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
	//Pas cool pour la m�moire mais bon c++ quoi...
	Salon newSalon = Salon::mapNomSalonSalon[nom].copySalon();
	newSalon.communicationMain(joueur);
	string message = "AskRestart";
	bool wantRestart = true;
	while (wantRestart) {
		Lobby::sendMessage(joueur, message);
		string stringRecu = Lobby::listenCommands(joueur.socket);
		if (stringRecu == "Restart") {
			newSalon.communicationMain(joueur);
		}
		else
		{
			wantRestart = false;
		}
	}
}
