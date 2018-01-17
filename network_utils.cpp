#include "network_utils.h"

int Write(int fd, string command) {
    int size = command.size();
    if (write(fd, &size, sizeof(size)) == -1) {
        return errno;
    }

    if (write(fd, command.c_str(), command.size()) == -1) {
        return errno;
    }
    return 0;
}

string Read(int fd, int MESSAGE_MAX_CHUNK_SIZE) {

    int size;
    int readBytes = read(fd, &size, 4);

    if(readBytes <= 0) {
        throw std::ios_base::failure(strerror(errno));
    }
    char *buffer = new char[MESSAGE_MAX_CHUNK_SIZE];
    string command = "";
    int totalReadBytes = 0;
    while(totalReadBytes < size) {

        int readBytes = read(fd, buffer, min(MESSAGE_MAX_CHUNK_SIZE - 1, size - totalReadBytes));
        if(readBytes <= 0) {
            delete[] buffer;
            throw  std::ios_base::failure(strerror(errno));
        }

        buffer[readBytes] = 0;
        command += buffer;
        totalReadBytes += readBytes;

    }

    delete[] buffer;
    return command;
}
