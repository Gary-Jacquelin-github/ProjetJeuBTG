
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <time.h>
#include <iostream>
#include <winsock2.h>
#include <string.h>
#include <string>
#include <cstring>
#include <datetimeapi.h>
#include <chrono>
#include <ctime>    
#include <sstream>
#pragma comment(lib, "ws2_32.lib")

#define MYPORT 3490     // Port du point de connexion
#define BACKLOG 10      // Combien de connexion en attente autorise-t-on ?
#define MAXDATASIZE 100 // Nombre maximal d'octets à envoyer en une fois

using std::string;
using std::cin;
using std::cout;
using std::endl;

int main()
{
	//WSADATA WSAData;
	//SOCKET sock_fd;         // sock_fd : point de connexion
	//SOCKET new_fd;          // new_fd : socket de communication
	//SOCKADDR_IN my_addr;    // adresse de transport de la socket coté serveur
	//SOCKADDR_IN their_addr; // adresse de transport de la socket coté client
	//unsigned int sin_size;  // Taille d'une adresse de transport
	//char date_string[100];  // Chaine de caractere contenant la date
	//SYSTEMTIME time_value;

	//WSAStartup(MAKEWORD(2, 0), &WSAData);

	//// Creation et initialisation du point de connexion
	//sock_fd = socket(AF_INET, SOCK_STREAM, 0);

	//// Configuration de l'adresse de transport
	//my_addr.sin_addr.s_addr = INADDR_ANY; // adresse, devrait être converti en reseau mais est egal à 0
	//my_addr.sin_family = AF_INET;         // type de la socket
	//my_addr.sin_port = htons(MYPORT);     // port, converti en reseau

	//// Demarrage du point de connexion : on ajoute l'adresse de transport dans la socket
	//if (bind(sock_fd, (SOCKADDR*)&my_addr, sizeof(my_addr)) == -1) {
	//	perror("Creation de socket : ");
	//}
	//// Attente sur le point de connexion
	//else if (listen(sock_fd, BACKLOG) == -1)
	//{
	//	perror("Demarrage du point d'ecoute : ");
	//}


	//// Boucle principale de traitement des demandes client
	//while (1)
	//{
	//	int sinsize = sizeof(their_addr);

	//	// Accept est bloquant : le processus n'avance pas tant que personne ne se connecte
	//	if ((new_fd = accept(sock_fd, (SOCKADDR*)&their_addr, &sinsize)) == -1)
	//		perror("Acceptation connexion: ");

	//	// Une connexion a eu lieu : on recupere l'adresse du client
	//	printf("server: got connection from %s\n",
	//		inet_ntoa(their_addr.sin_addr)); // inet_ntoa : net to ascii

	// // On recupere l'heure et la date
	//	GetLocalTime(&time_value);
	//	sprintf(date_string,
	//		"%d-%02d-%02d %02d:%02d:%02d.%03d",
	//		time_value.wYear,
	//		time_value.wMonth,
	//		time_value.wDay,
	//		time_value.wHour,
	//		time_value.wMinute,
	//		time_value.wSecond,
	//		time_value.wMilliseconds);

	//	string message = "Que voulez vous\r\n";

	//	if (send(new_fd, message.c_str(), strlen(message.c_str()), 0) < 0) {
	//		perror("Error send: ");
	//	}
	//	char tmp[6000];
	//	int nbBytes = 0;
	//	string stringRecu;
	//	int test = stringRecu.find("\r\n");
	//	while (test < 0) {
	//		if ((nbBytes = recv(new_fd, tmp, 6000, 0)) == -1) {
	//			perror("Error recu: ");
	//		}
	//		tmp[nbBytes] = '\0';
	//		stringRecu += tmp;
	//		test = stringRecu.find("\r\n");
	//	}

	//	stringRecu = stringRecu.substr(0, stringRecu.length() - 2);

	//	auto t = std::time(nullptr);
	//	auto tm = *std::localtime(&t);

	//	time_t rawtime;
	//	struct tm* timeinfo;
	//	char buffer[80];

	//	time(&rawtime);
	//	timeinfo = localtime(&rawtime);

	//	if (stringRecu == "DATE") {
	//		strftime(buffer, 80, "%d/%m/%y", timeinfo);
	//		puts(buffer);
	//		message = buffer;
	//		message += "\r\n";
	//	}
	//	else if (stringRecu == "JOUR")
	//	{
	//		strftime(buffer, 80, "%A", timeinfo);
	//		puts(buffer);
	//		message = buffer;
	//		message += "\r\n";
	//	}
	//	else if (stringRecu == "MOIS")
	//	{
	//		strftime(buffer, 80, "%B", timeinfo);
	//		puts(buffer);
	//		message = buffer;
	//		message += "\r\n";
	//	}

	//	if (send(new_fd, message.c_str(), message.length(), 0) < 0)
	//		perror("Error send: ");
	//}

	return 0;
}

