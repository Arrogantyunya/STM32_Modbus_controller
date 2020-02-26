#include "receipt.h"
#include "User_CRC8.h"
#include <libmaple/iwdg.h>
#include "LoRa.h"
// #include "Motor.h"
#include "Memory.h"
#include "Command_Analysis.h"
#include "fun_periph.h"
#include "public.h"
#include "Set_coil.h"

Receipt Message_Receipt;

// #define CLEAR_BUFFER_FLAG   0 //是否清除服务器数据

#define SEND_DATA_DELAY     200 //发送完一帧数据后延时时间（ms）

// unsigned char gMotorStatus = MotorFactoryMode;  //每次开机默认状态未初始化
//unsigned char x = 0x00;
unsigned char gStatus_Working_Parameter_Receipt = 0x00;//E014实时工作状态回执的状态
unsigned char gStatus_E014 = DefaultValue;	//E014通用回执的状态
unsigned char gLoRaCSQ[2] = { 0 };  //接收LoRa发送和接收的信号强度
extern unsigned char G_modbusPacket[20];
extern int G_modbusPacket_Length;

volatile bool gStateReportFlag = false;

// /*
//  @brief   : 清除服务器上一次接收的LoRa数据缓存
//  @param   : 无
//  @return  : 无
// */
// void Receipt::Clear_Server_LoRa_Buffer(void)
// {
// 	/*发送一帧帧尾，让服务器认为接收到了完成数据，清空缓存*/
// 	unsigned char Buffer[6] = { 0x0D, 0x0A, 0x0D, 0x0A, 0x0D, 0x0A };
// 	LoRa_Serial.write(Buffer, 6);
// 	delay(SEND_DATA_DELAY);
// }

/*
 @brief   : 随机生成回执消息的回执发送微秒延时
 @param   : 随机延时（ms）
 @return  : 无
 */
void Receipt::Receipt_Random_Wait_Value(unsigned long int *random_value)
{
	unsigned char RandomSeed;
	SN.Read_Random_Seed(&RandomSeed); //读取随机数种子
	/*RandomSeed * 1ms, 1.5S + RandomSeed * 0.1ms, 200ms*/
	*random_value = random(RandomSeed * 1000, 1500000) + random(RandomSeed * 100, 200000);
}

/*
 @brief   : 上报本设备通用设置参数,包括区域号、SN码、子设备路数、工作组号、采集间隔等（本机 ---> 服务器）
			该回执帧也用作设备第一次申请注册服务器。
 @param   : 无
 @return  : 无
 */
void Receipt::Report_General_Parameter(void)
{
	//  帧头     |    帧ID   |  数据长度   |    设备类型ID   |群发标志位 | 所在执行区域号  | 工作组号   | SN码    | 查询角色 | 采集时间间隔      |  时间   |  预留位     |  校验码  |     帧尾 
	//Frame head | Frame ID | Data Length | Device type ID | mass flag | Area number   | workgroup | SN code | channel | collect interval  |  RTC   |   allocate  |  CRC8   |  Frame end
	//  1 byte       2 byte      1 byte          2 byte       1 byte       1 byte          5 byte     9 byte    1 byte      2 byte           7 byte      8 byte     1 byte      6 byte

	unsigned char ReportFrame[64] = { 0 };
	unsigned char FrameLength = 0;
	unsigned char DataTemp[10];
	unsigned long int RandomSendInterval = 0;

	Receipt_Random_Wait_Value(&RandomSendInterval);
	delayMicroseconds(RandomSendInterval);
	iwdg_feed();

#if CLEAR_BUFFER_FLAG
	Clear_Server_LoRa_Buffer();
#endif

	ReportFrame[FrameLength++] = 0xFE; //帧头
	ReportFrame[FrameLength++] = 0xE0; //帧ID
	ReportFrame[FrameLength++] = 0x11;
	ReportFrame[FrameLength++] = 0x24; //帧有效数据长度
	/*设备类型*/
	ReportFrame[FrameLength++] = highByte(DEVICE_TYPE_ID);
	ReportFrame[FrameLength++] = lowByte(DEVICE_TYPE_ID);
	/*是否是群发*/
	gMassCommandFlag == true ? ReportFrame[FrameLength++] = 0x55 : ReportFrame[FrameLength++] = 0x00;
	/*区域号*/
	ReportFrame[FrameLength++] = SN.Read_Area_Number();
	/*组号*/
	SN.Read_Group_Number(&DataTemp[0]);
	for (unsigned char i = 0; i < 5; i++)
		ReportFrame[FrameLength++] = DataTemp[i];
	/*SN码*/
	SN.Read_SN_Code(&DataTemp[0]);
	for (unsigned char i = 0; i < 9; i++)
		ReportFrame[FrameLength++] = DataTemp[i];
	/*路数*/
	ReportFrame[FrameLength++] = 0x01; //卷膜机默认只有一路
	/*采集状态间隔*/
	ReportFrame[FrameLength++] = 0x00;
	ReportFrame[FrameLength++] = 0x00;
	/*RTC*/
	for (unsigned char i = 0; i < 7; i++)
		ReportFrame[FrameLength++] = 0x00;
	/*预留的8个字节*/
	for (unsigned char i = 0; i < 8; i++)
		ReportFrame[FrameLength++] = 0x00;
	/*CRC8*/
	ReportFrame[FrameLength++] = GetCrc8(&ReportFrame[4], 0x24);
	/*帧尾*/
	for (unsigned char i = 0; i < 6; i++)
		i % 2 == 0 ? ReportFrame[FrameLength++] = 0x0D : ReportFrame[FrameLength++] = 0x0A;

	Serial.println("Report general parameter...");
	/*打印要发送的数据帧*/
	Print_Debug(&ReportFrame[0], FrameLength);

	Some_Peripheral.Stop_LED();
	LoRa_Serial.write(&ReportFrame[0], FrameLength);
	delay(SEND_DATA_DELAY);
	Some_Peripheral.Start_LED();
}

/*
 @brief   : 当本地工作组号丢失，向服务器申请本机的工作组号（本设备 ---> 服务器）
 @param   : 无
 @return  : 无
 */
void Receipt::Request_Set_Group_Number(void)
{
	//  帧头     |    帧ID   |  数据长度   |    设备类型ID   |群发标志位 | 所在执行区域号 |  设备路数      | 校验码  |     帧尾 
	//Frame head | Frame ID | Data Length | Device type ID | mass flag | Area number  | Device channel |  CRC8  |  Frame end
	//  1 byte        2 byte      1 byte          2 byte       1 byte       1 byte          1 byte       1 byte     6 byte

	unsigned char RequestFrame[20] = { 0 };
	unsigned char FrameLength = 0;
	// unsigned char RandomSeed;
	unsigned long int RandomSendInterval = 0;

	Receipt_Random_Wait_Value(&RandomSendInterval);
	delayMicroseconds(RandomSendInterval);
	iwdg_feed();

#if CLEAR_BUFFER_FLAG
	Clear_Server_LoRa_Buffer();
#endif

	RequestFrame[FrameLength++] = 0xFE; //帧头
	RequestFrame[FrameLength++] = 0xE0; //帧ID
	RequestFrame[FrameLength++] = 0x12;
	RequestFrame[FrameLength++] = 0x05; //帧有效数据长度
	/*设备ID*/
	RequestFrame[FrameLength++] = highByte(DEVICE_TYPE_ID);
	RequestFrame[FrameLength++] = lowByte(DEVICE_TYPE_ID);
	/*是否是群发*/
	gMassCommandFlag == true ? RequestFrame[FrameLength++] = 0x55 : RequestFrame[FrameLength++] = 0x00;
	/*区域号*/
	RequestFrame[FrameLength++] = SN.Read_Area_Number();
	/*设备路数*/
	RequestFrame[FrameLength++] = 0x01;
	/*CRC8*/
	RequestFrame[FrameLength++] = GetCrc8(&RequestFrame[4], 0x05);
	/*帧尾*/
	for (unsigned char i = 0; i < 6; i++)
		i % 2 == 0 ? RequestFrame[FrameLength++] = 0x0D : RequestFrame[FrameLength++] = 0x0A;

	Serial.println("Requeset SN code to access network...");
	/*打印要发送的数据帧*/
	Print_Debug(&RequestFrame[0], FrameLength);

	Some_Peripheral.Stop_LED();
	LoRa_Serial.write(&RequestFrame[0], FrameLength);
	delay(SEND_DATA_DELAY);
	Some_Peripheral.Start_LED();
}

/*
 @brief   : 当本地SN码丢失，向服务器申请本机的SN码（本设备 ---> 服务器）
 @param   : 无
 @return  : 无
 */
void Receipt::Request_Device_SN_and_Channel(void)
{
	//  帧头     |    帧ID   |  数据长度   |    设备类型ID   | 群发标志位 | 所在执行区域号 |  设备路数      | 校验码  |     帧尾 
	//Frame head | Frame ID | Data Length | Device type ID |  mass flag |  Area number | Device channel |  CRC8  |  Frame end
	//  1 byte        2 byte      1 byte          2 byte        1 byte      1 byte          1 byte       1 byte     6 byte

	unsigned char RequestFrame[20] = { 0 };
	unsigned char FrameLength = 0;
	// unsigned char RandomSeed;
	unsigned long int RandomSendInterval = 0;

	Receipt_Random_Wait_Value(&RandomSendInterval);
	delayMicroseconds(RandomSendInterval);
	iwdg_feed();

#if CLEAR_BUFFER_FLAG
	Clear_Server_LoRa_Buffer();
#endif

	RequestFrame[FrameLength++] = 0xFE; //帧头
	RequestFrame[FrameLength++] = 0xE0; //帧ID
	RequestFrame[FrameLength++] = 0x13;
	RequestFrame[FrameLength++] = 0x05; //帧有效数据长度
	/*设备ID*/
	RequestFrame[FrameLength++] = highByte(DEVICE_TYPE_ID);
	RequestFrame[FrameLength++] = lowByte(DEVICE_TYPE_ID);
	/*是否是群发*/
	gMassCommandFlag == true ? RequestFrame[FrameLength++] = 0x55 : RequestFrame[FrameLength++] = 0x00;
	/*区域号*/
	RequestFrame[FrameLength++] = SN.Read_Area_Number();
	/*设备路数*/
	RequestFrame[FrameLength++] = 0x01;
	/*CRC8*/
	RequestFrame[FrameLength++] = GetCrc8(&RequestFrame[4], 0x05);
	/*帧尾*/
	for (unsigned char i = 0; i < 6; i++)
		i % 2 == 0 ? RequestFrame[FrameLength++] = 0x0D : RequestFrame[FrameLength++] = 0x0A;

	Serial.println("Requeset SN code to access network...");
	/*打印要发送的数据帧*/
	Print_Debug(&RequestFrame[0], FrameLength);

	Some_Peripheral.Stop_LED();
	LoRa_Serial.write(&RequestFrame[0], FrameLength);
	delay(SEND_DATA_DELAY);
	Some_Peripheral.Start_LED();
}

/*
 @brief   : 上报实时详细工作状态（本机 ---> 服务器）
 @param   : 上报次数
 @return  : 无
 */
void Receipt::Working_Parameter_Receipt(bool use_random_wait, unsigned char times, unsigned char randomId_1, unsigned char randomId_2)
{
  /*| 字节索引	| 0			| 1-2		| 3			| 4-5			| 6-7		| 8			| 9-10	| 11-12	| 13	| 14	| 15-22	| 23-30	| 31-46	| 47-62	| 63-64	| 65		| 66	| 67-73			|
	| 数据域		| frameHead	| frameId	| dataLen	| DeviceTypeId	| randomId	| status	| swVer	| hwVer	| rssi	| csq	| DI	| DO	| AI	| AO	| VOL	| LoraMode	| CRC8	| frameEnd      |
	| 长度		| 1			| 2			| 1			| 2				| 2			| 1			| 2		| 2		| 1		| 1		| 8		| 8		| 16	| 16	| 2		| 1			| 1		| 6				|
	| 示例数据	| FE		| E014		| 3E		| C003			| 1234		|			|       |       |		|		|		|		|		|		|		| F1		| D6	| 0D0A0D0A0D0A	|*/

	Serial.println("上报实时详细工作状态 <Working_Parameter_Receipt>");
	unsigned char ReceiptFrame[64] = { 0 };
	unsigned char ReceiptLength = 0;
	// unsigned char RandomSeed;
	unsigned long int RandomSendInterval = 0;

	iwdg_feed();
	//if (use_random_wait)
	//{
	//	//随机等待一段时间后再发送，避免大量设备发送堵塞。
	//	Receipt_Random_Wait_Value(&RandomSendInterval);
	//	delayMicroseconds(RandomSendInterval);
	//}
	iwdg_feed();

	if (gLoRaCSQ[0] == 0 || gLoRaCSQ[1] == 0)
	{
		Serial.println("开始查询信号质量");
		LoRa_MHL9LF.LoRa_AT(gLoRaCSQ, true, AT_CSQ_, 0);
	}

#if CLEAR_BUFFER_FLAG
	Clear_Server_LoRa_Buffer();
#endif

	ReceiptFrame[ReceiptLength++] = 0xFE; //帧头
	ReceiptFrame[ReceiptLength++] = 0xE0; //帧ID
	ReceiptFrame[ReceiptLength++] = 0x14; //
	ReceiptFrame[ReceiptLength++] = 0x3E; //数据长度55+7=63

	/*设备类型*/
	ReceiptFrame[ReceiptLength++] = highByte(DEVICE_TYPE_ID);
	ReceiptFrame[ReceiptLength++] = lowByte(DEVICE_TYPE_ID);

	/*randomId*/
	ReceiptFrame[ReceiptLength++] = randomId_1;
	ReceiptFrame[ReceiptLength++] = randomId_2;

	/*status*/
	Serial.println(String("gStatus_E014 = ") + gStatus_E014);
	ReceiptFrame[ReceiptLength++] = gStatus_E014;

	/*swVer*/
	ReceiptFrame[ReceiptLength++] = Vertion.Read_Software_version(SOFT_VERSION_BASE_ADDR);
	ReceiptFrame[ReceiptLength++] = Vertion.Read_Software_version(SOFT_VERSION_BASE_ADDR + 1);

	/*hwVer*/
	ReceiptFrame[ReceiptLength++] = Vertion.Read_hardware_version(HARD_VERSION_BASE_ADDR);
	ReceiptFrame[ReceiptLength++] = Vertion.Read_hardware_version(HARD_VERSION_BASE_ADDR + 1);

	/*验证服务器命令是否是群发*/
	//gMassCommandFlag == true ? ReceiptFrame[ReceiptLength++] = 0x55 : ReceiptFrame[ReceiptLength++] = 0x00;
	
	/*区域ID*/
	//ReceiptFrame[ReceiptLength++] = SN.Read_Area_Number();
	
	/*设备路数*/
	//ReceiptFrame[ReceiptLength++] = 0x01;
	
	/*SNR 和 RSSI*/
	ReceiptFrame[ReceiptLength++] = gLoRaCSQ[0];  //信号发送强度
	ReceiptFrame[ReceiptLength++] = gLoRaCSQ[1];  //信号接收强度
	//ReceiptFrame[ReceiptLength++] = Type_Conv.Dec_To_Hex(gLoRaCSQ[0]);  //信号发送强度
	//ReceiptFrame[ReceiptLength++] = Type_Conv.Dec_To_Hex(gLoRaCSQ[1]);  //信号接收强度

#if PLC_V1
	/*DI	8*/
	ReceiptFrame[ReceiptLength++] = Modbus_Coil.Get_DI_1to8();
	for (size_t i = 0; i < 7; i++)
	{
		ReceiptFrame[ReceiptLength++] = 0x00;
	}

	/*DO	8*/
	ReceiptFrame[ReceiptLength++] = Modbus_Coil.Get_DO_1to8();
	ReceiptFrame[ReceiptLength++] = Modbus_Coil.Get_DO_9to16();
	for (size_t i = 0; i < 6; i++)
	{
		ReceiptFrame[ReceiptLength++] = 0x00;
	}

	/*AI	16*/
	unsigned char* p = Modbus_Coil.Get_AI_1to8();
	for (size_t i = 0; i < 16; i++)
	{
		ReceiptFrame[ReceiptLength++] = *(p + i);
	}
	/*AO	16*/
	for (size_t i = 0; i < 16; i++)
	{
		ReceiptFrame[ReceiptLength++] = 0x00;
	}

	/*VOL*/
	ReceiptFrame[ReceiptLength++] = 0x09;
	ReceiptFrame[ReceiptLength++] = 0x60;//0x0960=2400mv

	/*LoraMode*/
	ReceiptFrame[ReceiptLength++] = 0xF1;

	/*CRC8*/
	ReceiptFrame[ReceiptLength++] = GetCrc8(&ReceiptFrame[4], 0x37);
	/*帧尾*/
	for (unsigned char i = 0; i < 6; i++)
		i % 2 == 0 ? ReceiptFrame[ReceiptLength++] = 0x0D : ReceiptFrame[ReceiptLength++] = 0x0A;

	Serial.println("LoRa parameter receipt...");
	Print_Debug(&ReceiptFrame[0], ReceiptLength);

	for (unsigned char i = 0; i < times; i++)
	{
		iwdg_feed();
		Some_Peripheral.Stop_LED();
		LoRa_Serial.write(&ReceiptFrame[0], ReceiptLength);
		delayMicroseconds(SEND_DATA_DELAY * 1000);
		//Some_Peripheral.Start_LED();
	}

	unsigned long T = millis(); unsigned long T1 = millis();//T是改变的，T1是用来计超时的
	unsigned long E014Interval = InitState.Read_E014Interval() * 1000;
	unsigned long Timeout = InitState.Read_Timeout() * 1000;
	Serial.println(String("E014Interval = ") + E014Interval / 1000 + " <Control_command_Receipt>");
	Serial.println(String("Timeout = ") + Timeout / 1000 + "S <Control_command_Receipt>");

	while (Get_receipt == false)
	{
		LoRa_Command_Analysis.Receive_LoRa_Cmd();//从网关接收LoRa数据
		iwdg_feed();
		if (millis() - T >= E014Interval)
		{
			T = millis();
			Serial.println("LoRa parameter receipt... <Working_Parameter_Receipt>");
			LoRa_Serial.write(ReceiptFrame, ReceiptLength);
		}
		if (millis() - T1 >= Timeout)
		{
			//达到超时时间，复位单片机
			nvic_sys_reset();
		}
	}

#elif PLC_V2

#endif
}

/*
 @brief   : 发送通用回执信息给服务器。（本设备 ---> 服务器）
			在大多数情况下，当接受到服务器的指令后，需要发送本条通用回执
 @param   : 1.回执状态
			2.回执次数
 @return  : 无
 */
void Receipt::General_Receipt(unsigned char status, unsigned char send_times)
{
	//  帧头     |    帧ID   |  数据长度   |    设备类型ID   | 群发标志位 | 所在执行区域号 |  设备路数      | 回执状态       |  预留位    | 校验码  |     帧尾 
	//Frame head | Frame ID | Data Length | Device type ID | mass flag | Area number | Device channel | receipt status |  allocate | CRC8    |  Frame end
	//  1 byte        2 byte      1 byte          2 byte       1 byte        1 byte          1 byte       1 byte          8 byte      1 byte     6 byte

	unsigned char ReceiptFrame[25] = { 0 };
	unsigned char ReceiptLength = 0;
	// unsigned char RandomSeed;
	unsigned long int RandomSendInterval = 0;

	Receipt_Random_Wait_Value(&RandomSendInterval);
	delayMicroseconds(RandomSendInterval);
	iwdg_feed();

	/*读取LoRa模块的 SNR and RSSI，为了防止影响性能，只获取一次信号值*/
	if (gLoRaCSQ[0] == 0 || gLoRaCSQ[1] == 0)
		LoRa_MHL9LF.LoRa_AT(gLoRaCSQ, true, AT_CSQ_, 0);

#if CLEAR_BUFFER_FLAG
	Clear_Server_LoRa_Buffer();
#endif

	ReceiptFrame[ReceiptLength++] = 0xFE; //帧头
	ReceiptFrame[ReceiptLength++] = 0xE0; //帧ID
	ReceiptFrame[ReceiptLength++] = 0x15;
	ReceiptFrame[ReceiptLength++] = 0x0E; //帧有效数据长度
	/*设备类型*/
	ReceiptFrame[ReceiptLength++] = highByte(DEVICE_TYPE_ID);
	ReceiptFrame[ReceiptLength++] = lowByte(DEVICE_TYPE_ID);
	/*是否是群发*/
	gMassCommandFlag == true ? ReceiptFrame[ReceiptLength++] = 0x55 : ReceiptFrame[ReceiptLength++] = 0x00;
	/*区域号*/
	ReceiptFrame[ReceiptLength++] = SN.Read_Area_Number();
	/*路数*/
	ReceiptFrame[ReceiptLength++] = 0x01;
	/*回执状态*/
	ReceiptFrame[ReceiptLength++] = status;
	/*预留的8个字节*/
	/*SNR and RSSI*/
	ReceiptFrame[ReceiptLength++] = Type_Conv.Dec_To_Hex(gLoRaCSQ[0]);  //发送信号强度
	ReceiptFrame[ReceiptLength++] = Type_Conv.Dec_To_Hex(gLoRaCSQ[1]);  //接收信号强度
	for (unsigned char i = 0; i < 6; i++)
		ReceiptFrame[ReceiptLength++] = 0x00;
	/*CRC8*/
	ReceiptFrame[ReceiptLength++] = GetCrc8(&ReceiptFrame[4], 0x0E);
	/*帧尾*/
	for (unsigned char i = 0; i < 6; i++)
		i % 2 == 0 ? ReceiptFrame[ReceiptLength++] = 0x0D : ReceiptFrame[ReceiptLength++] = 0x0A;

	Serial.println("Send General Receipt...");
	Print_Debug(&ReceiptFrame[0], ReceiptLength);

	//Some_Peripheral.Stop_LED();
	for (unsigned char i = 0; i < send_times; i++)
	{
		iwdg_feed();
		LoRa_Serial.write(ReceiptFrame, ReceiptLength);
		delay(SEND_DATA_DELAY);
	}
	//Some_Peripheral.Start_LED();
}

/*
 @brief   : 发送通用控制器Modbus控制指令回执信息给服务器。（本设备 ---> 服务器）
 @param   : 1.上报时间间隔，interval_1
			2.上报时间间隔，interval_2
			3.回执次数，send_times
			4.随机ID第1位，randomId_1
			5.随机ID第2位，randomId_2
			6.485设备回执数组的指针，R_Modbus_Instructions
			7.485设备回执数组的长度，R_Modbus_Length
 @return  : 无
 */
void Receipt::Control_command_Receipt(unsigned char interval_1, unsigned char interval_2, unsigned char send_times, unsigned char randomId_1, unsigned char randomId_2/*, unsigned char * R_Modbus_Instructions, int R_Modbus_Length*/)
{
	  /*| 字节索引	| 0			| 1-2		| 3			| 4 - 5			| 6				| 7			| 8			| 9			| 10		| 11~(11 + n)	|		|               |
		| 数据域		| frameHead	| frameId	| dataLen	| DeviceTypeId	| isBroadcast	| zoneId	| groupId	| random	| interval	| modbusPacket	| CRC8	| frameEnd		|
		| 长度		| 1			| 2			| 1			| 2				| 1				| 1			| 1			| 2			| 2			| n				| 1		| 6				|
		| 示例数据	| FE		| E000		| 9 + N		| C003			| 00			| 01		| 01		| 1234		| 0000		| XXXXXXXX		| 00	| 0D0A0D0A0D0A	|*/

	unsigned char ReceiptFrame[25] = { 0 };
	unsigned char ReceiptLength = 0;
	// unsigned char RandomSeed;
	unsigned long int RandomSendInterval = 0;

	//Receipt_Random_Wait_Value(&RandomSendInterval);
	//delayMicroseconds(RandomSendInterval);
	iwdg_feed();

	ReceiptFrame[ReceiptLength++] = 0xFE; //帧头
	ReceiptFrame[ReceiptLength++] = 0xE0; //帧ID
	ReceiptFrame[ReceiptLength++] = 0x00;
	ReceiptFrame[ReceiptLength++] = G_modbusPacket_Length + 9; //帧有效数据长度

	/*设备类型*/
	ReceiptFrame[ReceiptLength++] = highByte(DEVICE_TYPE_ID);
	ReceiptFrame[ReceiptLength++] = lowByte(DEVICE_TYPE_ID);
	/*是否是群发*/
	//gMassCommandFlag == true ? ReceiptFrame[ReceiptLength++] = 0x55 : ReceiptFrame[ReceiptLength++] = 0x00;
	ReceiptFrame[ReceiptLength++] = 0x00;
	/*区域号*/
	ReceiptFrame[ReceiptLength++] = SN.Read_Area_Number();
	/*组ID*/
	ReceiptFrame[ReceiptLength++] = 0x01;
	/*random*/
	ReceiptFrame[ReceiptLength++] = randomId_1;
	ReceiptFrame[ReceiptLength++] = randomId_2;
	/*interval*/
	ReceiptFrame[ReceiptLength++] = interval_1;
	ReceiptFrame[ReceiptLength++] = interval_2;
	/*设备回执的字段*/
	//extern unsigned char G_modbusPacket[10];
	//extern int G_modbusPacket_Length;
	for (unsigned char i = 0; i < G_modbusPacket_Length; i++)
	{
		ReceiptFrame[ReceiptLength++] = G_modbusPacket[i];
	}
	/*CRC8*/
	ReceiptFrame[ReceiptLength++] = GetCrc8(&ReceiptFrame[4], G_modbusPacket_Length + 9);
	/*帧尾*/
	for (unsigned char i = 0; i < 6; i++)
		i % 2 == 0 ? ReceiptFrame[ReceiptLength++] = 0x0D : ReceiptFrame[ReceiptLength++] = 0x0A;

	G_modbusPacket_Length = 0;//每次用完都得清零

	Serial.println("Send Control command Receipt...<Control_command_Receipt>");
	Print_Debug(&ReceiptFrame[0], ReceiptLength);

	for (unsigned char i = 0; i < send_times; i++)
	{
		iwdg_feed();
		LoRa_Serial.write(ReceiptFrame, ReceiptLength);
		delay(SEND_DATA_DELAY);
	}

	unsigned long T = millis(); unsigned long T1 = millis();//T是改变的，T1是用来计超时的
	unsigned long E000Interval = InitState.Read_E000Interval() * 1000;
	unsigned long Timeout = InitState.Read_Timeout() * 1000;
	Serial.println(String("E000Interval = ") + E000Interval/1000 + " <Control_command_Receipt>");
	Serial.println(String("Timeout = ") + Timeout/1000 + "S <Control_command_Receipt>");

	while (Get_receipt == false)
	{
		LoRa_Command_Analysis.Receive_LoRa_Cmd();//从网关接收LoRa数据
		iwdg_feed();
		if (millis() - T >= E000Interval)
		{
			T = millis();
			Serial.println("Send Control command Receipt...<Control_command_Receipt>");
			LoRa_Serial.write(ReceiptFrame, ReceiptLength);
		}
		if (millis() - T1 >= Timeout)
		{
			//达到超时时间，复位单片机
			nvic_sys_reset();
		}
	}
}

/*
 @brief   : 发送设置初始状态回执信息给服务器。（本设备 ---> 服务器）
 @param   : 1.回执状态，status
			2.回执次数，send_times
			3.随机ID第1位，randomId_1
			4.随机ID第2位，randomId_2
			5.485设备回执数组的指针，R_Modbus_Instructions
			6.485设备回执数组的长度，R_Modbus_Length
 @return  : 无
 */
void Receipt::Output_init_Receipt(unsigned char status, unsigned char send_times, unsigned char randomId_1, unsigned char randomId_2, unsigned char* R_Modbus_Instructions, int R_Modbus_Length)
{
  /*|字节索引	| 0			| 1 - 2		| 3			| 4 - 5		| 6			| 7-n	|		|				|
	|数据域		| frameHead | frameId	| dataLen	| randomId	| status	| RS485	| CRC8	| frameEnd		|
	|长度(byte)	| 1			| 2			| 1			| 2			| 1			| n		| 1		| 2				|
	|示例数据	| FE		| E002		|			| 1234		|			| xxxx	| 00	| 0D0A0D0A0D0A	|*/
	unsigned char ReceiptFrame[25] = { 0 };
	unsigned char ReceiptLength = 0;
	// unsigned char RandomSeed;
	unsigned long int RandomSendInterval = 0;

	//Receipt_Random_Wait_Value(&RandomSendInterval);
	//delayMicroseconds(RandomSendInterval);
	iwdg_feed();

	ReceiptFrame[ReceiptLength++] = 0xFE; //帧头
	ReceiptFrame[ReceiptLength++] = 0xE0; //帧ID
	ReceiptFrame[ReceiptLength++] = 0x02;
	ReceiptFrame[ReceiptLength++] = R_Modbus_Length + 3; //帧有效数据长度

	///*设备类型*/
	//ReceiptFrame[ReceiptLength++] = highByte(DEVICE_TYPE_ID);
	//ReceiptFrame[ReceiptLength++] = lowByte(DEVICE_TYPE_ID);
	///*是否是群发*/
	//gMassCommandFlag == true ? ReceiptFrame[ReceiptLength++] = 0x55 : ReceiptFrame[ReceiptLength++] = 0x00;
	///*区域号*/
	//ReceiptFrame[ReceiptLength++] = SN.Read_Area_Number();
	///*路数*/
	//ReceiptFrame[ReceiptLength++] = 0x01;
	/*randomId*/
	ReceiptFrame[ReceiptLength++] = randomId_1;
	ReceiptFrame[ReceiptLength++] = randomId_2;
	/*回执状态*/
	ReceiptFrame[ReceiptLength++] = status;
	/*485设备回执的字段*/
	for (unsigned char i = 0; i < R_Modbus_Length; i++)
	{
		ReceiptFrame[ReceiptLength++] = R_Modbus_Instructions[i];
	}
	/*CRC8*/
	ReceiptFrame[ReceiptLength++] = GetCrc8(&ReceiptFrame[4], R_Modbus_Length + 3);
	/*帧尾*/
	for (unsigned char i = 0; i < 6; i++)
		i % 2 == 0 ? ReceiptFrame[ReceiptLength++] = 0x0D : ReceiptFrame[ReceiptLength++] = 0x0A;

	Serial.println("Send Output init Receipt...<Output_init_Receipt>");
	Print_Debug(&ReceiptFrame[0], ReceiptLength);

	for (unsigned char i = 0; i < send_times; i++)
	{
		iwdg_feed();
		LoRa_Serial.write(ReceiptFrame, ReceiptLength);
		delay(SEND_DATA_DELAY);
	}
}

/*
 @brief   : 串口打印16进制回执信息
 @param   : 1.数据起始地址
			2.数据长度s
 @return  : 无
 */
void Receipt::Print_Debug(unsigned char *base_addr, unsigned char len)
{
	for (unsigned char i = 0; i < len; i++)
	{
		Serial.print(base_addr[i], HEX);
		Serial.print(" ");
	}
	Serial.println();
}
