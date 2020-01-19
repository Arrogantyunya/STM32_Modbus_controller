//
/*
    Name:       STM32_Modbus_controllerV1.0.0_alpha.ino
    Created:	2020/01/30 星期五 21:07:51
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
#include <wirish_debug.h>

#include "Modbus.h"
#include "ModbusSerial.h"
#include "Set_coil.h"
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
#define Init_Area				0x01	//初始区域ID


//全局变量
String comdata = "";//串口接收的字符串

//函数声明
void Request_Access_Network(void);
void Project_Debug(void);
void Key_Reset_LoRa_Parameter(void);


unsigned char gSN_Code[9] = {0x00}; //设备出厂默认SN码全为0

// the setup function runs once when you press reset or power the board
void setup()
{
	Modbus_Coil.Modbus_Config();

	/*Serial Wire debug only (JTAG-DP disabled, SW-DP enabled)*/
	// C:\Program Files (x86)\Arduino\hardware\stm32\STM32F1\system\libmaple\stm32f1\include\series
	afio_cfg_debug_ports(AFIO_DEBUG_NONE);

	/*配置IWDG*/
	iwdg_init(IWDG_PRE_256, 1000); //6.5ms * 1000 = 6500ms.

	Serial.begin(9600); //USART1, 当使用USART下载程序：USART--->USART1
	LoRa_MHL9LF.BaudRate(9600);
	bkp_init();								//备份寄存器初始化使能
	Serial.println("dididiid");
	EEPROM_Operation.EEPROM_GPIO_Config();	//设置EEPROM读写引脚
	Some_Peripheral.Peripheral_GPIO_Config(); //设置继电器，数字输入，模拟输入等外设引脚的模式，以及初始化状态
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

	SN.Clear_SN_Access_Network_Flag(); //清除注册到服务器标志位

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
	
	Modbus_Coil.Modbus_Realization();//

	Check_Store_Param_And_LoRa(); //检查存储参数以及LORA参数
	// Serial.println("test <loop>");
	// delay(2000);
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
			Serial.println("Write Inital SN success... <Request_Access_Network>");

		if (SN.Clear_Area_Number() && SN.Clear_Group_Number())
		{
			Serial.println("Already Clear area number and group number... <Request_Access_Network>");
		}

		unsigned char Default_WorkGroup[5] = {0x01, 0x00, 0x00, 0x00, 0x00};
		if (SN.Save_Group_Number(Default_WorkGroup))
			Serial.println("Save Inital group number success... <Request_Access_Network>");
		if (SN.Save_Area_Number(Init_Area))
			Serial.println("Save Inital area number success... <Request_Access_Network>");

		Serial.println("Not registered to server, please send \"S\"	<Request_Access_Network>");
		// LED_NO_REGISTER;
	}
	// while (SN.Verify_SN_Access_Network_Flag() == false)
	// {
	// 	iwdg_feed();
	// 	while (Serial.available() > 0)
	// 	{
	// 		comdata += char(Serial.read());  //每次读一个char字符，并相加
	// 		delay(2);
    // 	}
		
	// 	if (comdata.length() > 0)
	// 	{
	// 		comdata.toUpperCase();
	// 		Serial.println(comdata);
	// 		if (comdata == String("S"))
	// 		{
	// 			comdata = "";
	// 			Serial.println("Start sending registration data to server <Request_Access_Network>");
	// 			Message_Receipt.Report_General_Parameter();
	// 		}
			
	// 		iwdg_feed();
	// 	}
	// 	comdata = "";
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
	// while (1)
	// {
	// 	iwdg_feed();
	// 	for (size_t i = 0; i < 12; i++)
	// 	{
			
	// 	}
		
	// }
	
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