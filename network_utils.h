#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#pragma once

#include <string.h>
#include <unistd.h>
#include <iostream>

using std::string;
using std::min;


extern int Write(int fd, string command);

extern string Read(int fd, int MESSAGE_MAX_CHUNK_SIZE = 1024);


#endif // NETWORK_UTILS_H
