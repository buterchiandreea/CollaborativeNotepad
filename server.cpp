#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <map>
#include <vector>
#include <thread>
#include <sstream>
#include "network_utils.h"
#include "colors.h"


using std::thread;
using std::string;
using std::map;
using std::vector;
using std::stringstream;



#ifndef PORT
#define PORT 6969
#endif

#ifndef SERVER_BIND_ADDRESS
#define SERVER_BIND_ADDRESS "localhost"
#endif


extern int errno;




void clientThreadLoop(int clientFd, map<string, vector<int> > *documentsDict) {
   printf("[SERVER THREAD] Thread with fd %d started \n", clientFd);
   while(1) {
       string msg;
       try {
            msg = Read(clientFd);
       }
       catch (std::ios_base::failure exception) {
            printf(RED "[SERVER THREAD] %s \n" RESET, exception.what());
            break;
       }
       printf("[SERVER THREAD] Command received from client: %s \n", msg.c_str());

       if(msg == "list") {
           stringstream ss;
           ss << documentsDict->size();
           printf("%lu %s \n", documentsDict->size(), ss.str().c_str());
           Write(clientFd, ss.str());//server transmits to the client the number of existent documents
           for(auto it : *documentsDict) {
               Write(clientFd, it.first); //server transmits the existent documents
           }

       }
       else if(msg.compare(0, 6, "create") == 0) {

           string space = " ";
           std::size_t found = msg.find(space);
           string documentName = msg.substr(found+1); //the name of the document starts after space delimiter
           printf("Document: %s \n", documentName.c_str());

           if(documentsDict->find(documentName) != documentsDict->end()) {
              printf("Hello! Here is an error!\n");
              Write(clientFd, "ERROR: File with the same name already exists!");
              continue;
           }

           printf("Hello! Here is not an error!\n");

           documentsDict->insert(std::make_pair(documentName, vector<int>()));
           Write(clientFd, "OK!");
       }

       else if(msg.compare(0, 6, "delete") == 0) {

           string space = " ";
           std::size_t found = msg.find(space);
           string documentName = msg.substr(found+1); //the name of the document starts after space delimiter
           printf("Document: %s \n", documentName.c_str());

           auto elem = documentsDict->find(documentName);
           if(elem != documentsDict->end()) {

              documentsDict->erase(elem); //erase by key
           }

           else {
                   Write(clientFd, "ERROR: File was already removed!");
                   continue;
           }


           Write(clientFd, "OK!");
       }

       else if(msg.compare(0, 4, "open") == 0) {

           string space = " ";
           std::size_t found = msg.find(space);
           string documentName = msg.substr(found+1); //the name of the document starts after space delimiter
           printf("Document: %s \n", documentName.c_str());


       }

   }

}



int serverLoop(string baseAddr, int port, int backlog) {
    struct sockaddr_in server;
    struct sockaddr_in from;
    int sd;

    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1) {
        perror ("[SERVER] Error at socket! \n");
        return errno;
    }

    int on = 1;
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    bzero (&server, sizeof (server));
    bzero (&from, sizeof (from));

    server.sin_family = AF_INET;
    inet_pton(AF_INET, baseAddr.c_str(), &(server.sin_addr));
    server.sin_port = htons (port);


    if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1) {
        perror ("[SERVER] Error at bind! \n");
        return errno;
    }


    if (listen (sd, backlog) == -1) {
        perror ("[SERVER] Error at listen! \n");
        return errno;
    }
    map<string, vector<int> > *documentsDict = new map<string, vector<int> >();

    while (1)
    {
        int clientFd;
        int length = sizeof (from);
        fflush (stdout);
        if ( (clientFd = accept (sd, (struct sockaddr *) &from, (socklen_t *) &length)) < 0) {
            perror ("[SERVER] Error at accept! \n");
            continue;
        }

        printf("[SERVER] Client with fd %d connected \n", clientFd);

        auto client = thread(clientThreadLoop, clientFd, documentsDict);
        client.join();
    }
}




int main() {

    printf("Binding to %s:%d \n", SERVER_BIND_ADDRESS, PORT);
    serverLoop(SERVER_BIND_ADDRESS, PORT, 5);

    return 0;
}
