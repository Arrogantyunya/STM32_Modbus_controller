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
#define DEBUG_PRINT 0 //打印调试信息
#define Reset_RTC 0   //重置rtc时间
/* 使能宏 */
#define SOFT_HARD_VERSION 1 //使能写入软件和硬件版本
/* 替换宏 */
#define SoftWare_Version_HIGH 0x01 //软件版本的高位
#define SoftWare_Version_LOW 0x00  //软件版本的低位
#define HardWare_Version_HIGH 0x01 //硬件版本的高位
#define HardWare_Version_LOW 0x00  //硬件版本的低位

// Modbus Registers Offsets (0-9999)
const int KCZJ1_COIL = 0;
const int KCZJ2_COIL = 1;
const int KEY1_ISTS = 100;
const int KEY2_ISTS = 101;
const int SENSOR1_IREG = 200;
const int SENSOR2_IREG = 201;
const int SERVO_HREG = 300;

//函数声明
void Request_Access_Network(void);
void Project_Debug(void);
void Key_Reset_LoRa_Parameter(void);

unsigned char gSN_Code[9] = { 0x00 }; //设备出厂默认SN码全为0

void setup()
{
  // put your setup code here, to run once:

  /*Serial Wire debug only (JTAG-DP disabled, SW-DP enabled)*/
  afio_cfg_debug_ports(AFIO_DEBUG_SW_ONLY);

  /*配置IWDG*/
  iwdg_init(IWDG_PRE_256, 1000); //6.5ms * 1000 = 6500ms.

  Serial.begin(115200); //USART1, 当使用USART下载程序：USART--->USART1
  LoRa_MHL9LF.BaudRate(9600);
  bkp_init();                               //备份寄存器初始化使能
  EEPROM_Operation.EEPROM_GPIO_Config();    //设置EEPROM读写引脚
  Some_Peripheral.Peripheral_GPIO_Config(); //设置蜂鸣器，LED，功能按钮等引脚
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
  Vertion.Save_Software_version(SoftWare_Version_HIGH, SoftWare_Version_LOW);
  Vertion.Save_hardware_version(HardWare_Version_HIGH, HardWare_Version_LOW);
#endif

  Project_Debug();//工程模式

  SN.Clear_SN_Access_Network_Flag();//按键清除注册到服务器标志位
	
  /*Request access network(request gateway to save the device's SN code and channel)*/
	Request_Access_Network();//检查是否注册到服务器

	while (SN.Self_check(gSN_Code) == false)
	{
		LED_SELF_CHECK_ERROR;
		Serial.println("Verify SN code failed, try to Retrieving SN code...");
		Serial.println("验证SN代码失败，尝试找回SN代码…");
		Message_Receipt.Request_Device_SN_and_Channel();//当本地SN码丢失，向服务器申请本机的SN码
		LoRa_Command_Analysis.Receive_LoRa_Cmd();//接收LORA参数
		MyDelayMs(3000);
		iwdg_feed();
	}
	Serial.println("SN self_check success...SN自检成功");
	LED_RUNNING;
	iwdg_feed();

	Self_Check_Parameter_Timer_Init();//使用定时器3初始化自检参数功能自检周期
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
  
  Check_Store_Param_And_LoRa();//检查存储参数以及LORA参数
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

		unsigned char Default_WorkGroup[5] = { 0x01, 0x00, 0x00, 0x00, 0x00 };
		if (SN.Save_Group_Number(Default_WorkGroup))
			Serial.println("Save Inital gourp number success...保存初始组ID成功");

		LED_NO_REGISTER;
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
	if (digitalRead(SW_FUN1) == LOW)
	{
		MyDelayMs(100);
		if (digitalRead(SW_FUN1) == LOW)
		{
			MyDelayMs(3000);
			iwdg_feed();
			if (digitalRead(SW_FUN2) == LOW)
			{
				MyDelayMs(100);
				if (digitalRead(SW_FUN2) == LOW)
				{
					LoRa_Para_Config.Clear_LoRa_Config_Flag();
					Serial.println("Clear LoRa configuration flag SUCCESS... <Key_Reset_LoRa_Parameter>");
					Serial.println("清除LoRa配置标志成功...<Key_Reset_LoRa_Parameter>");
					iwdg_feed();
				}
			}
		}
	}
}