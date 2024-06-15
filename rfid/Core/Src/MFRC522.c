#include "MFRC522.h"
#include "stm32f4xx.h"
extern SPI_HandleTypeDef hspi1;


//uint8_t RC522_SPI_Transfer(uint8_t data)
//{
//	uint8_t rx_data;
//	//HAL_SPI_TransmitReceive(&hspi1,&data,&rx_data,1,100);
//	if(HAL_SPI_Transmit_IT(&hspi1, &data, sizeof(&data))== HAL_OK){
//		
//	}
//	return rx_data;
//}

uint8_t RC522_SPI_Transfer(uint8_t data)
{
	uint8_t rx_data;
	//HAL_SPI_TransmitReceive(&hspi1,&data,&rx_data,1,100);
	HAL_SPI_TransmitReceive(&hspi1, &data, &rx_data, 1, 50);
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
	return rx_data;
	
}

void MFRC522_WriteRegister(uint8_t addr, uint8_t val) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	RC522_SPI_Transfer((addr<<1) &0x7E);	
	RC522_SPI_Transfer(val);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

uint8_t MFRC522_ReadRegister(uint8_t addr) {
	uint8_t val;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	RC522_SPI_Transfer(((addr<<1)&0x7E) | 0x80);	
	val = RC522_SPI_Transfer(0x00);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
	return val;	
}

void MFRC522_Init(void)
{

	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
	MFRC522_WriteRegister(0x01, 0x0F); //Soft Reset
	MFRC522_WriteRegister(TModeReg, 0x8D);		//auto=1; f(Timer) = 6.78MHz/TPreScaler
	MFRC522_WriteRegister(TPrescalerReg, 0x3E);	//TModeReg[3..0] + TPrescalerReg
	MFRC522_WriteRegister(TReloadRegL, 30);           
	MFRC522_WriteRegister(TReloadRegH, 0);
	MFRC522_WriteRegister(TxAutoReg, 0x40);		//100%ASK
	MFRC522_WriteRegister(ModeReg, 0x3D);		//CRC Original value 0x6363	???
	//AntennaOn();		//Mo Anten
}
