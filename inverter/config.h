#ifndef CONFIG_H
#define CONFIG_H

#include "debug.h"
#include <cstddef>  // size_t
#include <map>      // map
#include <string>


#define MAX_INV_NUM 50

extern Debugs debugs;

enum INV_BRAND {
    SimDev      = 0
};

struct sInvConfig {
    INV_BRAND brand;
    unsigned char mpp;
    unsigned char phase;
    unsigned char port;
    unsigned int address;
    unsigned int baud;
    float kw;
    float kwr;
    char sn[24];
};


class Configs {
    unsigned int numOfInverter = 0;
    sInvConfig invConfigs[MAX_INV_NUM];

public:

    // Inverter functions
    unsigned int get_numOfInverter(void) {
        return numOfInverter;
    }

    INV_BRAND get_Inv_brand(int inv_num) {
        return invConfigs[inv_num].brand;
    }

    void set_Inv_port(int inv_num, int port) {
        invConfigs[inv_num].port = port;
    }

    unsigned int get_Inv_addr(int inv_num) {
        return invConfigs[inv_num].address;
    }

    sInvConfig get_Inv_config(int inv_num) {
        return invConfigs[inv_num];
    }

    int parsing_Json(void);
};


bool config_init(void);
const char* config_getdir(void);
void config_get_filename(char *path, const char* file);

#endif //CONFIG_H