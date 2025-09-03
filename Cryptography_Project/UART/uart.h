/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART0 TM4C123GH6PM driver
 *
 * Author: Yousouf Soliman
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

/*******************************************************************************
 *                             Inclusions                                      *
 *******************************************************************************/
#include "Service_Layer/tm4c123gh6pm_registers.h"
#include "Service_Layer/Std_Types.h"
#include "PORT/Port.h"
/*******************************************************************************
 *                            Definitions                                      *
 *******************************************************************************/
#define F_CPU 16000000UL
#define UART_DATA_5BITS          0x0
#define UART_DATA_6BITS          0x1
#define UART_DATA_7BITS          0x2
#define UART_DATA_8BITS          0x3
#define UART_LCRH_WLEN_BITS_POS  5
#define UART_CTL_UARTEN_MASK     0x00000001
#define UART_CTL_TXE_MASK        0x00000100
#define UART_CTL_RXE_MASK        0x00000200
#define UART_FR_TXFE_MASK        0x00000080
#define UART_FR_RXFE_MASK        0x00000010

typedef enum {
    BitData_5 = 0x0, BitData_6 = 0x1, BitData_7 = 0x2, BitData_8 = 0x3
} UART_BitData;

typedef enum {
    Parity_Odd, Parity_Even, Parity_Disabled
} UART_Parity;

typedef enum {
    StopBit_1, StopBit_2
} UART_StopBit;

typedef enum {
    HSM_off, HSM_on
} UART_SpeedMode;


typedef enum {
    BaudRate_2400 = 2400,
    BaudRate_4800 = 4800,
    BaudRate_9600 = 9600,
    BaudRate_14400 = 14400,
    BaudRate_19200 = 19200,
    BaudRate_28800 = 28800,
    BaudRate_38400 = 38400,
    BaudRate_57600 = 57600,
    BaudRate_76800 = 76800,
    BaudRate_115200 = 115200,
    BaudRate_230400 = 230400,
    BaudRate_250K = 250000,
    BaudRate_500K = 500000,
    BaudRate_1M = 1000000
} UART_BaudRate;

typedef struct {
    UART_BitData bit_data;
    UART_Parity parity;
    UART_StopBit stop_bit;
    UART_BaudRate baud_rate;
    UART_SpeedMode speed_mode;
} UART_ConfigType;


/*******************************************************************************
 *                            Function Prototypes                              *
 *******************************************************************************/

void UART0_Init(UART_ConfigType *Config_Ptr);
//void UART0_Init(void);
void UART0_SendByte(uint8 data);
void UART0_SendString(const uint8 *pData);
void UART0_ReceiveString(uint8 *pData);
void UART0_SendData(const uint8 *pData, uint32 uSize);
void UART0_ReceiveData(uint8 *pData, uint32 uSize);

void UART1_Init(UART_ConfigType *Config_Ptr);
void UART1_SendByte(uint8 data);
uint8 UART1_ReceiveByte(void);
void UART1_SendString(const uint8 *pData);
void UART1_ReceiveString(uint8 *pData);
void UART1_SendData(const uint8 *pData, uint32 uSize);
void UART1_ReceiveData(uint8 *pData, uint32 uSize);
uint8 UART1_DataAvailable(void);
#endif /* UART_H_ */
/*******************************************************************************
 *                            End of File                                      *
 *******************************************************************************/
