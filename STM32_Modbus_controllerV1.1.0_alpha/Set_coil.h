#ifndef _SET_COIL_H
#define _SET_COIL_H

#include <Arduino.h>

#define PLC_V1	true
#define PLC_V2	false


// Modbus Registers Offsets (0-9999)
const int KCZJ1_COIL = 0; 		//继电器1
const int KCZJ2_COIL = 1; 		//继电器2
const int KCZJ3_COIL = 2; 		//继电器3
const int KCZJ4_COIL = 3; 		//继电器4
const int KCZJ5_COIL = 4; 		//继电器5
const int KCZJ6_COIL = 5; 		//继电器6
const int KCZJ7_COIL = 6; 		//继电器7
const int KCZJ8_COIL = 7; 		//继电器8
const int KCZJ9_COIL = 8; 		//继电器9
const int KCZJ10_COIL = 9; 		//继电器10
const int KCZJ11_COIL = 10;		//继电器11
const int KCZJ12_COIL = 11;		//继电器12

const int KCZJ13_COIL = 12; 	//继电器13
const int KCZJ14_COIL = 13; 	//继电器14
const int KCZJ15_COIL = 14; 	//继电器15
const int KCZJ16_COIL = 15; 	//继电器16
const int KCZJ17_COIL = 16; 	//继电器17
const int KCZJ18_COIL = 17; 	//继电器18
const int KCZJ19_COIL = 18; 	//继电器19
const int KCZJ20_COIL = 19; 	//继电器20
const int KCZJ21_COIL = 20; 	//继电器21
const int KCZJ22_COIL = 21; 	//继电器22
const int KCZJ23_COIL = 22;		//继电器23
const int KCZJ24_COIL = 23;		//继电器24

const int KCZJ25_COIL = 24; 	//继电器25
const int KCZJ26_COIL = 25; 	//继电器26
const int KCZJ27_COIL = 26; 	//继电器27
const int KCZJ28_COIL = 27; 	//继电器28
const int KCZJ29_COIL = 28; 	//继电器29
const int KCZJ30_COIL = 29; 	//继电器30
const int KCZJ31_COIL = 30; 	//继电器31
const int KCZJ32_COIL = 31; 	//继电器32
const int KCZJ33_COIL = 32; 	//继电器33
const int KCZJ34_COIL = 33; 	//继电器34
const int KCZJ35_COIL = 34;		//继电器35
const int KCZJ36_COIL = 35;		//继电器36

const int KCZJ37_COIL = 36; 	//继电器37
const int KCZJ38_COIL = 37; 	//继电器38
const int KCZJ39_COIL = 38; 	//继电器39
const int KCZJ40_COIL = 39; 	//继电器40
const int KCZJ41_COIL = 40; 	//继电器41
const int KCZJ42_COIL = 41; 	//继电器42
const int KCZJ43_COIL = 42; 	//继电器43
const int KCZJ44_COIL = 43; 	//继电器44
const int KCZJ45_COIL = 44; 	//继电器45
const int KCZJ46_COIL = 45; 	//继电器46
const int KCZJ47_COIL = 46;		//继电器47
const int KCZJ48_COIL = 47;		//继电器48

const int KCZJ49_COIL = 48; 	//继电器49
const int KCZJ50_COIL = 49; 	//继电器50
const int KCZJ51_COIL = 50; 	//继电器51
const int KCZJ52_COIL = 51; 	//继电器52
const int KCZJ53_COIL = 52; 	//继电器53
const int KCZJ54_COIL = 53; 	//继电器54
const int KCZJ55_COIL = 54; 	//继电器55
const int KCZJ56_COIL = 55; 	//继电器56
const int KCZJ57_COIL = 56; 	//继电器57
const int KCZJ58_COIL = 57; 	//继电器58
const int KCZJ59_COIL = 58;		//继电器59
const int KCZJ60_COIL = 59;		//继电器60

const int KCZJ61_COIL = 60; 	//继电器61
const int KCZJ62_COIL = 61; 	//继电器62
const int KCZJ63_COIL = 62;		//继电器63
const int KCZJ64_COIL = 63;		//继电器64

const int DI1_ISTS = 0; 	//数字输入1
const int DI2_ISTS = 1; 	//数字输入2
const int DI3_ISTS = 2; 	//数字输入3
const int DI4_ISTS = 3; 	//数字输入4
const int DI5_ISTS = 4; 	//数字输入5
const int DI6_ISTS = 5; 	//数字输入6
const int DI7_ISTS = 6; 	//数字输入7
const int DI8_ISTS = 7; 	//数字输入8

const int AI1_IREG = 0; 	//模拟输入1
const int AI2_IREG = 1; 	//模拟输入2
const int AI3_IREG = 2; 	//模拟输入3
const int AI4_IREG = 3; 	//模拟输入4
const int AI5_IREG = 4; 	//模拟输入5
const int AI6_IREG = 5; 	//模拟输入6
const int AI7_IREG = 6; 	//模拟输入7
const int AI8_IREG = 7; 	//模拟输入8

const int Variable_1 = 0; //变量


class Modbus_Coils {
public:
    void Modbus_Config(void);           //进行modbus库的配置
    bool Set_Coil_DefaultValue(void);	//设置输出线圈默认值
	bool Set_Coil_InitValue(void);		//设置输出线圈初始值
    void Modbus_Realization(unsigned char* modbusPacket, int Length);      //Modbus的实现函数
	bool Init_DO_1to8(unsigned char num);		//DO1-DO8的线圈初始化
	bool Init_DO_9to16(unsigned char num);		//DO9-DO16的线圈初始化
	bool Init_DO_17to24(unsigned char num);		//DO17-DO24的线圈初始化
	bool Init_DO_25to32(unsigned char num);		//DO25-DO32的线圈初始化
	bool Init_DO_33to40(unsigned char num);		//DO33-DO40的线圈初始化
	bool Init_DO_41to48(unsigned char num);		//DO41-DO48的线圈初始化
	bool Init_DO_49to56(unsigned char num);		//DO49-DO56的线圈初始化
	bool Init_AO_1to8();		//AO1-AO8的线圈初始化
	unsigned char Get_DI_1to8(void);	//得到DI1-8的值
	unsigned char Get_DO_1to8(void);	//得到DO1-8的值
	unsigned char Get_DO_9to16(void);	//得到DO9-16的值
	unsigned char* Get_AI_1to8(void);	//得到AI1-8的值
};

extern Modbus_Coils Modbus_Coil;

#endif