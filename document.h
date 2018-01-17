#include<string>
#include<vector>
#include <list>
#include <map>
#include <mutex>
#include <chrono>
#include <sstream>

#include "operation.h"

using std::vector;
using std::list;
using std::string;
using std::map;
using std::mutex;
using std::stringstream;

struct Client {
    int fd;
    list<Operation> history;
};

struct Document {
    string documentText;
    string documentName;
    vector<Client> clients;
    int lastId;
    int lastIdCount;
    mutex lock;

    Document(const Document &doc);

    Document(string name);

    int AddClient(int fd);
    void DisconnectClient(int fd);

    void ApplyOperation(int fd, string command);
};
