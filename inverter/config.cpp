#include "config.h"
#include <nlohmann/json.hpp>
#include <unistd.h> // getuid
#include <pwd.h> // getpwuid
#include <sys/stat.h> // stat
#include <dirent.h> // readdir
#include <fstream>  // ifstream


const char* exec_name = "modbusTool";
std::string config_dirname = {};

// Initialize config directory name
bool config_init(void)
{
    config_dirname.append(getpwuid(getuid())->pw_dir);
    config_dirname.append("/.");
    config_dirname.append(exec_name);
    struct stat st = {0};
    int dir_err = 0; 
    if (stat(config_dirname.c_str(), &st) == -1)
        dir_err = mkdir(config_dirname.c_str(), 0744);
    
    if (dir_err == -1)
        return false;
    else
        return true;
}

// Get config directory name 
const char* config_getdir(void)
{
    return config_dirname.c_str();
}

void config_get_filename(char *path, const char* file) 
{
    strcpy(path, config_dirname.c_str());
    size_t len = strlen(path);
    path[len] = '/';
    strcpy(path + len + 1, file);
}

void to_json(nlohmann::json& j, const sInvConfig& s) {
    j = nlohmann::json{
            {"port", s.port},
            {"baud", s.baud},
            {"slaveId", s.address},
            {"brand", s.brand},
            {"mpp", s.mpp},
            {"phase", s.phase},
            {"kw", s.kw},
            {"kwr", s.kwr},
            {"sn", s.sn}};
}

void from_json(const nlohmann::json& j, sInvConfig& s) {
    j.at("port").get_to(s.port);
    j.at("baud").get_to(s.baud);
    j.at("slaveId").get_to(s.address);
    j.at("brand").get_to(s.brand);
    j.at("mpp").get_to(s.mpp);
    j.at("phase").get_to(s.phase);
    j.at("kw").get_to(s.kw);
    j.at("kwr").get_to(s.kwr);
    strcpy(s.sn, j.at("sn").get<std::string>().c_str());
}

int Configs::parsing_Json(void)
{
    std::string filename(config_getdir());
    filename.append("/config");
    std::ifstream i(filename);
    if(false == i.is_open())
        return -1;
    nlohmann::json json_body = nlohmann::json::parse(i);
    if (json_body.is_discarded())
        return -1;
    nlohmann::json j_plants = json_body["plants"];
    if (j_plants.is_array() == false)
        return -1;

    for (size_t i = 0;  i < j_plants.size(); i++) {
        nlohmann::json j_plant = j_plants.at(i);
        nlohmann::json j_devices = j_plant["devices"];
        //float price = j_plant.at("price").get<float>();
        if (j_devices.is_array() == false)
            return -1;
        for (size_t k = 0;  k < j_devices.size(); k++) {
            nlohmann::json j_device = j_devices.at(k);
            invConfigs[numOfInverter++] = j_device.get<sInvConfig>(); // Copy assignment
        }
    }
    return numOfInverter;
}