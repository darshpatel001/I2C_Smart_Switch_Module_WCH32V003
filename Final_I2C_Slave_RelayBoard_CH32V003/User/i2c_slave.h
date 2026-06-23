#ifndef __I2C_SLAVE_H
#define __I2C_SLAVE_H

#include "debug.h"
#include <string.h>   // for memcpy, strlen

#define I2C_BUFFER_SIZE   64

/* Slave default address */
#define I2C_SLAVE_ADDR    0x04 // master Detect 0x01

/* Extern Buffers */
extern u8 TxData[I2C_BUFFER_SIZE];
extern u8 RxData[I2C_BUFFER_SIZE];

/* Extern State Variables */
extern volatile uint16_t slave_recv_len;
extern volatile uint16_t slave_send_len;
extern volatile uint8_t  slave_state;

/* API */
void I2C_SendByte(uint8_t value);
void I2C_SendString(const char *str);
void IIC_Init(u32 bound, u16 address);



/* IRQ Handlers */
void I2C1_EV_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void I2C1_ER_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void I2C_RegisterReceiveCallback(void (*cb)(uint8_t *data, uint16_t len));

#endif /* __I2C_SLAVE_H */
