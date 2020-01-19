#include "Set_coil.h"
// #include <libmaple/iwdg.h>
#include "Modbus.h"
#include "ModbusSerial.h"
#include "fun_periph.h"
// #include "Memory.h"
// #include "Command_Analysis.h"
// #include "receipt.h"
// #include "Private_Timer.h"
// #include "fun_periph.h"
// #include "LoRa.h"

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
        Modbus_Coil.Modbus_Realization();
		return true;
	}
	else
	{
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
	
}

/*
 @brief   : 设置输出线圈初始值
 @param   : void
 @return  : 设置成功返回true，失败返回false
 */
void Modbus_Coils::Modbus_Realization(void)
{
	mb.task();

    digitalWrite(KCZJ1, mb.Coil(KCZJ1_COIL));
    digitalWrite(KCZJ2, mb.Coil(KCZJ2_COIL));
    digitalWrite(KCZJ3, mb.Coil(KCZJ3_COIL));
    digitalWrite(KCZJ4, mb.Coil(KCZJ4_COIL));
    digitalWrite(KCZJ5, mb.Coil(KCZJ5_COIL));
    digitalWrite(KCZJ6, mb.Coil(KCZJ6_COIL));
    digitalWrite(KCZJ7, mb.Coil(KCZJ7_COIL));
    digitalWrite(KCZJ8, mb.Coil(KCZJ8_COIL));
    digitalWrite(KCZJ9, mb.Coil(KCZJ9_COIL));
    digitalWrite(KCZJ10, mb.Coil(KCZJ10_COIL));
    digitalWrite(KCZJ11, mb.Coil(KCZJ11_COIL));
    digitalWrite(KCZJ12, mb.Coil(KCZJ12_COIL));

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