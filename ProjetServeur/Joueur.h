#pragma once
#include <string.h>
#include <string>
#include <cstring>
#include <datetimeapi.h>
#include <chrono>
#include <ctime>    
#include <sstream>

class Joueur
{
	public :
		std::string Prenom;
		std::string Nom;
		std::string Pseudo;
		int socket;
		int envoyerMessage(Joueur Destinataire, std::string message);
		int recevoirMessage(Joueur Destinataire, std::string message);
		
};

