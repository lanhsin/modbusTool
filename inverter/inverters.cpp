#include "inverters.h"
#include "../common/common.h" // getDeviceName
#include <iomanip>  // put_time
#include <iostream> // cout
#include <string.h> // strcpy
#include <unistd.h> // sleep
#include <modbus/modbus.h>
#include <nlohmann/json.hpp>

extern Configs configs;

INV_STATE getInvData(sInvConfig config, INV_Field *invField)
{
    return getInvData_simdev(invField);
}

void debug_print(INV_Field *Buf) 
{
    nlohmann::ordered_json j_object;
    j_object["Addr"] = Buf->SlaveId;
    j_object["Mode"] = Buf->Mode;
    j_object["ERR"] = Buf->Error.c_str();
    j_object["Eday"] = Buf->EToday;
    j_object["KWR"] = Buf->KWRating;
    j_object["Temp"] = Buf->Temperature;
    j_object["PAC"] = Buf->Pac;
    j_object["Etotal"] =  Buf->ETotal;
    j_object["Htotal"] = Buf->HTotal;
    j_object["SN"] = Buf->SN;

    nlohmann::json j_dc = nlohmann::json::array();
    for (int i = 0; i < 12; i++)
        j_dc.push_back({Buf->Power[i], Buf->Voltage[i], Buf->Current[i]});
    j_object["Power Voltage Current"] = j_dc;

    j_object["RVac"] = Buf->R_Voltage;
    j_object["RIac"] = Buf->R_Current;
    j_object["SVac"] = Buf->S_Voltage;
    j_object["SIac"] = Buf->S_Current;
    j_object["TVac"] = Buf->T_Voltage;
    j_object["TIac"] = Buf->T_Current;
    j_object["Frequency"] = Buf->R_Frequency;
    j_object["Brand"] =  "SimDev";

    size_t i = 0, k = 0;
    std::cout << "{\n";
    for (auto& x : j_object.items())
    {
        if (x.key().compare("Power Voltage Current") == 0)
        {
            std::cout << "    \"" << x.key() << "\": [\n";
            for (auto& y : j_dc.items()) {
                std::cout << "        " << y.value();
                if (++k == j_dc.size())
                    std::cout << '\n';
                else
                    std::cout << ",\n";
            }
            std::cout << "    ],\n";
            ++i;
        }
        else {
            std::cout << "    \"" << x.key() << "\": " << x.value();
            if (++i == j_object.size())
                std::cout << "\n";
            else
                std::cout << ",\n";
        }
    }
    std::cout << "}\n";
}

unsigned int debug_get_index(unsigned int slave_id)
{
    unsigned int ret = UINT_MAX;
    for (unsigned int index = 0; index < configs.get_numOfInverter(); ++index) {
        if (slave_id == configs.get_Inv_addr(index)) {
            ret = index;
            break;
        }
    }
    return ret;
}

void debug_tcp_inverter_address(unsigned int slave_id, unsigned int address)
{
    unsigned int index = debug_get_index(slave_id);
    if (UINT_MAX == index) {
        std::cout << "Wrong slave id " << slave_id << " in debug_inverter_address\n";
        return;
    }

    char device[16] = {0};
    sInvConfig config = configs.get_Inv_config(index);

    modbus_t *ctx = nullptr;
    getDeviceName(config.port, device);

    std::string slave_ip("192.168.0.1");
    ctx = modbus_new_tcp(slave_ip.c_str(), 502);
    if (ctx == nullptr) {
        std::cout << "Open " << device << " fail\n";
        return;
    }
    else if (modbus_connect(ctx) == -1) {
        std::cout << "Connect to " << device << " fail\n";
        modbus_free(ctx);
        return;
    }

    uint16_t registers[4];
    modbus_set_slave(ctx, config.address);

    usleep(50000);
    std::cout << "[" << std::hex << address << "] = ";
    if (debugs.is_hodingRegister())
    {
        if (modbus_read_registers(ctx, address, 1, registers) == 1)
            std::cout << registers[0] << std::endl;
        else
            std::cout << "invalid" << std::endl;
    }
    else
    {
        if (modbus_read_input_registers(ctx, address, 1, registers) == 1)
            std::cout << registers[0] << std::endl;
        else
            std::cout << "invalid" << std::endl;
    }
    std::cout << std::dec;
    modbus_close(ctx);
    modbus_free(ctx);
}

void debug_inverter_address(unsigned int slave_id, unsigned int address)
{
    unsigned int index = debug_get_index(slave_id);
    if (UINT_MAX == index) {
        std::cout << "Wrong slave id " << slave_id << " in debug_inverter_address\n";
        return;
    }

    if (TCP == debugs.get_mode()) {
        debug_tcp_inverter_address(slave_id, address);
        return;
    }

    char device[16] = {0};
    sInvConfig config = configs.get_Inv_config(index);

    getDeviceName(config.port, device);
    modbus_t *ctx = modbus_new_rtu(device, config.baud, 'N', 8, 1);
    if (ctx == nullptr) {
        std::cout << "Open " << device << " fail\n";
        return;
    }
    else if (modbus_connect(ctx) == -1) {
        std::cout << "Connect to " << device << " fail\n";
        modbus_free(ctx);
        return;
    }
    modbus_set_slave(ctx, config.address);

    uint16_t registers[1];
    usleep(50000);
    std::cout << "[" << std::hex << address << "] = ";
    if (debugs.is_hodingRegister())
    {
        if (modbus_read_registers(ctx, address, 1, registers) == 1)
            std::cout << registers[0] << std::endl;
        else
            std::cout << "invalid" << std::endl;
    }
    else
    {
        if (modbus_read_input_registers(ctx, address, 1, registers) == 1)
            std::cout << registers[0] << std::endl;
        else
            std::cout << "invalid" << std::endl;
    }
    std::cout << std::dec;
    modbus_close(ctx);
    modbus_free(ctx);
}

void debug_inverter(unsigned int slave_id)
{
    unsigned int index = debug_get_index(slave_id);
    if (UINT_MAX == index) {
        std::cout << "Wrong slave id " << slave_id << " in debug_inverter\n";
        return;
    }
    INV_Field invField;
    INV_STATE invState;
    // get current time using time_t
    const std::time_t now = std::time(nullptr);
    //Get Inverter Data
    sInvConfig config = configs.get_Inv_config(index);
    invField.clear();
    invField.SlaveId = config.address;
    invField.ts = now;
    sprintf(invField.KW, "%.2f", config.kw);
    sprintf(invField.KWR, "%.1f", config.kwr);
    strcpy(invField.SN, config.sn);
    invState = getInvData(config, &invField);

    std::string sId = std::to_string(invField.SlaveId);
    std::map<int, std::string> inv_status;
    inv_status.clear();
    switch (invState) {
        case INV_Logging:
            debug_print(&invField);
            break;
        case INV_ScanError: case INV_ModBusSelectTimoeout:
        case INV_ModBusCrcError: case INV_ModBusBadSlaveId: 
        case INV_ModBusDataError: case INV_Disconnect:
            inv_status[invState].append(sId + ' ');
            break;
        default:
            break;
    }
    inv_printerrors(inv_status);
}


void debug_inverters(void)
{
    INV_Field invField;
    INV_STATE invState;

    // get current time using time_t
    const std::time_t now = std::time(nullptr);

    //Get Inverter Data
    unsigned int okCount = 0;
    std::map<int, std::string> inv_status;
    inv_status.clear();
    for (unsigned int index = 0; index < configs.get_numOfInverter(); ++index) {
        sInvConfig config = configs.get_Inv_config(index);

        invField.clear();
        invField.SlaveId = config.address;
        invField.ts = now;
        sprintf(invField.KW, "%.2f", config.kw);
        sprintf(invField.KWR, "%.1f", config.kwr);
        strcpy(invField.SN, config.sn);
        invState = getInvData(config, &invField);

        std::string sId = std::to_string(invField.SlaveId);
        switch (invState) {
            case INV_Logging:
                debug_print(&invField);
                okCount++;
                break;
            case INV_ScanError: case INV_ModBusSelectTimoeout:
            case INV_ModBusCrcError: case INV_ModBusBadSlaveId: 
            case INV_ModBusDataError: case INV_Disconnect:
                inv_status[invState].append(sId + ' ');
                break;
            default:
                break;
        }
    }
    printf("\n# Ok/Total = %d/%d #\n", okCount, configs.get_numOfInverter());
    inv_printerrors(inv_status);
}

void debug_run()
{
    unsigned int slave_id = debugs.get_slave_id();
    unsigned int address = debugs.get_address();
    unsigned int count = debugs.get_count();
    unsigned int interval = debugs.get_interval();

    time_t next_interval = 0;
    for (unsigned int i = 0; i < count;) {
        const time_t now = std::time(nullptr);
        std::cout << "\nlocal: " << std::put_time(std::localtime(&now), "%c %Z") << '\n';

        if (now < next_interval) {
            sleep(1);
            continue;
        }
        else {
            i++;
            next_interval = now + interval * 60;
        }
        if (UINT_MAX == slave_id)
            debug_inverters();
        else if (UINT_MAX == address)
            debug_inverter(slave_id);
        else
            debug_inverter_address(slave_id, address);
    }
}