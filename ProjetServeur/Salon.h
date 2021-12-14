#pragma once

#include <thread>
#include <winsock2.h>
#include <string>
#include <map>
#include <list>

#include "Joueur.h"

using namespace std;

class Salon{
	
	public:
		static map<string, int> mapSalonNbJoueurs;
		static map<string, Salon> mapNomSalonSalon;
		static map<string, Joueur> mapNomSalonJoueurEnCours;

		Salon();
		Salon(string nom, int nbJoueur);

		string nomSalon;
		list<Joueur> Joueurs;
		int nbJoueurMax;
		void communicationMain(Joueur joueur);

	private:
		string lastMessage;
		bool needRoll;
		bool win;
		bool loose;
		int getNombreJoueurSalon();
		void incrNombreJoueurSalon();
		void decrNombreJoueurSalon();
		void sendMessageAll(string message);
		void attenteDebutPartie();

};