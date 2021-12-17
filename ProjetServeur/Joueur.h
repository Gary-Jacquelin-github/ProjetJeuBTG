#pragma once
#include <string>
#include <cstring>
#include <chrono>
#include <ctime>    
#include <sstream>
#include <winsock2.h>
#include <list>

#ifndef joueur_h
#define joueur_h 

using namespace std;

class Joueur
{
	public:
		Joueur();
		Joueur(SOCKET socket);
		Joueur(SOCKET socket, std::string pseudo);
		string prenom;
		string nom;
		string pseudo;
		int score;
		SOCKET socket;
		list<short> dices;
		int envoyerMessage(Joueur destinataire, std::string message);
		int recevoirMessage(Joueur envoyeur, std::string message);
		void rollDices();
		int countDiceOccurences(short dice);
};

#endif


