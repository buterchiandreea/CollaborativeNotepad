#ifndef OPERATION_H
#define OPERATION_H
#include <string>

using std::string;

class Operation
{
public:
    Operation();
    void applyOperation(string &text);

private:
    int pos;
    char *chr;
    int id;
    bool isInsert;
};

#endif // OPERATION_H
