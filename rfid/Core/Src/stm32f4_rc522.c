/**
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen MAJERLE, 2014
 * |
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * |
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */
#include "stm32f4_rc522.h"
#include "stm32f4xx.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
extern SPI_HandleTypeDef hspi1;

#define NSS_PIN 							GPIO_PIN_4
#define NSS_GPIO_PORT 				GPIOA
#define RST_PIN 							GPIO_PIN_0
#define RST_GPIO_PORT 				GPIOB 

uint8_t MFRC522_CheckVersion(void) {
    uint8_t version = MFRC522_ReadRegister(VersionReg);
    return (version == 0x90) ? MI_OK : MI_ERR;
}

void MFRC522_SPI_Transmit(uint8_t* data, uint16_t size) {
    HAL_SPI_Transmit(&hspi1, data, size, HAL_MAX_DELAY);
}

void MFRC522_SPI_Receive(uint8_t* data, uint16_t size) {
    HAL_SPI_Receive(&hspi1, data, size, HAL_MAX_DELAY);
}

 //Write to a register
void MFRC522_WriteRegister(uint8_t reg, uint8_t value) {
    uint8_t data[2];
    data[0] = reg & 0x7E;
    data[1] = value;
    HAL_GPIO_WritePin(NSS_GPIO_PORT, NSS_PIN, GPIO_PIN_RESET);
    MFRC522_SPI_Transmit(data, sizeof(data));
    HAL_GPIO_WritePin(NSS_GPIO_PORT, NSS_PIN, GPIO_PIN_SET);
}


// Read from a register
uint8_t MFRC522_ReadRegister(uint8_t reg) {
    uint8_t data[2];
    data[0] = (reg & 0x7E) | 0x80;
    HAL_GPIO_WritePin(NSS_GPIO_PORT, NSS_PIN, GPIO_PIN_RESET);
    MFRC522_SPI_Transmit(data, 1);
    MFRC522_SPI_Receive(&data[1], 1);
    HAL_GPIO_WritePin(NSS_GPIO_PORT, NSS_PIN, GPIO_PIN_SET);
    return data[1];
}

// Turn the antenna on
void MFRC522_AntennaOn(void) {
    uint8_t temp = MFRC522_ReadRegister(TxControlReg);
    if ((temp & 0x03) != 0x03) {
        MFRC522_WriteRegister(TxControlReg, temp | 0x03);
    }
}
// Initialize the MFRC522 module
void MFRC522_Init(void) {
    // Set the RST pin high
//    HAL_GPIO_WritePin(RST_GPIO_PORT, RST_PIN, GPIO_PIN_SET);
//    HAL_Delay(50);
//    HAL_GPIO_WritePin(RST_GPIO_PORT, RST_PIN, GPIO_PIN_RESET);
//    HAL_Delay(50);
//    HAL_GPIO_WritePin(RST_GPIO_PORT, RST_PIN, GPIO_PIN_SET);
//    HAL_Delay(50);

    // Soft reset
    MFRC522_WriteRegister(CommandReg, PCD_RESETPHASE);
    HAL_Delay(50);

    // Set the Tx and Rx modes
    MFRC522_WriteRegister(TModeReg, 0x8D);
    MFRC522_WriteRegister(TPrescalerReg, 0x3E);
    MFRC522_WriteRegister(TReloadRegL, 30);
    MFRC522_WriteRegister(TReloadRegH, 0);
    MFRC522_WriteRegister(TxASKReg, 0x40);
    MFRC522_WriteRegister(ModeReg, 0x3D);
    MFRC522_AntennaOn();
}
