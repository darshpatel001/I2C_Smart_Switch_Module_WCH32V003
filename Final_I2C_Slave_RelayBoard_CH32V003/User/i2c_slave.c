#include "i2c_slave.h"

/* Buffers */
u8 TxData[I2C_BUFFER_SIZE];
u8 RxData[I2C_BUFFER_SIZE];

/* Slave state */
volatile uint16_t slave_recv_len = 0;
volatile uint16_t slave_send_len = 0;
volatile uint8_t  slave_state    = 0;

/* Helper: Load string into TxData */
void I2C_SendString(const char *str) {
    size_t len = strlen(str);
    if(len > I2C_BUFFER_SIZE) len = I2C_BUFFER_SIZE;  // prevent overflow
    memcpy(TxData, str, len);
}

void I2C_SendByte(uint8_t value) {
    TxData[0] = value;
    slave_send_len = 1;   // tell ISR only 1 byte valid
}

/* Init I2C1 in Slave mode */
void IIC_Init(u32 bound, u16 address)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    I2C_InitTypeDef  I2C_InitTSturcture={0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    // PC2 = SCL, PC1 = SDA
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    I2C_InitTSturcture.I2C_ClockSpeed          = bound;
    I2C_InitTSturcture.I2C_Mode                = I2C_Mode_I2C;
    I2C_InitTSturcture.I2C_DutyCycle           = I2C_DutyCycle_16_9;
    I2C_InitTSturcture.I2C_OwnAddress1         = address;
    I2C_InitTSturcture.I2C_Ack                 = I2C_Ack_Enable;
    I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C1, &I2C_InitTSturcture);

    I2C_Cmd(I2C1, ENABLE);

    // Enable interrupts
    NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = I2C1_ER_IRQn;
    NVIC_Init(&NVIC_InitStructure);

    I2C_ITConfig(I2C1, I2C_IT_BUF, ENABLE);
    I2C_ITConfig(I2C1, I2C_IT_EVT, ENABLE);
    I2C_ITConfig(I2C1, I2C_IT_ERR, ENABLE);
}

/* User callback pointer */
static void (*i2c_on_receive)(uint8_t *data, uint16_t len) = 0;

/* Register callback */
void I2C_RegisterReceiveCallback(void (*cb)(uint8_t *data, uint16_t len))
{
    i2c_on_receive = cb;
}

/* I2C Event IRQ Handler */
void I2C1_EV_IRQHandler(void)
{
    if(I2C_GetITStatus(I2C1, I2C_IT_ADDR) != RESET) {
        if(I2C_GetFlagStatus(I2C1, I2C_FLAG_TRA) && I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE)) {
            // Master is reading from slave
            slave_send_len = 0;
        } else {
            (void)I2C_ReadRegister(I2C1, I2C_Register_STAR2);
        }
    }
    else if(I2C_GetITStatus(I2C1, I2C_IT_RXNE) != RESET) {
        RxData[slave_recv_len++] = I2C_ReceiveData(I2C1);
    }
    else if(I2C_GetITStatus(I2C1, I2C_IT_STOPF) != RESET) {
        I2C1->CTLR1 |= 0x0001; // clear STOPF
        if(i2c_on_receive) {
            i2c_on_receive(RxData, slave_recv_len);  // ? callback with received bytes
        }
        slave_recv_len = 0;  // reset for next
    }
    else if(I2C_GetITStatus(I2C1, I2C_IT_TXE) != RESET) {
        if(slave_send_len < I2C_BUFFER_SIZE) {
            I2C_SendData(I2C1, TxData[slave_send_len++]);
        } else {
            I2C_SendData(I2C1, 0xFF);
        }
    }
}

/* I2C Error IRQ Handler */
void I2C1_ER_IRQHandler(void)
{
    if(I2C_GetITStatus(I2C1, I2C_IT_AF)) {
        I2C_ClearITPendingBit(I2C1, I2C_IT_AF);
        slave_state = 0xFF;  // transmission ended
    }
}

