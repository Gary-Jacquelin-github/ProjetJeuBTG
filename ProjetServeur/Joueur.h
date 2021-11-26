#pragma once
#include <string>
#include <cstring>
#include <chrono>
#include <ctime>    
#include <sstream>

#ifndef joueur_h
#define joueur_h 

class Joueur
{
	public:
		Joueur(int socket, std::string pseudo);
		Joueur(int socket);
		std::string prenom;
		std::string nom;
		std::string pseudo;
		int socket;
		int envoyerMessage(Joueur destinataire, std::string message);
		int recevoirMessage(Joueur envoyeur, std::string message);

};

#endif


