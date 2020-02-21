#ifndef _RECEIPT_H
#define _RECEIPT_H

#include <Arduino.h>

#define DEVICE_TYPE_ID  0xC003

#define SOFT_VERSION    0x14 //V2.0

#define HARD_VERSION    0x53//V8.3

enum E014_ReceiptStatus
{
	DefaultValue = 0, Failed_save_E000interval, Modbuspacket_length_overflow, A000_Received_Success
}/*E014_ReceiptState*/;

enum ReceiptStatus {
	FactoryMode = 0, AskUploadParamsOk, AskUploadParamsErr, AssignGroupIdArrayOk, AssignGroupIdArrayErr, SetSnAndSlaverCountOk,
	SetSnAndSlaverCountErr, TrunOffOk, TrunOffErr, RestRollerOk, ResetRollerErr, OpenRollerOk, OpenRollerErr, LimitRollerOk,
	LimitRollerErr, SetLoRaModeOk, SetLoRaModeErr
};

// /*电机状态*/
// enum MotorStatus {
// 	MotorFactoryMode = 0, ROLL_OK, HIGH_POSITION_LIMIT_EXCEPTION, LOW_POSITION_LIMIT_EXCEPTION, LOW_POWER, MOTOR_EXCEPTION, MOTOR_CURRENT_EXCEPTION,
// 	ROLLING, CMD_EXCEPTION, NOT_INITIALIZED, STORE_EXCEPTION, RESET_ROLLING, RESET_ROLLOK, RS485_EXCEPTION, FORCE_STOP, MANUAL_ROLL_OK, MANUAL_KEY_EXCEPTION,
// 	OPEN_ROLL_ERROR
// };

// extern unsigned char gMotorStatus;

// /*
//  @brief   : 设置当前设备工作状态
//  @para    : 设备状态
//  @return  : 无
// */
// inline void Set_Motor_Status(unsigned char status) { gMotorStatus = status; }

// /*
//  @brief   : 读取当前设备工作状态
//  @para    : None
//  @return  : 设备状态
// */
// inline unsigned char Read_Motor_Status(void) { return (gMotorStatus); }

class Receipt {
public:
	void Report_General_Parameter(void);
	void Request_Set_Group_Number(void);
	void Request_Device_SN_and_Channel(void);
	//void Working_Parameter_Receipt(bool use_random_wait, unsigned char times);
	void Working_Parameter_Receipt(bool use_random_wait, unsigned char times, unsigned char randomId_1, unsigned char randomId_2);
	void General_Receipt(unsigned char status, unsigned char send_times);
	void Control_command_Receipt(unsigned char interval_1, unsigned char interval_2, unsigned char send_times, unsigned char randomId_1, unsigned char randomId_2/*, unsigned char* R_Modbus_Instructions, int R_Modbus_Length*/);
	void Output_init_Receipt(unsigned char status, unsigned char send_times, unsigned char randomId_1, unsigned char randomId_2, unsigned char* R_Modbus_Instructions, int R_Modbus_Length);
private:
	void Receipt_Random_Wait_Value(unsigned long int *random_value);
	// void Clear_Server_LoRa_Buffer(void);
	void Print_Debug(unsigned char *base_addr, unsigned char len);
};

extern Receipt Message_Receipt;
extern unsigned char gStatus_E014;
extern volatile bool gStateReportFlag;
extern unsigned char gLoRaCSQ[2];  //接收LoRa发送和接收的信号强度

#endif
