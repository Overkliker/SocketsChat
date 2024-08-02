#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int main() {

    WSADATA wsadata;

    int erStat = WSAStartup(MAKEWORD(2, 2), &wsadata);

    if (erStat != 0){
        cout << "Error WinSock version initializaion #";
        cout << WSAGetLastError();
        WSACleanup();
        return 1;
    }
    else{
        cout << "WinSock initialization is OK" << endl;
    }

    WSACleanup();

    return 0;
}
