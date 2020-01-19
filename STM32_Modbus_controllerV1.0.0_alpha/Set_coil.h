#ifndef _SET_COIL_H
#define _SET_COIL_H

#include <Arduino.h>

// Modbus Registers Offsets (0-9999)
const int KCZJ1_COIL = 0; 	//继电器1
const int KCZJ2_COIL = 1; 	//继电器2
const int KCZJ3_COIL = 2; 	//继电器3
const int KCZJ4_COIL = 3; 	//继电器4
const int KCZJ5_COIL = 4; 	//继电器5
const int KCZJ6_COIL = 5; 	//继电器6
const int KCZJ7_COIL = 6; 	//继电器7
const int KCZJ8_COIL = 7; 	//继电器8
const int KCZJ9_COIL = 8; 	//继电器9
const int KCZJ10_COIL = 9; 	//继电器10
const int KCZJ11_COIL = 10; //继电器11
const int KCZJ12_COIL = 11; //继电器12

const int DI1_ISTS = 100; 	//数字输入1
const int DI2_ISTS = 101; 	//数字输入2
const int DI3_ISTS = 102; 	//数字输入3
const int DI4_ISTS = 103; 	//数字输入4
const int DI5_ISTS = 104; 	//数字输入5
const int DI6_ISTS = 105; 	//数字输入6
const int DI7_ISTS = 106; 	//数字输入7
const int DI8_ISTS = 107; 	//数字输入8

const int AI1_IREG = 200; 	//模拟输入1
const int AI2_IREG = 201; 	//模拟输入2
const int AI3_IREG = 202; 	//模拟输入3
const int AI4_IREG = 203; 	//模拟输入4
const int AI5_IREG = 204; 	//模拟输入5
const int AI6_IREG = 205; 	//模拟输入6
const int AI7_IREG = 206; 	//模拟输入7
const int AI8_IREG = 207; 	//模拟输入8

const int Variable_1 = 300; //变量


class Modbus_Coils {
public:
    void Modbus_Config(void);           //进行modbus库的配置
    bool Set_Coil_DefaultValue(void);	//设置输出线圈默认值
	bool Set_Coil_InitValue(void);		//设置输出线圈初始值
    void Modbus_Realization(void);      //Modbus的实现函数
};

extern Modbus_Coils Modbus_Coil;

#endif