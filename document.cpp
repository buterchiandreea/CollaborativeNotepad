#include "document.h"
#include "network_utils.h"

Document::Document(const Document &doc) {
    this->documentName = doc.documentName;
    this->lastId = doc.lastId;
    this->lastIdCount = doc.lastIdCount;
    this->documentText = doc.documentText;
    this->clients = doc.clients;
    this->lock.unlock();
}

Document::Document(string name) {
    this->documentName = name;
    this->lastId = -1;
    this->lastIdCount = 0;
    this->lock.unlock();

    // read document from hard drive
}


int Document::AddClient(int fd) {
    Client c;
    c.fd = fd;

    int prevId = 0;
    if (!this->clients.empty()) {
        // If there is another client, we fetch the id of the last known operation
        prevId = this->clients[0].history.back().id;
    }
    this->clients.push_back(c);
    //printf("Clients: %d\n", this->clients.size());

    return prevId;
}

void Document::DisconnectClient(int fd) {
    vector<Client>::iterator found = this->clients.end();
    for (auto it = this->clients.begin(); it != this->clients.end(); it++) {
        if(it->fd == fd) {
            found = it;
            break;
        }
    }

    if (found != this->clients.end()) {
        this->clients.erase(found);
    }
}

void Document::ApplyOperation(int fd, string command) {
    Operation o(command);
    printf("ApplyOperation: %s %d\n", command.c_str(), (int)o.chr);
    bool deleteCaseDetected = false;

    this->lock.lock();
    o.id = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    if (o.id > lastId) {
        lastId = o.id;
        lastIdCount = 0;
    }
    o.serverId = lastIdCount;
    lastIdCount++;
    bool hasPeer = false;
    for(auto client : this->clients) {
        if(client.fd == fd) {
            client.history.push_back(o);

        }
        else {
            Write(client.fd, o.toStr()); //sends operation to the peer
            printf("Send op to peer: %d %s\n", client.fd, o.toStr().c_str());
            hasPeer = true;

            if (!updateOperation(client.history, o)) { //we check for the conflict
                deleteCaseDetected = true;

            }
        }
    }
    if(!deleteCaseDetected) {
        o.applyOperation(this->documentText);
    }

    this->lock.unlock(); //server does not accept another operation while updating

    stringstream msg;
    msg << o.id << " " <<o.serverId << " " <<hasPeer;
    Write(fd, msg.str());
}
