#ifndef OPERATION_H
#define OPERATION_H
#include <string>
#include <list>

using std::string;
using std::list;

struct Operation
{

    Operation(string message);
    Operation(bool isInsert, int pos, int prevId, char chr = 255);
    void applyOperation(string &text);
    string toStr();
    int pos;
    char chr;
    int id;
    bool isInsert;
    int serverId;
    int prevId;

};

extern bool updateOperation(list<Operation> &history, Operation &operation, bool secondCall = false);

#endif // OPERATION_H
