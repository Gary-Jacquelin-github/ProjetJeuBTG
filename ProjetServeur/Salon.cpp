#include "Salon.h"

#include <sstream>
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#pragma comment(lib, "ws2_32.lib")

#include "Lobby.h"

using namespace std;

map<string, int> Salon::mapSalonNbJoueurs = map<string, int>();

Salon::Salon(string nom, int nbJoueur) {
	nom = nom;
	nbJoueur = nbJoueur;
}

void Salon::communicationMain(SOCKET socket) {

	string message = "Bien le bonjour joueur\r\n";

	if (send(socket, message.c_str(), message.length(), 0) < 0) {
		perror("Error send: ");
	}
	/*
	if (JoueurManquant == 0 )
		message = "Tous les joueurs sont présents \r\n";
	else
		message = "Il manque " + to_string(JoueurManquant) + " joueurs \r\n";
		*/
	if (send(socket, message.c_str(), strlen(message.c_str()), 0) < 0) {
		perror("Error send: ");
	}

	char tmp[6000];
	int nbBytes = 0;
	string stringRecu;
	int finDeSession = -1;
	while (finDeSession < 0) {
		stringRecu = "";
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
		stringRecu = stringRecu.substr(0, stringRecu.length()-2);

		if (stringRecu == "Jouer") {
			if(send(socket , "yo", 2, 0) == -1)
				perror("Erreur d'envoi: \r\n");
		}
		/*
		if (stringRecu == "lobby") {
			Lobby monLobby;
			monLobby.creationLobby(2);
			if (send(socket, "yo", 2, 0) == -1)
				perror("Erreur d'envoi: \r\n");
		}
		*/
		if (stringRecu == "Exit") {
			finDeSession = 1;
			cout << "adios \r\n";
		}

	}
	closesocket(socket);
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