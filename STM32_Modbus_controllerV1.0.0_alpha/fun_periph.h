#ifndef _FUN_PERIPH_H
#define _FUN_PERIPH_H

#include <Arduino.h>

/*一些外设的引脚*/
/*Some peripheral GPIO configuration*/

#define LED1_PIN            PC9
#define LED2_PIN            PC8
#define LED3_PIN            PC7
#define LED4_PIN            PC6

#define SW_FUN1             PC0 //Button1
#define SW_FUN2             PC1 //Button2

#define KCZJ1				PB0	//继电器1
#define KCZJ2				PB1	//继电器2

#define DI1					PA6	//数字输入1
#define DI2					PA7	//数字输入2

#define DO1					PC4	//光耦输出1
#define DO2					PC5	//光耦输出2





enum LED {
	RED1, RED2, GREEN1, GREEN2
};

/*LED GPIO switch*/
#define GREEN1_OFF                  (digitalWrite(LED1_PIN, LOW))
#define GREEN1_ON                   (digitalWrite(LED1_PIN, HIGH))
#define RED1_OFF                    (digitalWrite(LED2_PIN, LOW))
#define RED1_ON                     (digitalWrite(LED2_PIN, HIGH))
#define GREEN2_OFF                  (digitalWrite(LED3_PIN, LOW))
#define GREEN2_ON                   (digitalWrite(LED3_PIN, HIGH))
#define RED2_OFF                    (digitalWrite(LED4_PIN, LOW))
#define RED2_ON                     (digitalWrite(LED4_PIN, HIGH))

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



#define LED_NO_REGISTER             (Some_Peripheral.LED_Display(RED1, 5))
#define LED_RUNNING                 (Some_Peripheral.LED_Display(GREEN1, 10))
#define LED_RESET_ROUTE             (Some_Peripheral.LED_Display(GREEN2, 20))
#define LED_OPENING                 (Some_Peripheral.LED_Display(GREEN2, 10))
#define LED_FORCE_OPENING           (Some_Peripheral.LED_Display(GREEN2, 5))
#define LED_SELF_CHECK_ERROR        (Some_Peripheral.LED_Display(RED2, 5))
#define LED_SET_LORA_PARA_ERROR     (Some_Peripheral.LED_Display(RED2, 20))

class Some_Peripherals {
public:
	/*Configurate some functional pins*/
	void Peripheral_GPIO_Config(void);
	void LED_Display(LED which_led, unsigned char freq);
	void Stop_LED(void);
	void Start_LED(void);

	// void Key_Buzz(unsigned int d); //800ms  1000 times.
};

void LED_Interrupt(void);

/*Create peripheral object*/
extern Some_Peripherals Some_Peripheral;

#endif