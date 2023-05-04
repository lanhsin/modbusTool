#ifndef DEBUG_H
#define DEBUG_H

#include <limits.h> // UINT_MAX

enum modbus_mode{
    RTU,
    TCP
};

class Debugs {
    bool bEnable = false;
    bool bholding  = true;
    modbus_mode mode = RTU;
    unsigned int slave_id = UINT_MAX;
    unsigned int address = UINT_MAX;
    unsigned int count =  1;
    unsigned int interval = 1;

public:
    bool is_enable(void) { return bEnable; }
    void set_enable(bool val) { bEnable = val; }

    unsigned int get_slave_id(void) { return slave_id; }
    void set_slave_id(unsigned int id) { slave_id = id; }

    unsigned int get_address(void) { return address; }
    void set_address(unsigned int addr) { address = addr; }

    modbus_mode get_mode(void) { return mode; }
    void set_mode(char* argv);

    bool is_hodingRegister(void) { return bholding; }
    void set_holdingRegister(int val);

    unsigned int get_count(void) { return count; }
    void set_count(unsigned int val) { count = val; }

    unsigned int get_interval(void) { return interval; }
    void set_interval(unsigned int val) { interval = val; }

    void parsing_argc(int argc, char **argv);
};

#endif