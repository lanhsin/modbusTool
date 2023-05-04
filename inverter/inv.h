#ifndef INV_H_
#define INV_H_

#include "config.h"

#define DC_NUM_MAX          12


/* Internal Defination
atWork Time :  OK:      INV_Logging
               Errors:  INV_Disconnect / INV_ScanError / INV_ModBusSelectionTimeout/ INV_ModBusCrcError / INV_ModBusBadSlaveId / INV_ModBusDataError
offline Time : INV_ReSend / INV_Idle
*/
enum INV_STATE{
    INV_Disconnect            ,
    INV_Logging               ,   // Normal State
    INV_ScanError             ,   // Can not find device.
    INV_ModBusSelectTimoeout  ,   // Can not received any packet from device.
    INV_ModBusCrcError        ,   // The CRC of received packet is error.
    INV_ModBusBadSlaveId      ,   // The slave id of received packet is error.
    INV_ModBusDataError       ,   // Others error.
    INV_ReSend                ,   // Fail send string data to internet.
    INV_Idle                      // Nothing happend. It's a idle state.
};

/* Internal Defination
*  Register value from inverter
*/
enum INV_MODE {
    InvMode_Init     ,  // Initial Mode (Power on Mode)
    InvMode_Wait     ,  // Wait Mode (Standby Mode)
    InvMode_Check    ,  // Check Mode
    InvMode_Work     ,  // Work Mode(Line Mode)
    InvMode_Offline  ,  // Offline Mode
    InvMode_Fault    ,  // Fault Mode
    InvMode_MFlash   ,  // Master flash Mode
    InvMode_SFlash      // Slave flash Mode
};

/* External Defination
*  Covert to inverter status from inverter mode
*/
enum INV_STATUS {
    InvStatus_Wait        = 0,
    InvStatus_Working     = 1,
    InvStatus_Fault       = 2,
    InvStatus_PermFault   = 3,
    InvStatus_Offline     = 4
};

struct INV_Field{
    unsigned int SlaveId;
    INV_BRAND Brand;
    char SN[24];
    unsigned int R_Voltage;     //0.1V
    unsigned int R_Current;     //0.1A
    unsigned int R_Power;       //0.01W
    unsigned int R_Frequency;   //0.01Hz
    unsigned int S_Voltage;
    unsigned int S_Current;
    unsigned int S_Power;
    unsigned int S_Frequency;
    unsigned int T_Voltage;
    unsigned int T_Current;
    unsigned int T_Power;
    unsigned int T_Frequency;
    unsigned int Voltage[DC_NUM_MAX];   //0.1V
    unsigned int Current[DC_NUM_MAX];   //0.1A
    unsigned int Power[DC_NUM_MAX];     //0.01W
    unsigned int Temperature;   //0.1°C
    unsigned int Mode;
    float KWRating;             //1W	//From inverter
    std::string Error;
    unsigned int ETotal;        //0.1kW-H
    unsigned int HTotal;
    unsigned int EToday;        //1kW
    unsigned int EdayCal;       //1kW
    unsigned int Pac;           //0.1W
    char KW[10];
    char KWR[10];//From TXT
    long ts;//timestamp
    void clear(void);
};

INV_STATE inv_errorState(int errnum, INV_Field *pvField);
void inv_printerrors(std::map<int, std::string>& inv_status);


#endif //INV_H
