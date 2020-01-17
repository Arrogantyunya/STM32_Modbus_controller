//
/*
    Name:       STM32_Modbus_controllerV1.0.0_alpha.ino
    Created:	  2019/12/30 星期一 10:53:34
    Author:     刘家辉
*/

/*
 * 主函数文件。
 * setup部分的功能有：复用调试引脚、开启独立看门狗、设备串口波特率、初始化各类总线
 * 工程模式、注册服务器申请、校正因突发断电的电机开度卷膜等。
 * loop部分的功能有：矫正开度误差、LoRa监听服务器指令、手动卷膜监测、定时自检参数等。
 *
 * 如有任何疑问，请发送邮件到： liujiahiu@qq.com
 */

#include <arduino.h>
#include <libmaple/pwr.h>
#include <libmaple/bkp.h>
#include <libmaple/iwdg.h>
#include <RTClock.h>

#include "Modbus.h"
#include "ModbusSerial.h"
#include "i2c.h"
#include "Command_Analysis.h"
#include "receipt.h"
#include "AT24CXX.h"
#include "Memory.h"
#include "LoRa.h"
#include "public.h"
#include "Security.h"
#include "fun_periph.h"
#include "Private_Timer.h"
#include "Private_RTC.h"

/* 测试宏 */
#define DEBUG_PRINT 	0 	//打印调试信息
#define Reset_RTC 		0   //重置rtc时间
#define Reset_Binding	0	//重新绑定
/* 使能宏 */
#define SOFT_HARD_VERSION	1 //使能写入软件和硬件版本
/* 替换宏 */
#define Software_version_high 	0x01 	//软件版本的高位
#define Software_version_low 	0x00  	//软件版本的低位
#define Hardware_version_high 	0x01 	//硬件版本的高位
#define Hardware_version_low 	0x00	//硬件版本的低位

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
const int DI3_ISTS = 100; 	//数字输入3
const int DI4_ISTS = 101; 	//数字输入4
const int DI5_ISTS = 100; 	//数字输入5
const int DI6_ISTS = 101; 	//数字输入6
const int DI7_ISTS = 100; 	//数字输入7
const int DI8_ISTS = 101; 	//数字输入8

const int AI1_IREG = 200; 	//模拟输入1
const int AI2_IREG = 201; 	//模拟输入2
const int AI3_IREG = 200; 	//模拟输入3
const int AI4_IREG = 201; 	//模拟输入4
const int AI5_IREG = 200; 	//模拟输入5
const int AI6_IREG = 201; 	//模拟输入6
const int AI7_IREG = 200; 	//模拟输入7
const int AI8_IREG = 201; 	//模拟输入8

const int Variable_1 = 300; //变量

String comdata = "";//串口接收的字符串

//函数声明
void Modbus_Config(void);
void Request_Access_Network(void);
void Project_Debug(void);
void Key_Reset_LoRa_Parameter(void);


unsigned char gSN_Code[9] = {0x00}; //设备出厂默认SN码全为0

ModbusSerial mb;

// the setup function runs once when you press reset or power the board
void setup()
{
	Modbus_Config();

	/*Serial Wire debug only (JTAG-DP disabled, SW-DP enabled)*/
	afio_cfg_debug_ports(AFIO_DEBUG_SW_ONLY);

	/*配置IWDG*/
	iwdg_init(IWDG_PRE_256, 1000); //6.5ms * 1000 = 6500ms.

	Serial.begin(9600); //USART1, 当使用USART下载程序：USART--->USART1
	LoRa_MHL9LF.BaudRate(9600);
	bkp_init();								  //备份寄存器初始化使能
	EEPROM_Operation.EEPROM_GPIO_Config();	//设置EEPROM读写引脚
	Some_Peripheral.Peripheral_GPIO_Config(); //设置继电器，数字输入，模拟输入等外设引脚
	iwdg_feed();

	LoRa_MHL9LF.LoRa_GPIO_Config();
	LoRa_MHL9LF.Mode(PASS_THROUGH_MODE);
	/*
	 *上电后LoRa模块会发送厂家信息过来
	 *这个时候配置的第一个参数在校验回车换行等参数
	 *的时候会受到影响。必须先接收厂家信息并清空缓存
	 */
	delay(2000);
	gIsHandleMsgFlag = false;
	LoRa_Command_Analysis.Receive_LoRa_Cmd(); //从网关接收LoRa数据
	gIsHandleMsgFlag = true;
	iwdg_feed();

	//Initialize LoRa parameter.
	LoRa_MHL9LF.Parameter_Init(false);
	LoRa_Para_Config.Save_LoRa_Config_Flag();

#if SOFT_HARD_VERSION
	Serial.println("");

	//软件版本存储程序
	if (Software_version_high == Vertion.Read_Software_version(SOFT_VERSION_BASE_ADDR) &&
		Software_version_low == Vertion.Read_Software_version(SOFT_VERSION_BASE_ADDR + 1))
	{
		Serial.println(String("Software_version is V") + String(Software_version_high, HEX) + "." + String(Software_version_low, HEX));
	}
	else
	{
		Vertion.Save_Software_version(Software_version_high, Software_version_low);
		Serial.println(String("Successfully store the software version, the current software version is V") + String(Software_version_high, HEX) + "." + String(Software_version_low, HEX));
	}
	//硬件版本存储程序
	if (Hardware_version_high == Vertion.Read_hardware_version(HARD_VERSION_BASE_ADDR) &&
		Hardware_version_low == Vertion.Read_hardware_version(HARD_VERSION_BASE_ADDR + 1))
	{
		Serial.println(String("Hardware_version is V") + Hardware_version_high + "." + Hardware_version_low);
	}
	else
	{
		Vertion.Save_hardware_version(Hardware_version_high, Hardware_version_low);
		Serial.println(String("Successfully store the hardware version, the current hardware version is V") + Hardware_version_high + "." + Hardware_version_low);
	}
#endif

	Project_Debug(); //工程模式

	if (Reset_Binding)
	{
		/* code */
	}
	
	SN.Clear_SN_Access_Network_Flag(); //按键清除注册到服务器标志位

	/*Request access network(request gateway to save the device's SN code and channel)*/
	Request_Access_Network(); //检查是否注册到服务器

	while (SN.Self_check(gSN_Code) == false)
	{
		// LED_SELF_CHECK_ERROR;
		Serial.println("Verify SN code failed, try to Retrieving SN code...");
		Serial.println("验证SN代码失败，尝试找回SN代码…");
		Message_Receipt.Request_Device_SN_and_Channel(); //当本地SN码丢失，向服务器申请本机的SN码
		LoRa_Command_Analysis.Receive_LoRa_Cmd();		 //接收LORA参数
		MyDelayMs(3000);
		iwdg_feed();
	}
	Serial.println("SN self_check success...SN自检成功");
	// LED_RUNNING;
	iwdg_feed();

	Self_Check_Parameter_Timer_Init(); //使用定时器3初始化自检参数功能自检周期
	Serial.println("定时自检机制初始化完成");
}

/*
 @brief     : 
 @param     : 无
 @return    : 无
 */
void loop()
{
	iwdg_feed();

	// LoRa_Command_Analysis.Receive_LoRa_Cmd();//从网关接收LoRa数据
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

	// Check_Store_Param_And_LoRa(); //检查存储参数以及LORA参数
	// Serial.println(analogRead(AI1));
	// delay(1000);
}


/*
 @brief   : 进行modbus库的配置
 @para    : None
 @return  : None
 */
void Modbus_Config(void)
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
 @brief   : 检测是否已经注册到服务器成功，如果没有注册，则配置相关参数为默认参数，然后注册到服务器。
			没有注册成功，红灯1每隔500ms闪烁。
			Checks whether registration with the server was successful, and if not,
			configures the relevant parameters as default parameters and registers with the server.
			Failing registration, red light flashes every 500ms.
 @para    : None
 @return  : None
 */
void Request_Access_Network(void)
{
	if (SN.Verify_SN_Access_Network_Flag() == false)
	{
		gAccessNetworkFlag = false;

		if (SN.Save_SN_Code(gSN_Code) && SN.Save_BKP_SN_Code(gSN_Code))
			Serial.println("Write Inital SN success...写入初始SN成功");

		if (SN.Clear_Area_Number() && SN.Clear_Group_Number())
		{
			Serial.println("Already Clear area number and grouop number...");
			Serial.println("已清除区域号和组号");
		}

		unsigned char Default_WorkGroup[5] = {0x01, 0x00, 0x00, 0x00, 0x00};
		if (SN.Save_Group_Number(Default_WorkGroup))
			Serial.println("Save Inital gourp number success...保存初始组ID成功");

		// LED_NO_REGISTER;
	}
	// while (SN.Verify_SN_Access_Network_Flag() == false)
	// {
	// 	iwdg_feed();
	// 	if (digitalRead(SW_FUN1) == LOW)
	// 	{
	// 		MyDelayMs(5000);
	// 		iwdg_feed();
	// 		if (digitalRead(SW_FUN1) == LOW)
	// 		{
	// 			Message_Receipt.Report_General_Parameter();

	// 			while (digitalRead(SW_FUN1) == LOW)
	// 				iwdg_feed();
	// 		}
	// 	}
	// 	LoRa_Command_Analysis.Receive_LoRa_Cmd();
	// }
	gAccessNetworkFlag = true;
}

/*
 @brief   : 工程模式。用于在单机工作的情况下。手动卷膜，可以测量脉冲数、电机电流、上下限位等。
			通过按键1，可以测试重置行程。
 @para    : 无
 @return  : 无
 */
void Project_Debug(void)
{
}

void Key_Reset_LoRa_Parameter(void)
{
	// if (digitalRead(SW_FUN1) == LOW)
	// {
	// 	MyDelayMs(100);
	// 	if (digitalRead(SW_FUN1) == LOW)
	// 	{
	// 		MyDelayMs(3000);
	// 		iwdg_feed();
	// 		if (digitalRead(SW_FUN2) == LOW)
	// 		{
	// 			MyDelayMs(100);
	// 			if (digitalRead(SW_FUN2) == LOW)
	// 			{
	// 				LoRa_Para_Config.Clear_LoRa_Config_Flag();
	// 				Serial.println("Clear LoRa configuration flag SUCCESS... <Key_Reset_LoRa_Parameter>");
	// 				Serial.println("清除LoRa配置标志成功...<Key_Reset_LoRa_Parameter>");
	// 				iwdg_feed();
	// 			}
	// 		}
	// 	}
	// }
}