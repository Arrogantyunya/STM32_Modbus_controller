#ifndef _COMMAND_ANALYSIS_H
#define _COMMAND_ANALYSIS_H

#include <Arduino.h>

enum Frame_ID {
	Modbus_Control, R_Modbus_Control,Output_default,Work_Para, Set_Group_Num, SN_Area_Channel, Work_Status
	/* , ResetRoll, Opening, Work_Limit*/, Stop_Work 
};

class Command_Analysis {
public:
	void Receive_LoRa_Cmd(void);

private:
	Frame_ID FrameID_Analysis(void);
	bool Verify_CRC8(unsigned char verify_data_base_addr, unsigned char verify_data_len);
	bool Verify_Device_Type_ID(void);
	void Verify_Mass_Commands(void);
	bool Verify_Area_Number(void);
	bool Verify_Work_Group(void);
	bool Verify_Frame_Validity(unsigned char verify_data_base_addr, unsigned char verify_data_len, bool area_flag, bool group_flag);

private:
	void Receive_Data_Analysis(void);
	void Query_Current_Work_Param(void);//服务器查询当前群控相关参数，如所在区域、SN码、路数、工作组、等
	void Set_Group_Number(void);//设置本设备的工作组号，不需要验证本设备原有工作组号
	void Set_SN_Area_Channel(void);//设置本设备的SN码、区域号、设备路数等参数
	void Detailed_Work_Status(void);//查询本设备详细工作状态
	// void ResetRoll_Command(void);//重置卷膜测量行程
	// void Opening_Command(void);//设置卷膜开度
	// void Working_Limit_Command(void);//电机工作电压阈值、上报状态间隔值设置
	void Stop_Work_Command(void);//强制停止当前设备的工作
	void General_controller_control_command(void);//服务器发送通用控制器Modbus控制指令
	void R_General_controller_control_command(void);//服务器发送通用控制器Modbus控制指令接收回执
	void Set_General_controller_output_init(void);//服务器设置通用控制器输出默认状态
};

/*Create command analysis project*/
extern Command_Analysis LoRa_Command_Analysis;

extern bool gAccessNetworkFlag;
// extern bool gStopWorkFlag;
extern bool gMassCommandFlag;
extern bool gIsHandleMsgFlag;

//
extern bool Get_receipt;

#endif
