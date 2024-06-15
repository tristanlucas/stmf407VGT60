#ifndef __STM32F4_RC522_H
#define __STM32F4_RC522_H

#include "stm32f4xx_hal.h"

#define     TModeReg              0x2A
#define     TPrescalerReg         0x2B
#define     TReloadRegH           0x2C
#define     TReloadRegL           0x2D
#define     ModeReg               0x11
#define     TxAutoReg             0x15

// Function prototypes
void MFRC522_Init(void);
void MFRC522_WriteRegister(uint8_t reg, uint8_t value);
uint8_t MFRC522_ReadRegister(uint8_t reg);

#endif /* __STM32F4_RC522_H */
