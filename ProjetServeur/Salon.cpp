#include "Salon.h"
#include "Lobby.h"

#include <sstream>
#include <iostream>
#include <mutex>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#pragma comment(lib, "ws2_32.lib")


using namespace std;

map<string, int> Salon::mapSalonNbJoueurs = map<string, int>();
map<string, Salon> Salon::mapNomSalonSalon = map<string, Salon>();
map<string, Joueur> Salon::mapNomSalonJoueurEnCours = map<string, Joueur>();

mutex newJoueurMutex;

Salon::Salon() {
}


Salon::Salon(string nom, int nbJoueur) {
	Salon::nomSalon = nom;
	Salon::nbJoueurMax = nbJoueur;
}

void Salon::communicationMain(Joueur joueur) {
	//gestion du surnombre de joueurs
	newJoueurMutex.lock();
	int taille = Salon::Joueurs.size();
	if (Salon::Joueurs.size() >= Salon::nbJoueurMax) {
		newJoueurMutex.unlock();
		string message = "Nous sommes désolé, ce salon est plein \r\n";
		SOCKET socket = joueur.socket;

		if (send(socket, message.c_str(), message.length(), 0) < 0) {
			perror("Error send: ");
		}
	}

	//On ajoute le joueur a notre liste de joueurs
	Salon::Joueurs.push_back(joueur);
	newJoueurMutex.unlock();
	Salon::incrNombreJoueurSalon();
	//On acceuille le joueur
	string message = "Bien le bonjour " + joueur.pseudo + " \r\n";
	SOCKET socket = joueur.socket;
	if (send(socket, message.c_str(), message.length(), 0) < 0) {
		perror("Error send: ");
	}
	Salon::sendMessageAll(joueur.pseudo + " a rejoins le salon\r\n");
	
	//On regarde si on commence la commence la partie et notifies les joueurs du salon
	if (Salon::Joueurs.size() >= Salon::nbJoueurMax) {
		message = "Tous les joueurs sont présents \r\n";
		mapNomSalonJoueurEnCours[this->nomSalon] = Salon::Joueurs.front();
	}
	else
		message = "Il manque " + to_string( Salon::nbJoueurMax - Salon::Joueurs.size() ) + " joueurs \r\n";
			
	Salon::sendMessageAll(message);

	//On attend le debut de la partie
	Salon::attenteDebutPartie();
	needRoll = true;

	//On commence la partie
	string stringRecu;
	int finDeSession = -1;
	while (finDeSession < 0) {
		//on regarde si il y a besoin de relancer les des
		if (needRoll)
			joueur.rollDices();
		stringRecu = Lobby::listenCommands(socket);


		if (stringRecu == "Jouer") {
			if (send(socket, "yo", 2, 0) == -1)
				perror("Erreur d'envoi: \r\n"); 
		}

		if (stringRecu == "Exit") {
			finDeSession = 1;
			cout << "adios \r\n";
		}

	}
	closesocket(socket);
	Salon::Joueurs.remove(joueur);
	Salon::decrNombreJoueurSalon();
	return;	
}

int Salon::getNombreJoueurSalon() {
	return Salon::mapSalonNbJoueurs[this->nomSalon];
}

void Salon::incrNombreJoueurSalon() {
	Salon::mapSalonNbJoueurs[this->nomSalon] = Salon::mapSalonNbJoueurs[this->nomSalon]++;
}

void Salon::decrNombreJoueurSalon(){
	Salon::mapSalonNbJoueurs[this->nomSalon] = Salon::mapSalonNbJoueurs[this->nomSalon]--;
}

void Salon::sendMessageAll(string message) {
	for (auto const& x : Salon::Joueurs)
	{
		if (send(x.socket, message.c_str(), message.length(), 0) == -1)
			perror("Erreur d'envoi \r\n");
	}
}

/// <summary>
/// On attends le début de la partie
/// </summary>
void Salon::attenteDebutPartie() {
	while (Salon::mapSalonNbJoueurs.count(this->nomSalon) == 0) {
		this_thread::sleep_for(250ms);
	}
}