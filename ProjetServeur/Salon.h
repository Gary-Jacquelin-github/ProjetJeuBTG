#pragma once

#include <thread>
#include <winsock2.h>
#include <string>
#include <map>
#include <list>

#include "CacheSalon.h"
#include "Joueur.h"
#include "Estimation.h"

using namespace std;

class Salon{
	
	public:

		static map<string, Salon> mapNomSalonSalon;
		static map<string, CacheSalon> mapSalonCacheSalon;

		Salon();
		Salon(string nom, int nbJoueur);

		string nomSalon;
		int nbJoueurMax;
		void communicationMain(Joueur joueur);
		Salon copySalon();

	private:
		string lastMessage;
		bool needRoll;
		bool win;
		bool loose;
		int tourEnCours;
		int getNombreJoueurSalon();
		void incrNombreJoueurSalon();
		void decrNombreJoueurSalon();
		void sendMessage(Joueur joueur, string message);
		void sendMessageAll(string message);
		void sendRoll(Joueur joueur);
		void attenteDebutPartie();
		void joueurSuivant();
		bool attenteTour(Joueur joueur);
		void newTurn();
		void checkScore(Estimation estimationActuelle);
		bool changeScore(Joueur joueur, int increment);
		void synchroRoll(Joueur joueur);
		Joueur checkGagnant();

};