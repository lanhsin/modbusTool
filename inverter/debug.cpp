#include "debug.h"
#include <cstring>  // strcmp
#include <stdlib.h> // atoi
#include <unistd.h> // sleep

void Debugs::set_mode(char* argv)
{
    if (strcmp(argv, "rtu") == 0)
        mode = RTU;
    else if (strcmp(argv, "tcp") == 0)
        mode = TCP;
}

void Debugs::set_holdingRegister(int val)
{
    if(val == 3)
        bholding = true;
    else if(val == 4)
        bholding = false;
}

void Debugs::parsing_argc(int argc, char **argv)
{
    if (argc < 3)
        return;
    for (int i  = 1; i < argc; i+=2) {
        if (strcmp(argv[i], "-a") == 0)
            set_slave_id(atoi(argv[i+1]));
        else if (strcmp(argv[i], "-r") == 0)
            set_address(atoi(argv[i + 1]));
        else if (strcmp(argv[i], "-m") == 0)
            set_mode(argv[i+1]);
        else if (strcmp(argv[i], "-t") == 0)
            set_holdingRegister(atoi(argv[i+1]));
        else if (strcmp(argv[i], "-c") == 0)
            set_count(atoi(argv[i+1]));
        else if (strcmp(argv[i], "-o") == 0)
            set_interval(atoi(argv[i+1]));
    }
}