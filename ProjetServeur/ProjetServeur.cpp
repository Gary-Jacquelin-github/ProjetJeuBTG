
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "Lobby.h"
#include "Salon.h"
#include <iostream>
#include <sstream>


using namespace std;

#pragma comment(lib, "ws2_32.lib")


#define MYPORT 3490     // Port du point de connexion
#define BACKLOG 10      // Combien de connexion en attente autorise-t-on ?
#define MAXDATASIZE 100 // Nombre maximal d'octets à envoyer en une fois


int main()
{
	WSADATA WSAData;
	SOCKET sock_fd;         // sock_fd : point de connexion
	SOCKET new_fd;          // new_fd : socket de communication
	SOCKADDR_IN my_addr;    // adresse de transport de la socket coté serveur
	SOCKADDR_IN their_addr; // adresse de transport de la socket coté client
	unsigned int sin_size;  // Taille d'une adresse de transport
	char date_string[100];  // Chaine de caractere contenant la date
	SYSTEMTIME time_value;

	const int nombreMaxJoueur = 100;
	int nbJoueur = 0;
	
	thread* t_threads = new thread[nombreMaxJoueur];

	WSAStartup(MAKEWORD(2, 0), &WSAData);

	// Creation et initialisation du point de connexion
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);

	// Configuration de l'adresse de transport
	my_addr.sin_addr.s_addr = INADDR_ANY; // adresse, devrait être converti en reseau mais est egal à 0
	my_addr.sin_family = AF_INET;         // type de la socket
	my_addr.sin_port = htons(MYPORT);     // port, converti en reseau


	std::cout << "Serva starto \r\n";
	// Demarrage du point de connexion : on ajoute l'adresse de transport dans la socket
	if (bind(sock_fd, (SOCKADDR*)&my_addr, sizeof(my_addr)) == -1) {
		perror("Creation de socket : ");
	}
	// Attente sur le point de connexion
	else if (listen(sock_fd, BACKLOG) == -1)
	{
		perror("Demarrage du point d'ecoute : ");
	}


	// Boucle principale de traitement des demandes client
	while (1)
	{
		int sinsize = sizeof(their_addr);

		// Accept est bloquant : le processus n'avance pas tant que personne ne se connecte
		if ((new_fd = accept(sock_fd, (SOCKADDR*)&their_addr, &sinsize)) == -1)
			perror("Acceptation connexion: ");

		if (nbJoueur < nombreMaxJoueur) {
			nbJoueur++;
			t_threads[nbJoueur] = thread(Lobby::listenPlayer, new_fd);
		}
		else {
			string message = "Le serveur est plein veuillez reessayer plus tard\r\n";
			if (send(new_fd, message.c_str(), strlen(message.c_str()), 0) < 0) {
				perror("Error send: ");
			}
			closesocket(new_fd);
		}		
	}

	return 0;
}

