#pragma once

#include <thread>
#include <winsock2.h>
#include <string>

using namespace std;

void communicationMain(SOCKET socket, int JoueurManquant);