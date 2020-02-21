#ifndef _FUN_PERIPH_H
#define _FUN_PERIPH_H

#include <Arduino.h>

/*Some peripheral GPIO configuration*/
/* 12路继电器输出 */
#define KCZJ1				PB14	//继电器1
#define KCZJ2				PB15	//继电器2
#define KCZJ3				PC6		//继电器3
#define KCZJ4				PC7		//继电器4
#define KCZJ5				PC8		//继电器5
#define KCZJ6				PC9		//继电器6
#define KCZJ7				PA8		//继电器7
#define KCZJ8				PA11	//继电器8
#define KCZJ9				PA12	//继电器9
#define KCZJ10				PA13	//继电器10
#define KCZJ11				PA15	//继电器11
#define KCZJ12				PC10	//继电器12

/* 8路数字输入 */
#define DI1					PC0		//数字输入1
#define DI2					PC1		//数字输入2
#define DI3					PC2		//数字输入3
#define DI4					PC3		//数字输入4
#define DI5					PC12	//数字输入5
#define DI6					PC11	//数字输入6
#define DI7					PD2		//数字输入7
#define DI8					PB3		//数字输入8

/* 8路模拟输入 */
#define AI1					PA0		//模拟输入1
#define AI2					PA1		//模拟输入2
#define AI3					PA4		//模拟输入3
#define AI4					PA5		//模拟输入4
#define AI5					PA6		//模拟输入5
#define AI6					PA7		//模拟输入6
#define AI7					PC4		//模拟输入7
#define AI8					PC5		//模拟输入8




// #define LED1_PIN            PC9
// #define LED2_PIN            PC8
// #define LED3_PIN            PC7
// #define LED4_PIN            PC6

// #define SW_FUN1             PC0 //Button1
// #define SW_FUN2             PC1 //Button2

// #define KCZJ1				PB0	//继电器1
// #define KCZJ2				PB1	//继电器2

// #define DI1					PA6	//数字输入1
// #define DI2					PA7	//数字输入2

// #define DO1					PC4	//光耦输出1
// #define DO2					PC5	//光耦输出2

// #define AI1					PA0	//模拟输入1
// #define AI2					PA1	//模拟输入2

// #define AO1					PC2	//模拟输出1
// #define AO2					PC3	//模拟输出2





// enum LED {
// 	RED1, RED2, GREEN1, GREEN2
// };

// /*LED GPIO switch*/
// #define GREEN1_OFF                  (digitalWrite(LED1_PIN, LOW))
// #define GREEN1_ON                   (digitalWrite(LED1_PIN, HIGH))
// #define RED1_OFF                    (digitalWrite(LED2_PIN, LOW))
// #define RED1_ON                     (digitalWrite(LED2_PIN, HIGH))
// #define GREEN2_OFF                  (digitalWrite(LED3_PIN, LOW))
// #define GREEN2_ON                   (digitalWrite(LED3_PIN, HIGH))
// #define RED2_OFF                    (digitalWrite(LED4_PIN, LOW))
// #define RED2_ON                     (digitalWrite(LED4_PIN, HIGH))

/*KCZJ GPIO switch*/
#define KCZJ1_ON                     (digitalWrite(KCZJ1, LOW))
#define KCZJ1_OFF                    (digitalWrite(KCZJ1, HIGH))
#define KCZJ2_ON                     (digitalWrite(KCZJ2, LOW))
#define KCZJ2_OFF                    (digitalWrite(KCZJ2, HIGH))

/*DigitalOutput GPIO switch*/
#define DO1_ON                     (digitalWrite(DO1, HIGH))
#define DO1_OFF                    (digitalWrite(DO1, LOW))
#define DO2_ON                     (digitalWrite(DO2, HIGH))
#define DO2_OFF                    (digitalWrite(DO2, LOW))



// #define LED_NO_REGISTER             (Some_Peripheral.LED_Display(RED1, 5))
// #define LED_RUNNING                 (Some_Peripheral.LED_Display(GREEN1, 10))
// #define LED_RESET_ROUTE             (Some_Peripheral.LED_Display(GREEN2, 20))
// #define LED_OPENING                 (Some_Peripheral.LED_Display(GREEN2, 10))
// #define LED_FORCE_OPENING           (Some_Peripheral.LED_Display(GREEN2, 5))
// #define LED_SELF_CHECK_ERROR        (Some_Peripheral.LED_Display(RED2, 5))
// #define LED_SET_LORA_PARA_ERROR     (Some_Peripheral.LED_Display(RED2, 20))

class Some_Peripherals {
public:
	/*Configurate some functional pins*/
	void Peripheral_GPIO_Pinmode(void);
	/*Configurate some functional pins*/
	void Peripheral_GPIO_Config(void);
	// void LED_Display(LED which_led, unsigned char freq);
	void Stop_LED(void);
	void Start_LED(void);
	// bool Peripheral_GPIO_Config_Init(void);//外设引脚初始化。
	

	// void Key_Buzz(unsigned int d); //800ms  1000 times.
};

void LED_Interrupt(void);

/*Create peripheral object*/
extern Some_Peripherals Some_Peripheral;

#endif