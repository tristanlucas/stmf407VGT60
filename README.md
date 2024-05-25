
```

- RC522_SPI_Transfer: A utility function to transfer data over SPI communication protocol.
- Write_MFRC522 and Read_MFRC522: Functions to write to and read from specific registers of the MFRC522 module.
- SetBitMask and ClearBitMask: Functions to set or clear specific bits in a register of the MFRC522.
- AntennaOn and AntennaOff: Functions to turn the antenna of the MFRC522 module on and off.
- MFRC522_Reset and MFRC522_Init: Functions to reset and initialize the MFRC522 module.
- MFRC522_ToCard: Function to communicate with ISO14443 cards, such as Mifare cards.
- MFRC522_Request: Function to find cards and read their type.
- MFRC522_Anticoll: Function to perform anti-collision detection and read the serial number of a card.
- CalulateCRC: Function to calculate CRC for MFRC522 communication.
- MFRC522_SelectTag: Function to select a card and read its memory capacity.
- MFRC522_Auth: Function to verify a card password.
- MFRC522_Read and MFRC522_Write: Functions to read from and write to a block of a card's memory.
- MFRC522_Halt: Function to command a card into hibernation.
```

```
PA4 for NSS (GPIO Output) - Not Hardware NSS Signal, using with Software Signal Type. 
PB0 for RST (GPIO Output)

PA5 > SPI1_SCK
PA6 > MISO
PA7 > MOSI


USART2_TX > PA2
USART2_RX > PA3 
with asynchronous. 

```