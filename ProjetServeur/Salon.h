#pragma once

#include <thread>
#include <winsock2.h>
#include <string>
#include <map>

using namespace std;

class Salon{
	
	public:
		Salon(string nom, int nbJoueur);
		static map<string, int> mapSalonNbJoueurs;
		string nomSalon;
		map<SOCKET, int> socketJoueurs;
		static void communicationMain(SOCKET socket);
	private:
		int getNombreJoueurSalon();
		void incrNombreJoueurSalon();
		void decrNombreJoueurSalon();
};