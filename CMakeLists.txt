cmake_minimum_required(VERSION 3.17)


# Project declaration
project(modbusTool)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)


set(common_srcs
    common/common.cpp
    common/network.cpp
    )

set(inverter_srcs
    inverter/debug.cpp
    inverter/config.cpp
    inverter/inv.cpp
    inverter/simdev.cpp
    inverter/inverters.cpp
    )

set(libmodbus_srcs
    lib/modbus/modbus.c
    lib/modbus/modbus-data.c
    lib/modbus/modbus-rtu.c
    lib/modbus/modbus-tcp.c
)
add_library(modbus STATIC ${libmodbus_srcs})

add_executable(modbusTool main.cpp ${common_srcs} ${inverter_srcs})
target_compile_options(modbusTool PUBLIC -Wall)
target_include_directories(modbusTool PUBLIC include)
set_target_properties(modbusTool PROPERTIES LINK_FLAGS_RELEASE -s)
target_link_libraries(modbusTool modbus -L${CMAKE_CURRENT_SOURCE_DIR}/lib)

install(TARGETS modbusTool DESTINATION /usr/local/bin)


