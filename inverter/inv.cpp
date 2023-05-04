#include "inv.h"
#include "../common/common.h"
#include <unistd.h>
#include <string.h>     // memset
#include <modbus/modbus.h>

void INV_Field::clear(void) 
{
    memset(SN, 0, 24);
    R_Voltage = R_Current = R_Power = R_Frequency = 0;
    S_Voltage = S_Current = S_Power = S_Frequency = 0;
    T_Voltage = T_Current = T_Power = T_Frequency = 0;
    for (int i = 0; i < 12; i++)
        Voltage[i] = Current[i] = Power[i] = 0;

    Temperature = 0;
    KWRating = 0;
    Error.clear();
    ETotal = HTotal = EToday = EdayCal = Pac = 0;
}

INV_STATE inv_errorState(int errnum, INV_Field *invField)
{
    invField->Mode = InvMode_Offline;
    INV_STATE inv_state;
    switch (errnum) {
        case ETIMEDOUT:
            inv_state = INV_ModBusSelectTimoeout;
            invField->Error.append("沒有回應");
            break;
        case EMBBADCRC:
            inv_state = INV_ModBusCrcError;
            invField->Error.append("資料格式錯誤");
            break;
        case EMBBADSLAVE:
            inv_state = INV_ModBusBadSlaveId;
            invField->Error.append("資料格式錯誤");
            break;
        default:
            inv_state = INV_ModBusDataError;
            invField->Error.append("資料格式錯誤");
            break;
    }
    return inv_state;
}

static void inv_printerror(INV_STATE inv_state, std::map<int, std::string>& inv_status)
{
    if (inv_status[inv_state].size()) {
        switch (inv_state) {
            case INV_Disconnect:
                printf("Disconnect:");
                break;
            case INV_Logging:
                printf("Send Failed:");
                break;
            case INV_ScanError:
                printf("Fail to scan:");
                break;
            case INV_ModBusSelectTimoeout:
                printf("No response:");
                break;
            case INV_ModBusCrcError:
                printf("CRC error:");
                break;
            case INV_ModBusBadSlaveId:
                printf("Slave id error:");
                break;
            case INV_ModBusDataError:
                printf("DataError:");
                break;
            case INV_ReSend:
                printf("ReSend:");
                break;
            default:
                break;
        }
        printf("%s\n", inv_status[inv_state].c_str());
    }
}

void inv_printerrors(std::map<int, std::string>& inv_status)
{
    inv_printerror(INV_Disconnect, inv_status);
    inv_printerror(INV_Logging, inv_status);
    inv_printerror(INV_ScanError, inv_status);
    inv_printerror(INV_ModBusSelectTimoeout, inv_status);
    inv_printerror(INV_ModBusCrcError, inv_status);
    inv_printerror(INV_ModBusBadSlaveId, inv_status);
    inv_printerror(INV_ModBusDataError, inv_status);
    inv_printerror(INV_ReSend, inv_status);
}

