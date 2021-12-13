#pragma once
#include <string>
#include <winsock2.h>

using namespace std;

class Lobby
{
	public:
		Lobby();
		void static listenPlayer(SOCKET socket);
		void static creationSalon(string nom, int nbJoueur);

};

