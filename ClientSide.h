#include "winsock2.h"
#include "ws2tcpip.h"
#include <iostream>
#include <stdio.h>

using namespace std;


class Client{
    int erStat{};
    sockaddr_in servInfo{};

    char* clientIp;
    unsigned clientPort;

    WSADATA wsadata{};
    SOCKET ClientSocket{};
    SOCKET ServerSocket{};
public:
    Client(char* _clientIp, unsigned _clientPort){
        this->clientIp = _clientIp;
        this->clientPort = _clientPort;
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
        ClientSocket = socket(AF_INET, SOCK_STREAM, 0);

        if (ClientSocket == INVALID_SOCKET){
            cout << "Error create socket: "<< WSAGetLastError() << endl;
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }
        else{
            cout << "Server socket initialized" << endl;
        }

        in_addr ipToNum;
        erStat = inet_pton(AF_INET, this->clientIp, &ipToNum);

        if (erStat <= 0){
            cout << "Error convert ip address" << endl;
            return 1;
        }


        ZeroMemory(&servInfo, sizeof(servInfo));

        servInfo.sin_addr = ipToNum;
        servInfo.sin_family = AF_INET;
        servInfo.sin_port = htons(this->clientPort);


        erStat = bind(ClientSocket, (sockaddr*)&servInfo, sizeof(servInfo));

        if (erStat != 0){
            cout << "Error binding socket: " << WSAGetLastError() << endl;
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }

        else{
            cout << "Socket was bind" << endl;
            return 0;
        }
    }

    void connectToServer(char* serverIp, unsigned serverPort){

        in_addr ipToNum;
        erStat = inet_pton(AF_INET, serverIp, &ipToNum);

        if (erStat <= 0){
            cout << "Error convert ip address" << endl;
            return;
        }

        ZeroMemory(&servInfo, sizeof(servInfo));

        servInfo.sin_port = htons(serverPort);
        servInfo.sin_family = AF_INET;
        servInfo.sin_addr = ipToNum;


        erStat = connect(ClientSocket, (sockaddr*)&servInfo, sizeof(servInfo));

        if (erStat != 0){
            cout << "Error to connect: " << WSAGetLastError() << endl;
            closesocket(ClientSocket);
            WSACleanup();
        }
        else{
            cout << "Connection SUCCESSFULLY" << endl;
        }
    }

    char* recivePacket(vector<char> serverBuff){
        int packetSize = recv(ClientSocket, serverBuff.data(), serverBuff.size(), 0);

        return serverBuff.data();
    }

    int sendPacket(vector<char> clientBuff){
        if (clientBuff[0] == 'x' && clientBuff[1] == 'x' && clientBuff[2] == 'x'){
            shutdown(ClientSocket, SD_BOTH);
            closesocket(ClientSocket);
            WSACleanup();

            return 1;
        }

        int packetSize = send(ClientSocket, clientBuff.data(), clientBuff.size(), 0);

        if (packetSize == SOCKET_ERROR){
            cout << "Can't send message to Client. Error # " << WSAGetLastError() << endl;
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }

        return 0;
    }
};
