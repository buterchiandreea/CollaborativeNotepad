#include "operation.h"
#include <string>

using std::string;

Operation::Operation() {

}

void Operation::applyOperation(string &text) {

    if(isInsert == 1) {

        text.insert(this->pos, this->chr);
    }

    else {
        text.erase(this->pos, 1);

    }
}
