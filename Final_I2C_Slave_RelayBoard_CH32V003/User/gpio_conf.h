
#include "debug.h"

/* ---------------- Pin Naming ---------------- */

/* Blink LEDs */
#define RELAY_PORT   GPIOC
#define RELAY1_PIN   GPIO_Pin_3   // PC3
#define RELAY2_PIN   GPIO_Pin_4   // PC4
#define RELAY3_PIN   GPIO_Pin_5   // PC5
#define RELAY4_PIN   GPIO_Pin_6   // PC6

void GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    /* Enable clocks for GPIOC */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    /* Configure PC3, PC4, PC5, PC6 as Output Push-Pull */
    GPIO_InitStructure.GPIO_Pin = RELAY1_PIN | RELAY2_PIN | RELAY3_PIN | RELAY4_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(RELAY_PORT, &GPIO_InitStructure);
}
