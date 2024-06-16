#include "MFRC522.h"
#include "stm32f4xx.h"
extern SPI_HandleTypeDef hspi1;

#define MAX_LE N 16


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

void Write_MFRC522(uint8_t addr, uint8_t val)
{
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
	RC522_SPI_Transfer((addr<<1)&0x7E);	
	RC522_SPI_Transfer(val);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
}

uint8_t Read_MFRC522(uint8_t addr)
{
	uint8_t val;
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
	RC522_SPI_Transfer(((addr<<1)&0x7E) | 0x80);	
	val = RC522_SPI_Transfer(0x00);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
	return val;	
}

void SetBitMask(uint8_t reg, uint8_t mask)  
{
    uint8_t tmp;
    tmp = Read_MFRC522(reg);
    Write_MFRC522(reg, tmp | mask);  // set bit mask
}

void AntennaOn(void)
{
	

Read_MFRC522(TxControlReg);
//	if (!(temp & 0x03))
//	{
//		SetBitMask(TxControlReg, 0x03);
//	}
	SetBitMask(TxControlReg, 0x03);
}
void MFRC522_Init(void)
{

	//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
	//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
	Write_MFRC522(0x01, 0x0F); //Soft Reset
	Write_MFRC522(TModeReg, 0x8D);		//auto=1; f(Timer) = 6.78MHz/TPreScaler
	Write_MFRC522(TPrescalerReg, 0x3E);	//TModeReg[3..0] + TPrescalerReg
	Write_MFRC522(TReloadRegL, 30);  
	Write_MFRC522(TReloadRegH, 0);
	Write_MFRC522(0x26, 0x70);	
	Write_MFRC522(TxAutoReg, 0x40);		//100%ASK
	Write_MFRC522(ModeReg, 0x3D);		//CRC Original value 0x6363	???
	AntennaOn();		//Mo Anten
}

void ClearBitMask(uint8_t reg, uint8_t mask)  
{
    uint8_t tmp;
    tmp = Read_MFRC522(reg);
    Write_MFRC522(reg, tmp & (~mask));  // clear bit mask
} 

uint8_t MFRC522_ToCard(uint8_t command, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, uint *backLen)
{
    uint8_t status = MI_ERR;
    uint8_t irqEn = 0x00;
    uint8_t waitIRq = 0x00;
    uint8_t lastBits;
    uint8_t n;
    uint i;

    switch (command)
    {
        case PCD_AUTHENT:		//Acknowledging the liver
		{
			irqEn = 0x12;
			waitIRq = 0x10;
			break;
		}
		case PCD_TRANSCEIVE:	// FIFO data collection
		{
			irqEn = 0x77;
			waitIRq = 0x30;
			break;
		}
		default:
			break;
    }
   
    Write_MFRC522(CommIEnReg, irqEn|0x80);	//Yeu cau ngat
    ClearBitMask(CommIrqReg, 0x80);			//Clear all the bits
    SetBitMask(FIFOLevelReg, 0x80);			//FlushBuffer=1, FIFO
    
	Write_MFRC522(CommandReg, PCD_IDLE);	//NO action; Huy bo lenh hien hanh	???

	// Record in FIFO
    for (i=0; i<sendLen; i++)
    {   
		Write_MFRC522(FIFODataReg, sendData[i]);    
	}

	//chay
	Write_MFRC522(CommandReg, command);
    if (command == PCD_TRANSCEIVE)
    {    
		SetBitMask(BitFramingReg, 0x80);		//StartSend=1,transmission of data starts  
	}   
    
	//The team is allowed to be stored
	i = 2000;	//i tuy thuoc tan so thach anh, thoi gian toi da cho the M1 la 25ms
    do 
    {
		//CommIrqReg[7..0]
		//Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
        n = Read_MFRC522(CommIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitIRq));

    ClearBitMask(BitFramingReg, 0x80);			//StartSend=0
	
    if (i != 0)
    {    
        if(!(Read_MFRC522(ErrorReg) & 0x1B))	//BufferOvfl Collerr CRCErr ProtecolErr
        {
            status = MI_OK;
            if (n & irqEn & 0x01)
            {   
				status = MI_NOTAGERR;			//??   
			}

            if (command == PCD_TRANSCEIVE)
            {
               	n = Read_MFRC522(FIFOLevelReg);
              	lastBits = Read_MFRC522(ControlReg) & 0x07;
                if (lastBits)
                {   
					*backLen = (n-1)*8 + lastBits;   
				}
                else
                {   
					*backLen = n*8;   
				}

                if (n == 0)
                {   
					n = 1;    
				}
                if (n > MAX_LEN)
                {   
					n = MAX_LEN;   
				}
				
				//FIFO doc in the received data
                for (i=0; i<n; i++)
                {   
					backData[i] = Read_MFRC522(FIFODataReg);    
				}
            }
        }
        else
        {   
			status = MI_ERR;  
		}
        
    }
	
    //SetBitMask(ControlReg,0x80);           //timer stops
    //Write_MFRC522(CommandReg, PCD_IDLE); 

    return status;
}


uint8_t MFRC522_Request(uint8_t reqMode, uint8_t *TagType)
{
	uint8_t status;  
	uint backBits;			//The bits are manipulated

	Write_MFRC522(BitFramingReg, 0x07);		//TxLastBists = BitFramingReg[2..0]	???
	
	TagType[0] = reqMode;
	status = MFRC522_ToCard(PCD_TRANSCEIVE, TagType, 1, TagType, &backBits);

	if ((status != MI_OK) || (backBits != 0x10))
	{    
		status = MI_ERR;
	}
   
	return status;
}

uint8_t MFRC522_Anticoll(uint8_t *serNum)
{
    uint8_t status;
    uint8_t i;
	uint8_t serNumCheck=0;
    uint unLen;
    

    //ClearBitMask(Status2Reg, 0x08);		//TempSensclear
    //ClearBitMask(CollReg,0x80);			//ValuesAfterColl
	Write_MFRC522(BitFramingReg, 0x00);		//TxLastBists = BitFramingReg[2..0]
 
    serNum[0] = PICC_ANTICOLL;
    serNum[1] = 0x20;
    status = MFRC522_ToCard(PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);

    if (status == MI_ERR)
	{
		//Check the serial number
		for (i=0; i<4; i++)
		{   
		 	serNumCheck ^= serNum[i];
		}
		if (serNumCheck != serNum[i])
		{   
			status = MI_OK;    
		}
    }

    //SetBitMask(CollReg, 0x80);		//ValuesAfterColl=1

    return status;
} 

