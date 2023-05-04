#ifndef INVERTERS_H
#define INVERTERS_H

#include "inv.h"


INV_STATE getInvData_simdev(INV_Field *invField);


void debug_print(INV_Field *Buf) ;
void debug_run();

#endif //INVERTERS_H
