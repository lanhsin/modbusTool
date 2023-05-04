#include "inv.h"

INV_STATE getInvData_simdev(INV_Field *invField)
{
    invField->Pac = 20;
    /* use scale factor, 0.01A for Iac 0.1V for V , 1W for Power */
    invField->R_Voltage = 2560;
    invField->R_Current = 40;
    invField->R_Power = invField->R_Voltage * invField->R_Current;
    invField->R_Frequency = 6000;

    /* use scale factor, 0.01A for Iac 0.1V for V , 1W for Power */
    invField->Voltage[0] = 3000;
    invField->Current[0] = 10;
    invField->Power[0] = invField->Voltage[0] * invField->Current[0];
    invField->Mode = InvMode_Work;

    /* 0x430  1072 Today Energy */
    invField->EToday = 50000;
    /*  0x434 Life Energy  10Wh */
    invField->ETotal = 60000;
    /*  0x436 Life  Runtime 1s */
    invField->HTotal = 5;

    /* 0x43A 1082 Inverter Temperature 1°C */
    invField->Temperature = 540;

    invField->KWRating = 5000;

    return INV_Logging;
}

