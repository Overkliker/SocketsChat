#include <iostream>
#include <stdio.h>
#include <vector>
#include "ServerSide.h"
#include "ClientSide.h"
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;



void recvMessagesClient(Client* client){
    vector <char> servBuff;

    while (true){
        cout << client->recivePacket(servBuff) << endl;
    }
}

void sendMessages(Client* client){
    vector <char> clientBuff {'a', 's'};
    char* message;
    while (true){
//        cout << "Enter message: ";
//        cin >> message;
//        cout << endl;
//
//
//        for (int i = 0; i < strlen(message); i++){
//            clientBuff.push_back(message[i]);
//        }

        cout << "Your (host) message: ";
        fgets(clientBuff.data(), clientBuff.size(), stdin);
        cout << endl;
        client->sendPacket(clientBuff);

        clientBuff.clear();
    }
}


int main() {
    int who;
    cin >> who;

    char ip[] = "127.0.0.1";
    if (who){

        Server* server = new Server(ip, 1234);

        server->createWsaStartup(2, 2);
        server->createSock();
        server->listenClients();
    }
    else{

        Client* client = new Client(ip, 1111);

        client->createWsaStartup(2, 2);
        client->createSock();
        client->connectToServer(ip, 1234);

//        thread t (recvMessagesClient, client);

        sendMessages(client);
    }
    return 0;
}
