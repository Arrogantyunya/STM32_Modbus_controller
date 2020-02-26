#include "Private_Timer.h"
// #include "fun_periph.h"
#include "Security.h"
#include "receipt.h"
#include "Memory.h"
#include <Arduino.h>

/*Timer timing time*/
#define TIMER_NUM             1000000L * 1 //1S
#define CHECK_TIMER_NUM       1000000L

volatile static unsigned int gSelfCheckNum;
volatile static unsigned int gStateReportNum;

/*
 @brief   : 使用定时器2初始化卷膜行程计时参数
 @param   : 无
 @return  : 无
 */
// void Roll_Timer_Init(void)
// {
// 	Timer2.setPeriod(TIMER_NUM); // in microseconds，1S
// 	Timer2.attachCompare1Interrupt(Timer2_Interrupt);
// 	Timer2.setCount(0);
// 	Timer2.pause();
// }

/*
 @brief   : 使用定时器2初始化自动上报状态周期
 @param   : 无
 @return  : 无
 */
void Realtime_Status_Reporting_Timer_Init(void)
{
	Timer2.setPeriod(TIMER_NUM); // in microseconds，1S
	Timer2.attachCompare1Interrupt(Timer2_Interrupt);
	Timer2.setCount(0);
}

/*
 @brief   : 使用定时器3初始化自检参数功能自检周期
 @param   : 无
 @return  : 无
 */
void Self_Check_Parameter_Timer_Init(void)
{
	Timer3.setPeriod(TIMER_NUM); // in microseconds，1S
	Timer3.attachCompare1Interrupt(Timer3_Interrupt);
	Timer3.setCount(0);
}

/*
 @brief   : 开始卷膜计时
 @param   : 无
 @return  : 无
 */
// void Start_Roll_Timing(void)
// {
// 	gRollingTime = 0;
// 	gRollingTimeVarFlag = false;
// 	Timer2.setCount(0);
// 	Timer2.resume();
// }

/*
 @brief   : 开始上报状态周期计时
 @param   : 无
 @return  : 无
 */
void Start_Status_Report_Timing(void)
{
	Timer2.resume();
	Timer2.setCount(0);
}

/*
 @brief   : 开始自检周期计时
 @param   : 无
 @return  : 无
 */
void Start_Self_Check_Timing(void)
{
	Timer3.resume();
	Timer3.setCount(0);
}

/*
 @brief   : 停止卷膜计时
 @param   : 无
 @return  : 无
 */
// void Stop_Roll_Timing(void)
// {
// 	Timer2.pause();
// }

/*
 @brief   : 停止状态上报周期计时
 @param   : 无
 @return  : 无
 */
void Stop_Status_Report_Timing(void)
{
	Timer2.pause();
}

/*
 @brief   : 停止自检周期计时
 @param   : 无
 @return  : 无
 */
void Stop_Self_Check_Timing(void)
{
	Timer3.pause();
}

/*
 @brief   : 卷膜计时定时器2计时中断处理函数
 @param   : 无
 @return  : 无
 */
// void Timer2_Interrupt(void)
// {
// 	gRollingTime++;
// 	gRollingTimeVarFlag = true;
// }


/*
 @brief   : 上报实时状态定时器2计时中断处理函数
 @param   : 无
 @return  : 无
 */
void Timer2_Interrupt(void)
{
	gStateReportNum++;
	/*大于0秒 && 小于1800秒（30分钟）才能开启自动上报*/
	if (InitState.Read_E014Auto_report() > 0 && InitState.Read_E014Auto_report() <= 1800)
	{
		if (gStateReportNum >= InitState.Read_E014Auto_report()) //
		{
			gStateReportNum = 0;
			gStateReportFlag = true;
		}
	}
}

/*
 @brief   : 自检计时定时器3计时中断处理函数
 @param   : 无
 @return  : 无
 */
void Timer3_Interrupt(void)
{
	gSelfCheckNum++;
	if (gSelfCheckNum >= 14400) //4 hours	14400
	{
		gSelfCheckNum = 0;
		gCheckStoreParamFlag = true;
	}
}