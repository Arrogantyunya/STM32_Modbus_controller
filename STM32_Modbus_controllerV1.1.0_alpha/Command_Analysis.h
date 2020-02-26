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
	void Query_Current_Work_Param(void);//��������ѯ��ǰȺ����ز���������������SN�롢·���������顢��
	void Set_Group_Number(void);//���ñ��豸�Ĺ�����ţ�����Ҫ��֤���豸ԭ�й������
	void Set_SN_Area_Channel(void);//���ñ��豸��SN�롢����š��豸·���Ȳ���
	void Detailed_Work_Status(void);//��ѯ���豸��ϸ����״̬
	// void ResetRoll_Command(void);//���þ�Ĥ�����г�
	// void Opening_Command(void);//���þ�Ĥ����
	// void Working_Limit_Command(void);//���������ѹ��ֵ���ϱ�״̬���ֵ����
	void Stop_Work_Command(void);//ǿ��ֹͣ��ǰ�豸�Ĺ���
	void General_controller_control_command(void);//����������ͨ�ÿ�����Modbus����ָ��
	void R_General_controller_control_command(void);//����������ͨ�ÿ�����Modbus����ָ����ջ�ִ
	void Set_General_controller_output_init(void);//����������ͨ�ÿ��������Ĭ��״̬
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
