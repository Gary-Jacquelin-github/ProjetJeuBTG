#pragma once
#include <string>
#include <winsock2.h>

#include "Joueur.h"
#include "Salon.h"

using namespace std;

class Lobby
{
	public:
		Lobby();
		static string listenCommands(SOCKET socket);
		void static listenPlayer(SOCKET socket);
		void static creationSalon(string nom, int nbJoueur);
		void static jumpInSalon(Joueur joueur, string nom);

};

