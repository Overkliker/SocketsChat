#include <iostream>
#include <vector>
#include "ServerSide.h"
#include "ClientSide.h"
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;


[[noreturn]] void recvMessagesServer(Server* server){
    vector <char> servBuff;

    while (true){
        cout << server->recivePacket(servBuff, 0) << endl;
    }
}

void sendMessages(Server* server){
    vector <char> clientBuff;
    char message[] = "";
    while (true){
        cout << "Enter message: ";
        cin >> message;

        for (const int i : message){
            clientBuff.push_back(i);
        }

        cout << "Your (host) message: ";
        fgets(clientBuff.data(), clientBuff.size(), stdin);
        server->sendPacket(clientBuff, 0);

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

        thread t (recvMessagesServer, server);
        t.join();
//        sendMessages(server);
    }
    else{

        Client* client = new Client(ip, 1111);

        client->createWsaStartup(2, 2);
        client->createSock();
        client->connectToServer(ip, 1234);
    }
    return 0;
}