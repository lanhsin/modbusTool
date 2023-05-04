#include "inverter/inverters.h"
#include "../common/common.h"
#include <iostream>

Configs configs;
Debugs  debugs;

int main(int argc, char **argv)
{
    if (false == local_time_init())
        return 0;
    else if (false == config_init())
        return 0;

    debugs.set_enable(true);
    debugs.parsing_argc(argc, argv);

    int configNum = configs.parsing_Json();
    if (configNum == 0) {
        std::cout << "No config file!\n";
        return 0;
    }

    if(debugs.is_enable()) {
        debug_run();
        return 0;
    }

    return 0;
}
