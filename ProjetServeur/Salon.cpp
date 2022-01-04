#include "Salon.h"
#include "Lobby.h"

#include <sstream>
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#pragma comment(lib, "ws2_32.lib")


using namespace std;

map<string, Salon> Salon::mapNomSalonSalon = map<string, Salon>();
map<string, CacheSalon>  Salon::mapSalonCacheSalon = map<string, CacheSalon>();

mutex newJoueurMutex;
mutex synchroList;

/// <summary>
/// constructeur par defaut
/// </summary>
Salon::Salon() {
	Salon::nbJoueurMax = 6;
}


/// <summary>
/// constructeur utilisé
/// </summary>
Salon::Salon(string nom, int nbJoueur) {
	Salon::nomSalon = nom;
	Salon::nbJoueurMax = nbJoueur;
}

Salon Salon::copySalon() {
	return Salon(this->nomSalon, this->nbJoueurMax);
}

/// <summary>
/// boucle principale du salon de jeu
/// </summary>
/// <param name="joueur"></param>
void Salon::communicationMain(Joueur joueur) {
	//gestion du surnombre de joueurs
	newJoueurMutex.lock();

	if (Salon::mapSalonCacheSalon[this->nomSalon].Joueurs.size() >= Salon::nbJoueurMax) {
		newJoueurMutex.unlock();
		string message = "Nous sommes désolé, ce salon est plein \r\n";
		SOCKET socket = joueur.socket;
		Salon::sendMessage(joueur, message);
		return;
	}	

	//On ajoute le joueur a notre liste de joueurs
	Salon::mapSalonCacheSalon[this->nomSalon].Joueurs.push_back(joueur);
	newJoueurMutex.unlock();
	Salon::incrNombreJoueurSalon();
	//On acceuille le joueur
	string message = "Bien le bonjour " + joueur.pseudo + " \r\n";
	SOCKET socket = joueur.socket;
	Salon::sendMessage(joueur, message);

	Salon::sendMessageAll(joueur.pseudo + " a rejoins le salon\r\n");
	
	//On regarde si on commence la commence la partie et notifies les joueurs du salon
	if (Salon::mapSalonCacheSalon[this->nomSalon].Joueurs.size() >= Salon::nbJoueurMax) {
		Salon::mapSalonCacheSalon[this->nomSalon].joueurEnCours = Salon::mapSalonCacheSalon[this->nomSalon].Joueurs.front();
		Salon::mapSalonCacheSalon[this->nomSalon].isEnCours = true;
		Salon::mapSalonCacheSalon[this->nomSalon].tourEnCours = 1;
		message = "Players";
		for (auto it = Salon::mapSalonCacheSalon[this->nomSalon].Joueurs.begin(); it != Salon::mapSalonCacheSalon[this->nomSalon].Joueurs.end(); it++) {
			message += " " + it->pseudo;
		}
		message += "\r\n";
	}
	else
		message = "Il manque " + to_string( Salon::nbJoueurMax - Salon::mapSalonCacheSalon[this->nomSalon].Joueurs.size() ) + " joueurs \r\n";
			
	Salon::sendMessageAll(message);

	//On attend le debut de la partie
	Salon::attenteDebutPartie();
	this->needRoll = true;

	//On commence la partie
	string stringRecu;
	int finDeSession = -1;
	size_t trouveCommande = -1;
	string commande;
	while (finDeSession < 0 && Salon::mapSalonCacheSalon[this->nomSalon].isEnCours) {
		//on regarde si il y a besoin de relancer les des
		if (this->needRoll) {
			joueur.rollDices();
			Salon::sendRoll(joueur);
			Salon::synchroRoll(joueur);
			this->needRoll = false;
		}
		//on attend son tour, si c'est une fin de tour on recommence
		if (!Salon::attenteTour(joueur))
			continue;

		//Recoit commande
		stringRecu = Lobby::listenCommands(socket);

		commande = "AnsEstimate ";
		trouveCommande = stringRecu.find(commande);
		if (trouveCommande >= 0) {
			//obligé de les mettre dans ses variable pcq c++ c'est nul
			size_t tailleCommande = commande.size();
			size_t tailleRecu = stringRecu.size() - 1;
			string commande = stringRecu.substr(tailleCommande, tailleRecu);
			Estimation est = Estimation::Estimation(commande, joueur);
			Salon::sendMessageAll(est.getMessage());
			if (est.dodo || est.kalzone) {
				Salon::checkScore(est);
				Salon::newTurn();
			}
			else {
				Salon::mapSalonCacheSalon[this->nomSalon].lastEstimation = est;
				Salon::joueurSuivant();
			}

		}

		//Il peux partir à la fin de son tour
		if (stringRecu == "Exit") {
			finDeSession = 1;
			Salon::sendMessage(joueur, "adios \r\n");
		}
	}
	//On enlève le joueur de la liste
	for (auto it = Salon::mapSalonCacheSalon[this->nomSalon].Joueurs.begin(); it != Salon::mapSalonCacheSalon[this->nomSalon].Joueurs.end();) {
		if (it->socket == joueur.socket) {
			it = Salon::mapSalonCacheSalon[this->nomSalon].Joueurs.erase(it);
		}
		else {
			++it;
		}
	}
	Salon::decrNombreJoueurSalon();
	return;	
}

/// <summary>
/// tout est dans le titre
/// </summary>
int Salon::getNombreJoueurSalon() {
	return Salon::mapSalonCacheSalon[this->nomSalon].nbJoueurs;
}

/// <summary>
/// tout est dans le titre
/// </summary>
void Salon::incrNombreJoueurSalon() {
	Salon::mapSalonCacheSalon[this->nomSalon].nbJoueurs = Salon::mapSalonCacheSalon[this->nomSalon].nbJoueurs++;
}

/// <summary>
/// tout est dans le titre
/// </summary>
void Salon::decrNombreJoueurSalon(){
	Salon::mapSalonCacheSalon[this->nomSalon].nbJoueurs = Salon::mapSalonCacheSalon[this->nomSalon].nbJoueurs--;
}

/// <summary>
/// envoie le jet de dé du joueur
/// </summary>
/// <param name="joueur"></param>
void Salon::sendRoll(Joueur joueur) {
	string res = "SelfRolls";
	for (short const& x : joueur.dices)
		res += " " + to_string(x);

	res += "\r\n";
	Salon::sendMessage(joueur, res);
}

/// <summary>
/// envoie un message au joueur
/// </summary>
/// <param name="joueur"></param>
/// <param name="message"></param>
void Salon::sendMessage(Joueur joueur, string message) {
	if (send(joueur.socket, message.c_str(), message.length(), 0) < 0) {
		perror("Error send: ");
	}
}

/// <summary>
/// Envoie un message à tous les joueurs du salon
/// </summary>
/// <param name="message"></param>
void Salon::sendMessageAll(string message) {
	for (auto const& x : Salon::mapSalonCacheSalon[this->nomSalon].Joueurs)
	{
		Salon::sendMessage(x, message);
	}
}

/// <summary>
/// On dit à tous le monde que c'est la fin du tour
/// </summary>
void Salon::newTurn() {
	Salon::mapSalonCacheSalon[this->nomSalon].tourEnCours++;
	this->needRoll = true;
}

/// <summary>
/// On attend le début de la partie
/// </summary>
void Salon::attenteDebutPartie() {
	while (!Salon::mapSalonCacheSalon[this->nomSalon].isEnCours) {
		this_thread::sleep_for(1000ms);
	}
	this->tourEnCours = 1;
}

/// <summary>
///  On attend son tour
/// </summary>
/// <param name="joueur"></param>
/// <returns>true si c'est reellement son tour</returns>
bool Salon::attenteTour(Joueur joueur) {
	//On attend la fin du tour de jeu ou que ce soit au joueur de jouer.
	while (Salon::mapSalonCacheSalon[this->nomSalon].joueurEnCours.socket != joueur.socket && Salon::mapSalonCacheSalon[this->nomSalon].tourEnCours == this->tourEnCours) {
		this_thread::sleep_for(100ms);
	}

	//On est sortie pcq c'est le tour du joueur
	if (Salon::mapSalonCacheSalon[this->nomSalon].joueurEnCours.socket == joueur.socket) {
		string message = "AskEstimate \r\n";
		Salon::sendMessage(joueur, message);
		return true;
	}
	//C'est un nouveau tour
	this->tourEnCours = Salon::mapSalonCacheSalon[this->nomSalon].tourEnCours;
	this->needRoll = true;
	return false;
}

/// <summary>
/// Passe au joueur suivant comme son nom l'indique
/// </summary>
void Salon::joueurSuivant() {
	int i = 1;
	//On le met ici pour limiter les recherches dans la map, on recherche encore pour les affectation pcq les pointeur et c++ sont mal fait
	CacheSalon cache = Salon::mapSalonCacheSalon[this->nomSalon];
	for (auto const& x : cache.Joueurs) {
		if(x.socket == cache.joueurEnCours.socket)
			break;
		i++;
	}
	
	if (i < cache.Joueurs.size()) {
		list<Joueur>::iterator it = cache.Joueurs.begin();
		advance(it, i);
		Salon::mapSalonCacheSalon[this->nomSalon].joueurEnCours = *it;
	}
	else {
		Salon::mapSalonCacheSalon[this->nomSalon].joueurEnCours = cache.Joueurs.front();
	}
}

/// <summary>
/// On regarde le resultat des estimation des joueurs
/// </summary>
/// <param name="estimationActuelle"></param>
void Salon::checkScore(Estimation estimationActuelle) {
	CacheSalon cache = Salon::mapSalonCacheSalon[this->nomSalon];
	short wantedDice = cache.lastEstimation.dice;
	int res = 0;	
	bool perdu = false;

	for (Joueur x : cache.Joueurs)
		res += x.countDiceOccurences(wantedDice);

	string message = "Il y avait " + to_string(res) + " fois le dé " + to_string(wantedDice) + ". ";
	
	//Qu'es ce qu'on doit verifier
	if (estimationActuelle.dodo) {
		//Es-ce que la dernière estimation était bonne?
		if (res >= cache.lastEstimation.dice) {
			//On change le score 
			perdu = Salon::changeScore(estimationActuelle.joueur, -1);
			message += "Le joueur " + estimationActuelle.joueur.pseudo + " perd 1 dé. ";
			if (perdu) message += "\r\nLe joueur " + estimationActuelle.joueur.pseudo + " a perdu.";
		}
		else {
			perdu = Salon::changeScore(cache.lastEstimation.joueur, -1);
			message += "Le joueur " + cache.lastEstimation.joueur.pseudo + " perd 1 dé. ";
			if (perdu) message += "\r\nLe joueur " + cache.lastEstimation.joueur.pseudo + " a perdu.";
		}
	}
	else {
		//Si on est en kalzone
		if (res == cache.lastEstimation.dice) {
			Salon::changeScore(estimationActuelle.joueur, 1);
			message += "Le joueur " + estimationActuelle.joueur.pseudo + " gagne 1 dé! ";
		}
		else {
			perdu = Salon::changeScore(estimationActuelle.joueur, -1);
			message += "Le joueur " + estimationActuelle.joueur.pseudo + " perd 1 dé. ";
			if (perdu) message += "\r\nLe joueur " + estimationActuelle.joueur.pseudo + " a perdu.";
		}
	}
	message += "\r\n";
	if (perdu) {
		Joueur gagnant = checkGagnant();
		if (gagnant.socket != 0) {
			message += "\r\nLe joueur " + gagnant.pseudo + " a gagné ! ! \r\n";
			Salon::mapSalonCacheSalon[this->nomSalon].isEnCours = false;
		}
	}
	Salon::sendMessageAll(message);
}

/// <summary>
/// Change le score d'un joueur, plutot sale pcq c++ c'est nul
/// </summary>
/// <param name="joueur"></param>
/// <param name="increment"></param>
/// <returns>true si le joueur a perdu</returns>
bool Salon::changeScore(Joueur joueur, int increment) {
	synchroList.lock();
	bool perdu = false;
	//Obligé de faire comme ca pcq c++ c'est nul
	list<Joueur> localJoueurs = Salon::mapSalonCacheSalon[this->nomSalon].Joueurs;
	for (auto it = localJoueurs.rbegin(); it != localJoueurs.rend(); it++) {
		if (it->socket == joueur.socket) {
			it->score += increment;
			perdu = it->score == 0;
		}
	}
	Salon::mapSalonCacheSalon[this->nomSalon].Joueurs = localJoueurs;
	synchroList.unlock();
	return  perdu;
}

/// <summary>
/// Syncronise le joueur et la liste statique pcq c++ c'est nul
/// </summary>
/// <param name="joueur"></param>
void Salon::synchroRoll(Joueur joueur) {	
	synchroList.lock();
	//Obligé de faire comme ca pcq c++ c'est nul
	list<Joueur> localJoueurs = Salon::mapSalonCacheSalon[this->nomSalon].Joueurs;
	for (auto it = localJoueurs.rbegin(); it != localJoueurs.rend(); it++) {
		if (it->socket == joueur.socket) {
			it->dices = joueur.dices;
		}
	}
	Salon::mapSalonCacheSalon[this->nomSalon].Joueurs = localJoueurs;
	synchroList.unlock();
}

/// <summary>
/// retourne un joueur avec un socket 0 si il n'y a pas de vainqueur sinon le joueur gagnant
/// </summary>
/// <returns></returns>
Joueur Salon::checkGagnant() {
	Joueur gagnant = Joueur(0, "");
	list<Joueur> localJoueurs = Salon::mapSalonCacheSalon[this->nomSalon].Joueurs;
	for (auto it = localJoueurs.rbegin(); it != localJoueurs.rend(); it++) {
		if (it->score==0) {
			if (gagnant.socket == 0)
				gagnant = *it;
			else
				return Joueur(0, "");
		}
	}
	return gagnant;
}

/// <summary>
/// sert a retourner les elements de tous les salons en cours d'utilisation
/// </summary>
/// <returns></returns>
string Salon::getSalonsInfos() {
	//On les charge ici pour éviter des potentielles erreurs dans la boucle
	map<string, Salon> mapNomSalonSalonLocal = Salon::mapNomSalonSalon;
	map<string, CacheSalon> mapSalonCacheSalonLocal = Salon::mapSalonCacheSalon;

	string res = "AnswerAvailableSalon";

	for (auto it = mapNomSalonSalonLocal.rbegin(); it != mapNomSalonSalonLocal.rend(); it++) {
		CacheSalon salon = mapSalonCacheSalonLocal[it->first];
		if (!salon.isEnCours)
			res += " " + it->first + " " + to_string(salon.nbJoueurs) + " " + to_string(it->second.nbJoueurMax);
	}

	return res;
}

