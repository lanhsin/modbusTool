# Modbus Trace Tool

### Enviroment

1. mkdir ~/.modbusTool
2. cp config ~/.modbusTool

### Support Command

1. modbusTool
   * Trace modbus transfers of all inverters

2. modbusTool -a [1-MAX_NUM]
   * Trace modbus transfer of the specific slave id

3. modbusTool -a [1-MAX_NUM] -r address -m rtu -t 4
   modbusTool -a [1-MAX_NUM] -r address -m tcp -t 4
   * Trace modbus transfer of the specific slave id and specific register address
   -m rtu : rtu mode (default)
   -m tcp : tcp mode

4. modbusTool -a [1-MAX_NUM] -r address -t 3
   modbusTool -a [1-MAX_NUM] -r address -t 4
   * Trace modbus transfer of the specific slave id and specific register address
   -t 3 : input register
   -t 4 : holding register (default)

5. -c [1-MAX_NUM]  
   Ex: modbusTool -a [1-MAX_NUM] -c [1-MAX_NUM]
   * The number of read (1 is default)

6. -o [1 - 10.0]
   Ex: modbusTool -a [1-MAX_NUM] -o [1 - 10.0]
   Time-out in seconds (1 - 10.0, 1.0 s is default)

### Example

1. modbusTool

2. modbusTool -a 1

3. modbusTool -a 1 -r 6672 -m rtu -t 3
   [1a10] = 3631
