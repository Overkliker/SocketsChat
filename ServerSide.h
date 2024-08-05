#include "winsock2.h"
#include "ws2tcpip.h"
#include <iostream>
#include <stdio.h>
#include <vector>

using namespace std;


class Server{
    int erStat;
    sockaddr_in servInfo;

    WSADATA wsadata;
    SOCKET ServerSocket;

    char* serverIp;
    unsigned serverPort;

    vector<SOCKET> clients;
public:

    Server(char* _serverIp, unsigned _serverPort){
        this->serverIp = _serverIp;
        this->serverPort = _serverPort;
    }

    int createWsaStartup(int minVersion, int maxVersion){
        erStat = WSAStartup(MAKEWORD(maxVersion, minVersion), &wsadata);

        if (erStat != 0){
            cout << "Error WinSock version initializaion #";
            cout << WSAGetLastError();
            return 1;
        }
        else{
            cout << "WinSock initialization is OK" << endl;
            return 0;
        }
    }

    int createSock(){
        ServerSocket = socket(AF_INET, SOCK_STREAM, 0);

        if (ServerSocket == INVALID_SOCKET){
            cout << "Error create socket: "<< WSAGetLastError() << endl;
            closesocket(ServerSocket);
            WSACleanup();
            return 1;
        }
        else{
            cout << "Server socket initialized" << endl;
        }

        in_addr ipToNum;
        erStat = inet_pton(AF_INET, this->serverIp, &ipToNum);

        if (erStat <= 0){
            cout << "Error convert ip address" << endl;
            return 1;
        }


        ZeroMemory(&servInfo, sizeof(servInfo));

        servInfo.sin_addr = ipToNum;
        servInfo.sin_family = AF_INET;
        servInfo.sin_port = htons(this->serverPort);


        erStat = bind(ServerSocket, (sockaddr*)&servInfo, sizeof(servInfo));

        if (erStat != 0){
            cout << "Error binding socket: " << WSAGetLastError() << endl;
            closesocket(ServerSocket);
            WSACleanup();
            return 1;
        }

        else{
            cout << "Socket was bind" << endl;
            return 0;
        }
    }

    void listenClients(){
        erStat = listen(ServerSocket, SOMAXCONN);

        if (erStat != 0){
            cout << "Error to listen: " << WSAGetLastError() << endl;
            closesocket(ServerSocket);
            WSACleanup();
        }
        else{
            cout << "Listening..." << endl;
            accessConnection();
        }
    }

    void accessConnection(){
        sockaddr_in clientInfo;

        ZeroMemory(&clientInfo, sizeof(clientInfo));

        int clientInfoSize = sizeof(clientInfo);

        SOCKET ClientSocket = accept(ServerSocket, (sockaddr*)&clientInfo, &clientInfoSize);

        if (ClientSocket == INVALID_SOCKET){
            cout << "Client detected? but cant connect. Error: " << WSAGetLastError() << endl;
            closesocket(ServerSocket);
            closesocket(ClientSocket);
            WSACleanup();

            return;
        }
        else{
            cout << "Connection SUCCESSFULLY" << endl;

            clients.push_back(ClientSocket);
        }
    }

    char* recivePacket(vector<char> serverBuff, int clientIndex){
        int packetSize = recv(clients[clientIndex], serverBuff.data(), serverBuff.size(), 0);

        return serverBuff.data();
    }

    int sendPacket(vector<char> clientBuff, int clientIndex){
        if (clientBuff[0] == 'x' && clientBuff[1] == 'x' && clientBuff[2] == 'x'){
            shutdown(clients[clientIndex], SD_BOTH);
            closesocket(ServerSocket);
            closesocket(clients[0]);
            WSACleanup();

            return 1;
        }

        int packetSize = send(clients[clientIndex], clientBuff.data(), clientBuff.size(), 0);

        if (packetSize == SOCKET_ERROR){
            cout << "Can't send message to Client. Error # " << WSAGetLastError() << endl;
            closesocket(ServerSocket);
            closesocket(clients[clientIndex]);
            WSACleanup();
            return 1;
        }

        return 0;
    }
};
