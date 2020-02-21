/************************************************************************************
 *
 * 代码与注释：卢科青
 * 日期：2019/3/17
 *
 * 配置一些外设功能。如蜂鸣器、LED灯、按钮等。这些外设用来作为程序调试和人机交互。
 * 头文件中提供了各个类的公共接口。
 *
 * 如有任何疑问，请发送邮件到： idlukeqing@163.com
*************************************************************************************/

#include "fun_periph.h"
#include <libmaple/iwdg.h>
#include "pwm.h"
#include "Memory.h"
#include "Set_coil.h"

Some_Peripherals Some_Peripheral;

// LED Which_LED;
// unsigned int LED_Freq = 0;  //LED_Freq * 100ms


/*
 @brief   : 设置继电器，数字输入，模拟输入等外设引脚的模式
 @param   : 无
 @return  : 无
 */
void Some_Peripherals::Peripheral_GPIO_Pinmode(void)
{
	pinMode(KCZJ1, OUTPUT);
	pinMode(KCZJ2, OUTPUT);
	pinMode(KCZJ3, OUTPUT);
	pinMode(KCZJ4, OUTPUT);
	pinMode(KCZJ5, OUTPUT);
	pinMode(KCZJ6, OUTPUT);
	pinMode(KCZJ7, OUTPUT);
	pinMode(KCZJ8, OUTPUT);
	pinMode(KCZJ9, OUTPUT);
	pinMode(KCZJ10, OUTPUT);
	pinMode(KCZJ11, OUTPUT);
	pinMode(KCZJ12, OUTPUT);
	digitalWrite(KCZJ1, HIGH);
	digitalWrite(KCZJ2, HIGH);
	digitalWrite(KCZJ3, HIGH);
	digitalWrite(KCZJ4, HIGH);
	digitalWrite(KCZJ5, HIGH);
	digitalWrite(KCZJ6, HIGH);
	digitalWrite(KCZJ7, HIGH);
	digitalWrite(KCZJ8, HIGH);
	digitalWrite(KCZJ9, HIGH);
	digitalWrite(KCZJ10, HIGH);
	digitalWrite(KCZJ11, HIGH);
	digitalWrite(KCZJ12, HIGH);

	pinMode(DI1, INPUT);
	pinMode(DI2, INPUT);
	pinMode(DI2, INPUT);
	pinMode(DI3, INPUT);
	pinMode(DI4, INPUT);
	pinMode(DI5, INPUT);
	pinMode(DI5, INPUT);
	pinMode(DI6, INPUT);

	pinMode(AI1, INPUT);
	pinMode(AI2, INPUT);
	pinMode(AI3, INPUT);
	pinMode(AI4, INPUT);
	pinMode(AI5, INPUT);
	pinMode(AI6, INPUT);
	pinMode(AI7, INPUT);
	pinMode(AI8, INPUT);
}


/*
 @brief   : 设置继电器，数字输入，模拟输入等外设引脚的状态
 @param   : 无
 @return  : 无
 */
void Some_Peripherals::Peripheral_GPIO_Config(void)
{
	/*this-> Peripheral_GPIO_Pinmode();*/
	
	if (!InitState.Read_InitState_Flag())//读取初始状态的标志位
	{
		Serial.println("No initialization configuration saved <Peripheral_GPIO_Config>");//未保存初始化配置
		iwdg_feed();

		InitState.Clean_Timeout();//清除Clean_Timeout时间
		
		while (!Modbus_Coil.Set_Coil_DefaultValue())//设置线圈为默认值
		{
			Serial.println("ERROR!!! Set_Coil_DefaultValue Failure!!! <Peripheral_GPIO_Config>");//设置线圈初始值失败
			delay(1000);
			//这里不能喂狗，打印一定次数的错误报警信息后，看门狗自动复位单片机。
		}
	}
	else
	{
		Serial.println("Setting of peripheral pin initialization <Peripheral_GPIO_Config>");//外围引脚初始化设置
		while (!Modbus_Coil.Set_Coil_InitValue())//设置线圈为初始值
		{
			Serial.println("ERROR!!! Set_Coil_InitValue Failure!!! <Peripheral_GPIO_Config>");//设置线圈初始值失败
			delay(1000);
			//这里不能喂狗，打印一定次数的错误报警信息后，看门狗自动复位单片机。
		}
	}
}

/*
 @brief   : 选择要显示的LED灯（颜色），和闪烁的频率
 @param   : 1.哪一个LED
			2.频率值（100ms/次）
 @return  : 无
 */
// void Some_Peripherals::LED_Display(LED which_led, unsigned char freq)
// {
// 	Which_LED = which_led;
// 	LED_Freq = freq;
// 	Timer4.setPeriod(100000); // in microseconds, 100ms
// 	Timer4.attachCompare1Interrupt(LED_Interrupt);
// 	Timer4.setCount(0);
// }

/*
 @brief   : 暂停所有LED闪烁，同时暂停定时器4
 @param   : 无
 @return  : 无
*/
void Some_Peripherals::Stop_LED(void)
{
	Timer4.pause();
	// GREEN1_OFF;
	// GREEN2_OFF;
	// RED1_OFF;
	// RED2_OFF;
}

/*
 @brief   : 恢复定时器4计数，LED开始闪烁
 @param   : 无
 @return  : 无
 */
void Some_Peripherals::Start_LED(void)
{
	Timer4.resume();
}

/*
 @brief   : 外设引脚初始化。
 @param   : void
 @return  : 初始化完成返回true，失败返回false
 */
// bool Some_Peripherals::Peripheral_GPIO_Config_Init(void)
// {
	
// }



/*
 @brief   : 蜂鸣器按照固定频率发出响声，表明按键被按下。
 @param   : d ---> cycle time, 0.7ms one time.
 @return  : 无
 */
// void Some_Peripherals::Key_Buzz(unsigned int d) //700ms  1000 times.
// {
// 	for (unsigned int i = 0; i < d; i++)
// 	{
// 		BUZZ_ON;
// 		delayMicroseconds(500);
// 		BUZZ_OFF;
// 		delayMicroseconds(200);
// 	}
// }

/*
 @brief   : LED闪烁中断函数
 @param   : 无
 @return  : 无
 */
void LED_Interrupt(void)
{
	// static unsigned char LED_StatusFlag = 0; //LED灯状态标志位
	// static unsigned int LED_Num = 0;
	// LED_Num++;
	// if (LED_Num >= LED_Freq)
	// {
	// 	LED_Num = 0;
	// 	LED_StatusFlag = ~LED_StatusFlag;
	// 	switch (Which_LED)
	// 	{
	// 	case GREEN1: LED_StatusFlag == 0 ? digitalWrite(LED1_PIN, HIGH) : digitalWrite(LED1_PIN, LOW);
	// 		digitalWrite(LED2_PIN, LOW); digitalWrite(LED3_PIN, LOW); digitalWrite(LED4_PIN, LOW); break;

	// 	case RED1: LED_StatusFlag == 0 ? digitalWrite(LED2_PIN, HIGH) : digitalWrite(LED2_PIN, LOW);
	// 		digitalWrite(LED1_PIN, LOW); digitalWrite(LED3_PIN, LOW); digitalWrite(LED4_PIN, LOW); break;

	// 	case GREEN2: LED_StatusFlag == 0 ? digitalWrite(LED3_PIN, HIGH) : digitalWrite(LED3_PIN, LOW);
	// 		digitalWrite(LED1_PIN, LOW); digitalWrite(LED2_PIN, LOW); digitalWrite(LED4_PIN, LOW); break;

	// 	case RED2: LED_StatusFlag == 0 ? digitalWrite(LED4_PIN, HIGH) : digitalWrite(LED4_PIN, LOW);
	// 		digitalWrite(LED1_PIN, LOW); digitalWrite(LED2_PIN, LOW); digitalWrite(LED3_PIN, LOW); break;
	// 	}
	// }
}

