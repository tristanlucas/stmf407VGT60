#ifndef __STM32F4_RC522_H
#define __STM32F4_RC522_H

#include "stm32f4xx_hal.h"
#include "main.h"
// Register addresses
#define CommandReg       0x01
#define CommIEnReg       0x02
#define CommIrqReg       0x04
#define FIFOLevelReg     0x0A
#define FIFODataReg      0x09
#define BitFramingReg    0x0D
#define ErrorReg         0x06
#define ControlReg       0x0C
#define TModeReg         0x2A
#define TPrescalerReg    0x2B
#define TReloadRegL      0x2C
#define TReloadRegH      0x2D
#define TxASKReg         0x15
#define ModeReg          0x11
#define TxControlReg     0x14

// MFRC522 command codes
#define PCD_IDLE              0x00
#define PCD_TRANSCEIVE        0x0C
#define PCD_AUTHENT           0x0E
#define PCD_RECEIVE           0x08
#define PCD_RESETPHASE        0x0F
#define PCD_CALCCRC           0x03

// MIFARE command codes
#define PICC_REQIDL           0x26
#define PICC_ANTICOLL         0x93
#define PICC_SELECTTAG        0x93
#define PICC_AUTHENT1A        0x60
#define PICC_AUTHENT1B        0x61
#define PICC_READ             0x30
#define PICC_WRITE            0xA0
#define PICC_DECREMENT        0xC0
#define PICC_INCREMENT        0xC1
#define PICC_RESTORE          0xC2
#define PICC_TRANSFER         0xB0
#define PICC_HALT             0x50

#define MI_OK                 0
#define MI_NOTAGERR           1
#define MI_ERR                2

// Card types
#define MF1S50                0x00
#define MF1S70                0x01
#define MF1ULT                0x02

#define VersionReg 0x37

// Function prototypes

uint8_t MFRC522_CheckVersion(void);
void MFRC522_Init(void);
uint8_t MFRC522_CheckCard(uint8_t* uid, uint8_t* uidLength);
void MFRC522_WriteRegister(uint8_t reg, uint8_t value);
uint8_t MFRC522_ReadRegister(uint8_t reg);
void MFRC522_AntennaOn(void);
void MFRC522_ClearBitMask(uint8_t reg, uint8_t mask);
void MFRC522_SetBitMask(uint8_t reg, uint8_t mask);

#endif /* __STM32F4_RC522_H */
