#include "Set_coil.h"
// #include <libmaple/iwdg.h>
#include "Modbus.h"
#include "ModbusSerial.h"
#include "fun_periph.h"
 #include "Memory.h"
// #include "Command_Analysis.h"
// #include "receipt.h"
// #include "Private_Timer.h"
// #include "fun_periph.h"
// #include "LoRa.h"
#include<math.h>

Modbus_Coils Modbus_Coil;
ModbusSerial mb;

/*
 @brief   : 进行modbus库的配置
 @para    : None
 @return  : None
 */
void Modbus_Coils::Modbus_Config(void)
{
	// Config Modbus Serial(port, speed, byte format)
	mb.config(&Serial1, 9600, SERIAL_8N1);
	// Set the Slave ID (1-247)
	mb.setSlaveId(1);

	// Add register
	mb.addCoil(KCZJ1_COIL);
	mb.addCoil(KCZJ2_COIL);
	mb.addCoil(KCZJ3_COIL);
	mb.addCoil(KCZJ4_COIL);
	mb.addCoil(KCZJ5_COIL);
	mb.addCoil(KCZJ6_COIL);
	mb.addCoil(KCZJ7_COIL);
	mb.addCoil(KCZJ8_COIL);
	mb.addCoil(KCZJ9_COIL);
	mb.addCoil(KCZJ10_COIL);
	mb.addCoil(KCZJ11_COIL);
	mb.addCoil(KCZJ12_COIL);

	mb.addCoil(KCZJ13_COIL);
	mb.addCoil(KCZJ14_COIL);
	mb.addCoil(KCZJ15_COIL);
	mb.addCoil(KCZJ16_COIL);
	mb.addCoil(KCZJ17_COIL);
	mb.addCoil(KCZJ18_COIL);
	mb.addCoil(KCZJ19_COIL);
	mb.addCoil(KCZJ20_COIL);
	mb.addCoil(KCZJ21_COIL);
	mb.addCoil(KCZJ22_COIL);
	mb.addCoil(KCZJ23_COIL);
	mb.addCoil(KCZJ24_COIL);

	mb.addCoil(KCZJ25_COIL);
	mb.addCoil(KCZJ26_COIL);
	mb.addCoil(KCZJ27_COIL);
	mb.addCoil(KCZJ28_COIL);
	mb.addCoil(KCZJ29_COIL);
	mb.addCoil(KCZJ30_COIL);
	mb.addCoil(KCZJ31_COIL);
	mb.addCoil(KCZJ32_COIL);
	mb.addCoil(KCZJ33_COIL);
	mb.addCoil(KCZJ34_COIL);
	mb.addCoil(KCZJ35_COIL);
	mb.addCoil(KCZJ36_COIL);

	mb.addCoil(KCZJ37_COIL);
	mb.addCoil(KCZJ38_COIL);
	mb.addCoil(KCZJ39_COIL);
	mb.addCoil(KCZJ40_COIL);
	mb.addCoil(KCZJ41_COIL);
	mb.addCoil(KCZJ42_COIL);
	mb.addCoil(KCZJ43_COIL);
	mb.addCoil(KCZJ44_COIL);
	mb.addCoil(KCZJ45_COIL);
	mb.addCoil(KCZJ46_COIL);
	mb.addCoil(KCZJ47_COIL);
	mb.addCoil(KCZJ48_COIL);

	mb.addCoil(KCZJ49_COIL);
	mb.addCoil(KCZJ50_COIL);
	mb.addCoil(KCZJ51_COIL);
	mb.addCoil(KCZJ52_COIL);
	mb.addCoil(KCZJ53_COIL);
	mb.addCoil(KCZJ54_COIL);
	mb.addCoil(KCZJ55_COIL);
	mb.addCoil(KCZJ56_COIL);
	mb.addCoil(KCZJ57_COIL);
	mb.addCoil(KCZJ58_COIL);
	mb.addCoil(KCZJ59_COIL);
	mb.addCoil(KCZJ60_COIL);

	mb.addCoil(KCZJ61_COIL);
	mb.addCoil(KCZJ62_COIL);
	mb.addCoil(KCZJ63_COIL);
	mb.addCoil(KCZJ64_COIL);

	mb.addIsts(DI1_ISTS);
	mb.addIsts(DI2_ISTS);
	mb.addIsts(DI3_ISTS);
	mb.addIsts(DI4_ISTS);
	mb.addIsts(DI5_ISTS);
	mb.addIsts(DI6_ISTS);
	mb.addIsts(DI7_ISTS);
	mb.addIsts(DI8_ISTS);

	mb.addIreg(AI1_IREG);
	mb.addIreg(AI2_IREG);
	mb.addIreg(AI3_IREG);
	mb.addIreg(AI4_IREG);
	mb.addIreg(AI5_IREG);
	mb.addIreg(AI6_IREG);
	mb.addIreg(AI7_IREG);
	mb.addIreg(AI8_IREG);

	mb.addHreg(Variable_1, 127);
}

/*
 @brief   : 设置输出线圈默认值
 @param   : void
 @return  : 设置成功返回true，失败返回false
 */
bool Modbus_Coils::Set_Coil_DefaultValue(void)
{
	unsigned char count = 0;
	if(mb.Coil(KCZJ1_COIL,0xFF00) == true)	count++;
	if(mb.Coil(KCZJ2_COIL,0xFF00) == true)	count++;
	if(mb.Coil(KCZJ3_COIL,0xFF00) == true)	count++;
	if(mb.Coil(KCZJ4_COIL,0xFF00) == true)	count++;
	if(mb.Coil(KCZJ5_COIL,0xFF00) == true)	count++;
	if(mb.Coil(KCZJ6_COIL,0xFF00) == true)	count++;
	if(mb.Coil(KCZJ7_COIL,0xFF00) == true)	count++;
	if(mb.Coil(KCZJ8_COIL,0xFF00) == true)	count++;
	if(mb.Coil(KCZJ9_COIL,0xFF00) == true)	count++;
	if(mb.Coil(KCZJ10_COIL,0xFF00) == true)	count++;
	if(mb.Coil(KCZJ11_COIL,0xFF00) == true)	count++;
	if(mb.Coil(KCZJ12_COIL,0xFF00) == true)	count++;

    

	if (count == 12)
	{
		unsigned char *_empty = NULL;
		Modbus_Coil.Modbus_Realization(_empty, 0);//设置输出线圈状态，modbus实现
		Serial.println("Set Coil DefaultValue Success. <Set_Coil_DefaultValue>");
		return true;
	}
	else
	{
		Serial.println("Set Coil DefaultValue Failure! <Set_Coil_DefaultValue>");
		return false;
	}
}

/*
 @brief   : 设置输出线圈初始值
 @param   : void
 @return  : 设置成功返回true，失败返回false
 */
bool Modbus_Coils::Set_Coil_InitValue(void)
{
	unsigned char DO_Init[8] = { 0x00 };
	unsigned char *data = InitState.Read_DO_InitState();//读取DO的初始化状态
	//Serial.println("...........");
	for (size_t i = 0; i < 8; i++)
	{
		//Serial.println(*(data+i));
		DO_Init[i] = *(data + i);
		//Serial.println(DO_Init[i],HEX);
	}
#if PLC_V1	//只有12个DO输出

	if (Init_DO_1to8(DO_Init[0]) && Init_DO_9to16(DO_Init[1]))
	{
		unsigned char *_empty = NULL;
		Modbus_Coil.Modbus_Realization(_empty, 0);//设置输出线圈状态，modbus实现
		Serial.println("PLC_V1 DO外设引脚初始化成功<Set_Coil_InitValue>");
		return true;
	}
	
	return false;
#elif PLC_V2

#endif // PLC_Vx	
}

/*
 @brief   : 设置输出线圈状态，modbus实现
 @param   : 1.一个数组指针(如果不需要使用数组，传入一个指向NULL的指针)
			2.数组的长度(如果传入的是指向NULL的指针，该参数填0)
 @return  : void
 */
void Modbus_Coils::Modbus_Realization(unsigned char* modbusPacket, int Length)
{
	mb.task(modbusPacket,Length);

    digitalWrite(KCZJ1, !mb.Coil(KCZJ1_COIL));
    digitalWrite(KCZJ2, !mb.Coil(KCZJ2_COIL));
    digitalWrite(KCZJ3, !mb.Coil(KCZJ3_COIL));
    digitalWrite(KCZJ4, !mb.Coil(KCZJ4_COIL));
    digitalWrite(KCZJ5, !mb.Coil(KCZJ5_COIL));
    digitalWrite(KCZJ6, !mb.Coil(KCZJ6_COIL));
    digitalWrite(KCZJ7, !mb.Coil(KCZJ7_COIL));
    digitalWrite(KCZJ8, !mb.Coil(KCZJ8_COIL));
    digitalWrite(KCZJ9, !mb.Coil(KCZJ9_COIL));
    digitalWrite(KCZJ10, !mb.Coil(KCZJ10_COIL));
    digitalWrite(KCZJ11, !mb.Coil(KCZJ11_COIL));
    digitalWrite(KCZJ12, !mb.Coil(KCZJ12_COIL));
	
	/*Serial.println("----------------------");
	delay(1000);
	Serial.println(String("mb.Coil(KCZJ1_COIL) = ") + mb.Coil(KCZJ1_COIL));
	Serial.println(String("mb.Coil(KCZJ2_COIL) = ") + mb.Coil(KCZJ2_COIL));
	Serial.println(String("mb.Coil(KCZJ3_COIL) = ") + mb.Coil(KCZJ3_COIL));
	Serial.println(String("mb.Coil(KCZJ4_COIL) = ") + mb.Coil(KCZJ4_COIL));
	Serial.println(String("mb.Coil(KCZJ5_COIL) = ") + mb.Coil(KCZJ5_COIL));
	Serial.println(String("mb.Coil(KCZJ6_COIL) = ") + mb.Coil(KCZJ6_COIL));
	Serial.println(String("mb.Coil(KCZJ7_COIL) = ") + mb.Coil(KCZJ7_COIL));
	Serial.println(String("mb.Coil(KCZJ8_COIL) = ") + mb.Coil(KCZJ8_COIL));
	Serial.println(String("mb.Coil(KCZJ9_COIL) = ") + mb.Coil(KCZJ9_COIL));
	Serial.println(String("mb.Coil(KCZJ10_COIL) = ") + mb.Coil(KCZJ10_COIL));
	Serial.println(String("mb.Coil(KCZJ11_COIL) = ") + mb.Coil(KCZJ11_COIL));
	Serial.println(String("mb.Coil(KCZJ12_COIL) = ") + mb.Coil(KCZJ12_COIL));
	Serial.println(String("mb.Coil(KCZJ13_COIL) = ") + mb.Coil(KCZJ13_COIL));
	Serial.println(String("mb.Coil(KCZJ14_COIL) = ") + mb.Coil(KCZJ14_COIL));
	Serial.println(String("mb.Coil(KCZJ15_COIL) = ") + mb.Coil(KCZJ15_COIL));
	Serial.println(String("mb.Coil(KCZJ16_COIL) = ") + mb.Coil(KCZJ16_COIL));*/

    mb.Ists(DI1_ISTS, digitalRead(DI1));
    mb.Ists(DI2_ISTS, digitalRead(DI2));
    mb.Ists(DI3_ISTS, digitalRead(DI3));
    mb.Ists(DI4_ISTS, digitalRead(DI4));
    mb.Ists(DI5_ISTS, digitalRead(DI5));
    mb.Ists(DI6_ISTS, digitalRead(DI6));
    mb.Ists(DI7_ISTS, digitalRead(DI7));
    mb.Ists(DI8_ISTS, digitalRead(DI8));

    mb.Ireg(AI1_IREG, analogRead(AI1));
    mb.Ireg(AI2_IREG, analogRead(AI2));
    mb.Ireg(AI3_IREG, analogRead(AI3));
    mb.Ireg(AI4_IREG, analogRead(AI4));
    mb.Ireg(AI5_IREG, analogRead(AI5));
    mb.Ireg(AI6_IREG, analogRead(AI6));
    mb.Ireg(AI7_IREG, analogRead(AI7));
    mb.Ireg(AI8_IREG, analogRead(AI8));
}


/*
 @brief   : DO1-DO8的线圈初始化
 @param   : void
 @return  : void
 */
bool Modbus_Coils::Init_DO_1to8(unsigned char num)
{
	Serial.println("开始设置线圈值<Init_DO_1to8>");
	unsigned char n1, n = 0; unsigned char count = 0;
	String DO_Init_1 = String(num, BIN);/* String DO_Init_2 = String(DO_Init[1], BIN);*/
	Serial.println(String("DO_Init_1 = ") + DO_Init_1); /*Serial.println(String("DO_Init_2 = ") + DO_Init_2);*/
	if (DO_Init_1.length() == 8)
	{
		if (mb.Coil(KCZJ1_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ2_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ3_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ4_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ5_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ6_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ7_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ8_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
	}
	else if (DO_Init_1.length() == 7)
	{
		if (mb.Coil(KCZJ1_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ2_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ3_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ4_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ5_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ6_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ7_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ8_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
	}
	else if (DO_Init_1.length() == 6)
	{
		if (mb.Coil(KCZJ1_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ2_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ3_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ4_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ5_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ6_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ7_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ8_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
	}
	else if (DO_Init_1.length() == 5)
	{
		if (mb.Coil(KCZJ1_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ2_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ3_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ4_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ5_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ6_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ7_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ8_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
	}
	else if (DO_Init_1.length() == 4)
	{
		if (mb.Coil(KCZJ1_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ2_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ3_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ4_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ5_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ6_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ7_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ8_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
	}
	else if (DO_Init_1.length() == 3)
	{
		if (mb.Coil(KCZJ1_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ2_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ3_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ4_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ5_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ6_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ7_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ8_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
	}
	else if (DO_Init_1.length() == 2)
	{
		if (mb.Coil(KCZJ1_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ2_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ3_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ4_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ5_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ6_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ7_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ8_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
	}
	else if (DO_Init_1.length() == 1)
	{
		if (mb.Coil(KCZJ1_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ2_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ3_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ4_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ5_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ6_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ7_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ8_COIL, (DO_Init_1.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_1.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
	}
	else
	{
		return false;
	}


	if (count == 8)
	{
		Serial.println("设置线圈值成功<Init_DO_1to8>");
		return true;
	}
	Serial.println("设置线圈值失败<Init_DO_1to8>");
	return false;
}

/*
 @brief   : DO9-DO16的线圈初始化
 @param   : void
 @return  : void
 */
bool Modbus_Coils::Init_DO_9to16(unsigned char num)
{
	Serial.println("开始设置线圈值<Init_DO_9to16>");
	unsigned char n1, n = 0; unsigned char count = 0;
	String DO_Init_2 = String(num, BIN);
	Serial.println(String("DO_Init_2 = ") + DO_Init_2);
	if (DO_Init_2.length() == 8)
	{
		if (mb.Coil(KCZJ9_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ10_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ11_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ12_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		//if (mb.Coil(KCZJ13_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		//if (mb.Coil(KCZJ14_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		//if (mb.Coil(KCZJ15_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		//if (mb.Coil(KCZJ16_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
	}
	else if (DO_Init_2.length() == 7)
	{
		if (mb.Coil(KCZJ9_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ10_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ11_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ12_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		//if (mb.Coil(KCZJ13_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		//if (mb.Coil(KCZJ14_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		//if (mb.Coil(KCZJ15_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		//if (mb.Coil(KCZJ16_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
	}
	else if (DO_Init_2.length() == 6)
	{
		if (mb.Coil(KCZJ9_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ10_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ11_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		if (mb.Coil(KCZJ12_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		//if (mb.Coil(KCZJ13_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		//if (mb.Coil(KCZJ14_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		//if (mb.Coil(KCZJ15_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		//if (mb.Coil(KCZJ16_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
	}
	else if (DO_Init_2.length() == 5)
	{
		if (mb.Coil(KCZJ9_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ10_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ11_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ12_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		//if (mb.Coil(KCZJ13_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		//if (mb.Coil(KCZJ14_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		//if (mb.Coil(KCZJ15_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		//if (mb.Coil(KCZJ16_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
	}
	else if (DO_Init_2.length() == 4)
	{
		if (mb.Coil(KCZJ9_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ10_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ11_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ12_COIL, 0x0000) == true)	count++;
		//if (mb.Coil(KCZJ13_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		//if (mb.Coil(KCZJ14_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		//if (mb.Coil(KCZJ15_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		//if (mb.Coil(KCZJ16_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
	}
	else if (DO_Init_2.length() == 3)
	{
		if (mb.Coil(KCZJ9_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ10_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ11_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ12_COIL, 0x0000) == true)	count++;
		//if (mb.Coil(KCZJ13_COIL, 0x0000) == true)	count++;
		//if (mb.Coil(KCZJ14_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		//if (mb.Coil(KCZJ15_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		//if (mb.Coil(KCZJ16_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
	}
	else if (DO_Init_2.length() == 2)
	{
		if (mb.Coil(KCZJ9_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ10_COIL, 0xFF00) == true)	count++;
		if (mb.Coil(KCZJ11_COIL, 0xFF00) == true)	count++;
		if (mb.Coil(KCZJ12_COIL, 0xFF00) == true)	count++;
		//if (mb.Coil(KCZJ13_COIL, 0xFF00) == true)	count++;
		//if (mb.Coil(KCZJ14_COIL, 0xFF00) == true)	count++;
		//if (mb.Coil(KCZJ15_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
		//if (mb.Coil(KCZJ16_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
	}
	else if (DO_Init_2.length() == 1)
	{
		if (mb.Coil(KCZJ9_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ10_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ11_COIL, 0x0000) == true)	count++;
		if (mb.Coil(KCZJ12_COIL, 0x0000) == true)	count++;
		//if (mb.Coil(KCZJ13_COIL, 0x0000) == true)	count++;
		//if (mb.Coil(KCZJ14_COIL, 0x0000) == true)	count++;
		//if (mb.Coil(KCZJ15_COIL, 0x0000) == true)	count++;
		//if (mb.Coil(KCZJ16_COIL, (DO_Init_2.substring(n1++, n1) > String(0) ? 0xFF00 : 0x0000)) == true)	count++/*Serial.println((DO_Init_2.substring(n++, n) > String(0) ? 0xFF00 : 0x0000), HEX)*/;
	}
	else
	{
		return false;
	}


	if (count == 4)
	{
		Serial.println("设置线圈值成功<Init_DO_9to16>");
		return true;
	}
	Serial.println("设置线圈值失败<Init_DO_9to16>");
	return false;
}

/*
 @brief   : DO17-DO824的线圈初始化
 @param   : void
 @return  : void
 */
bool Modbus_Coils::Init_DO_17to24(unsigned char num)
{
	Serial.println("<Init_DO_9to16>");
}

/*
 @brief   : DO25-DO32的线圈初始化
 @param   : void
 @return  : void
 */
bool Modbus_Coils::Init_DO_25to32(unsigned char num)
{
	Serial.println("<Init_DO_9to16>");
}

/*
 @brief   : DO33-DO40的线圈初始化
 @param   : void
 @return  : void
 */
bool Modbus_Coils::Init_DO_33to40(unsigned char num)
{
	Serial.println("<Init_DO_9to16>");
}

/*
 @brief   : DO41-DO48的线圈初始化
 @param   : void
 @return  : void
 */
bool Modbus_Coils::Init_DO_41to48(unsigned char num)
{
	Serial.println("<Init_DO_9to16>");
}

/*
 @brief   : DO49-DO56的线圈初始化
 @param   : void
 @return  : void
 */
bool Modbus_Coils::Init_DO_49to56(unsigned char num)
{
	Serial.println("<Init_DO_49to56>");
}

/*
 @brief   : DO1-DO8的线圈初始化
 @param   : void
 @return  : void
 */
bool Modbus_Coils::Init_AO_1to8()
{
	Serial.println("<Init_AO_1to8>");
}

/*
 @brief   : 得到DI1-8的值
 @param   : void
 @return  : DI1to8
 */
unsigned char Modbus_Coils::Get_DI_1to8(void)
{
	unsigned char DI1to8 = 0x00;
	for (size_t i = 0; i < 8; i++)
	{
		switch (i)
		{
		case 0: digitalRead(DI1) == HIGH ? DI1to8 += 0 : DI1to8 += 128;
			Serial.println(String("DI1to8 = ") + DI1to8); break;
		case 1:	digitalRead(DI2) == HIGH ? DI1to8 += 0 : DI1to8 += 64;
			Serial.println(String("DI1to8 = ") + DI1to8); break;
		case 2: digitalRead(DI3) == HIGH ? DI1to8 += 0 : DI1to8 += 32;
			Serial.println(String("DI1to8 = ") + DI1to8); break;
		case 3:	digitalRead(DI4) == HIGH ? DI1to8 += 0 : DI1to8 += 16;
			Serial.println(String("DI1to8 = ") + DI1to8); break;
		case 4: digitalRead(DI5) == HIGH ? DI1to8 += 0 : DI1to8 += 8;
			Serial.println(String("DI1to8 = ") + DI1to8); break;
		case 5:	digitalRead(DI6) == HIGH ? DI1to8 += 0 : DI1to8 += 4;
			Serial.println(String("DI1to8 = ") + DI1to8); break;
		case 6: digitalRead(DI7) == HIGH ? DI1to8 += 0 : DI1to8 += 2;
			Serial.println(String("DI1to8 = ") + DI1to8); break;
		case 7:	digitalRead(DI8) == HIGH ? DI1to8 += 0 : DI1to8 += 1;
			Serial.println(String("DI1to8 = ") + DI1to8); break;
		default:
			break;
		}
	}
	
	return DI1to8;
}

/*
 @brief   : 得到DO1-8的值
 @param   : void
 @return  : DO1to8
 */
unsigned char Modbus_Coils::Get_DO_1to8(void)
{
	unsigned char DO1to8 = 0x00;
	for (size_t i = 0; i < 8; i++)
	{
		switch (i)
		{
		case 0: digitalRead(KCZJ1) == HIGH ? DO1to8 += 0 : DO1to8 += 128;
			Serial.println(String("DO1to8 = ") + DO1to8); break;
		case 1:	digitalRead(KCZJ2) == HIGH ? DO1to8 += 0 : DO1to8 += 64;
			Serial.println(String("DO1to8 = ") + DO1to8); break;
		case 2: digitalRead(KCZJ3) == HIGH ? DO1to8 += 0 : DO1to8 += 32;
			Serial.println(String("DO1to8 = ") + DO1to8); break;
		case 3:	digitalRead(KCZJ4) == HIGH ? DO1to8 += 0 : DO1to8 += 16;
			Serial.println(String("DO1to8 = ") + DO1to8); break;
		case 4: digitalRead(KCZJ5) == HIGH ? DO1to8 += 0 : DO1to8 += 8;
			Serial.println(String("DO1to8 = ") + DO1to8); break;
		case 5:	digitalRead(KCZJ6) == HIGH ? DO1to8 += 0 : DO1to8 += 4;
			Serial.println(String("DO1to8 = ") + DO1to8); break;
		case 6: digitalRead(KCZJ7) == HIGH ? DO1to8 += 0 : DO1to8 += 2;
			Serial.println(String("DO1to8 = ") + DO1to8); break;
		case 7:	digitalRead(KCZJ8) == HIGH ? DO1to8 += 0 : DO1to8 += 1;
			Serial.println(String("DO1to8 = ") + DO1to8); break;
		default:
			break;
		}
	}

	return DO1to8;
}

/*
 @brief   : 得到DO9-16的值
 @param   : void
 @return  : DO9to16
 */
unsigned char Modbus_Coils::Get_DO_9to16(void)
{
	unsigned char DO9to16 = 0x00;
#if PLC_V1
	for (size_t i = 0; i < 8; i++)
	{
		switch (i)
		{
		case 0: digitalRead(KCZJ9) == HIGH ? DO9to16 += 0 : DO9to16 += 128;
			Serial.println(String("DO9to16 = ") + DO9to16); break;
		case 1:	digitalRead(KCZJ10) == HIGH ? DO9to16 += 0 : DO9to16 += 64;
			Serial.println(String("DO9to16 = ") + DO9to16); break;
		case 2: digitalRead(KCZJ11) == HIGH ? DO9to16 += 0 : DO9to16 += 32;
			Serial.println(String("DO9to16 = ") + DO9to16); break;
		case 3:	digitalRead(KCZJ12) == HIGH ? DO9to16 += 0 : DO9to16 += 16;
			Serial.println(String("DO9to16 = ") + DO9to16); break;
		case 4: DO9to16 += 0;
			Serial.println(String("DO9to16 = ") + DO9to16); break;
		case 5:	DO9to16 += 0;
			Serial.println(String("DO9to16 = ") + DO9to16); break;
		case 6: DO9to16 += 0;
			Serial.println(String("DO9to16 = ") + DO9to16); break;
		case 7:	DO9to16 += 0;
			Serial.println(String("DO9to16 = ") + DO9to16); break;
		default: break;
		}
	}
#elif PLC_V2
	switch (i)
	{
	case 0: digitalRead(KCZJ9) == HIGH ? DI1to8 += 0 : DI1to8 += 128;
		Serial.println(String("DO1to8 = ") + DI1to8); break;
	case 1:	digitalRead(KCZJ10) == HIGH ? DI1to8 += 0 : DI1to8 += 64;
		Serial.println(String("DO1to8 = ") + DI1to8); break;
	case 2: digitalRead(KCZJ11) == HIGH ? DI1to8 += 0 : DI1to8 += 32;
		Serial.println(String("DO1to8 = ") + DI1to8); break;
	case 3:	digitalRead(KCZJ12) == HIGH ? DI1to8 += 0 : DI1to8 += 16;
		Serial.println(String("DO1to8 = ") + DI1to8); break;
	case 4: digitalRead(KCZJ13) == HIGH ? DI1to8 += 0 : DI1to8 += 8;
		Serial.println(String("DO1to8 = ") + DI1to8); break;
	case 5:	digitalRead(KCZJ14) == HIGH ? DI1to8 += 0 : DI1to8 += 4;
		Serial.println(String("DO1to8 = ") + DI1to8); break;
	case 6: digitalRead(KCZJ15) == HIGH ? DI1to8 += 0 : DI1to8 += 2;
		Serial.println(String("DO1to8 = ") + DI1to8); break;
	case 7:	digitalRead(KCZJ16) == HIGH ? DI1to8 += 0 : DI1to8 += 1;
		Serial.println(String("DO1to8 = ") + DI1to8); break;
	default:
		break;
	}
#endif

	return DO9to16;
}

/*
 @brief   : 得到AI1-8的值
 @param   : void
 @return  :
 */
unsigned char * Modbus_Coils::Get_AI_1to8(void)
{
#if PLC_V1
	static unsigned char AI_1to8[16] = {0x00};
	int x[8] = {0x00};
	x[0] = analogRead(AI1);
	x[1] = analogRead(AI2);
	x[2] = analogRead(AI3);
	x[3] = analogRead(AI4);
	x[4] = analogRead(AI5);
	x[5] = analogRead(AI6);
	x[6] = analogRead(AI7);
	x[7] = analogRead(AI8);

	//x[0] = 488;//0000 0001 1110 1000
	//AI_1to8[0] = x[0] >> 8;
	//Serial.println(String("AI_1to8[0] = ") + String(AI_1to8[0], BIN));
	//AI_1to8[1] = x[0];
	//Serial.println(String("AI_1to8[1] = ") + String(AI_1to8[1], BIN));

	for (size_t i = 0; i < 8; i++)
	{
		AI_1to8[2*i] = x[i] >> 8;
		AI_1to8[(2*i)+1] = x[i];
	}
	return AI_1to8;
#elif PLC_V2

#endif // PLC_V1
}
