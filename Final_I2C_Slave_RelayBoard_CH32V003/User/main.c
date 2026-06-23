// #include "debug.h"
// #include "gpio_conf.h"
// #include "i2c_slave.h"

// volatile uint8_t prevByte = 0xFF;  // previous applied lower nibble
// volatile uint8_t currByte = 0;     // current lower nibble

// void MyReceiveCallback(uint8_t *data, uint16_t len)
// {
//     if(len == 0) return;  // no data received

//  // Keep only lower nibble (bits 0-3)
//  currByte = data[0] & 0x0F;

//  // Only update relays if lower nibble changed
//  if(currByte != prevByte)
//  {
//      printf("Lower nibble changed: 0x%X -> 0x%X\r\n", prevByte, currByte);

//  // Update relays (active HIGH)
//  GPIO_WriteBit(RELAY_PORT, RELAY1_PIN, (currByte & 0x01) ? Bit_SET : Bit_RESET);
//  GPIO_WriteBit(RELAY_PORT, RELAY2_PIN, (currByte & 0x02) ? Bit_SET : Bit_RESET);
//  GPIO_WriteBit(RELAY_PORT, RELAY3_PIN, (currByte & 0x04) ? Bit_SET : Bit_RESET);
//  GPIO_WriteBit(RELAY_PORT, RELAY4_PIN, (currByte & 0x08) ? Bit_SET : Bit_RESET);
//  prevByte = currByte;  // save for next comparison
// }
// }


// int main(void)
// {
//     SystemCoreClockUpdate();
//     Delay_Init();
//     USART_Printf_Init(115200);

//  printf("CH32V003 I2C Slave ready (UART debug)\r\n");

//  IIC_Init(100000, 0x04);  // Slave address = 0x02
//  I2C_RegisterReceiveCallback(MyReceiveCallback);

//  while(1) {
//      // nothing here, everything handled in ISR
//  }
// }


#include "debug.h"
#include "gpio_conf.h"
#include "i2c_slave.h"
#include <stdbool.h>
#include <stdint.h>

// #define Debug_relay

/* ---------------- Global State ---------------- */
volatile uint8_t prevByte = 0x00;  // last applied nibble
volatile uint8_t currByte = 0;     // new nibble from ISR
volatile bool rxFlag = false;      // flag set by ISR

/* ---------------- Helper Functions ---------------- */
static void Relay_Update (uint8_t value) {
    GPIO_WriteBit (RELAY_PORT, RELAY1_PIN, (value & 0x01) ? Bit_SET : Bit_RESET);
    GPIO_WriteBit (RELAY_PORT, RELAY2_PIN, (value & 0x02) ? Bit_SET : Bit_RESET);
    GPIO_WriteBit (RELAY_PORT, RELAY3_PIN, (value & 0x04) ? Bit_SET : Bit_RESET);
    GPIO_WriteBit (RELAY_PORT, RELAY4_PIN, (value & 0x08) ? Bit_SET : Bit_RESET);
}

#ifdef Debug_relay
static void Relay_DebugPrint (uint8_t oldVal, uint8_t newVal) {
    printf ("\r\n[Relay Update] 0x%X ¡ú 0x%X | State: ", oldVal, newVal);

    for (int i = 0; i < 4; i++) {
        if (newVal & (1 << i))
            printf ("[R%d:ON] ", i + 1);
        else
            printf ("[R%d:OFF] ", i + 1);
    }
    printf ("\r\n");
}
#endif

/* ---------------- I?C Callback ---------------- */
void MyReceiveCallback (uint8_t *data, uint16_t len) {
    if (len == 0)
        return;

    uint8_t newNibble = (~data[0]) & 0x0F;  // keep only lower 4 bits

    if (newNibble != prevByte) {
        currByte = newNibble;
        rxFlag = true;  // notify main loop
    }
}

/* ---------------- Main ---------------- */
int main (void) {
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init (115200);
    
    printf ("\r\n=== CH32V003 I?C Relay Controller ===\r\n");
    GPIO_Config();
    // ?? Confirm whether your library expects 0x04 (7-bit) or 0x08 (8-bit)
    IIC_Init (100000, 0x04); // 0x04 - 0x02 -> Relay board, 0x08 - 0x04 -> Fan Reg board,
    I2C_RegisterReceiveCallback (MyReceiveCallback);

    printf ("I?C Slave ready @ address 0x04\r\n");
    printf ("Waiting for master commands...\r\n");
    Relay_Update (0x00);
    while (1) {
        if (rxFlag) {
            __disable_irq();
            uint8_t newVal = currByte;
            rxFlag = false;
            __enable_irq();

            Relay_Update (newVal);
            #ifdef Debug_relay
            Relay_DebugPrint (prevByte, newVal);
            #endif
            prevByte = newVal;
        }
    }
}
